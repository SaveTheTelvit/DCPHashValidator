#include "xmlfilereadermodule.h"

DCPPackage* XmlFileReaderModule::getFullAsset(const QString &path, int mode)
{
    DCPPackage *package = readAssetmap(path);
    if (!package) return nullptr;
    if ((*package).isPKLExist()) {
        if (assetPKL((*package).PKL()->path, package)) {
            if (mode == WithVolindex) {
                Asset volAsset;
                volAsset.path = path.mid(0, path.lastIndexOf('/')) + "/VOLINDEX" + ((path.mid(path.size() - 4) == ".xml") ? ".xml" : "");
                volAsset.type = Asset::AdditionalFiles;
                package->push_back(volAsset);
            }
        } else (*package).setPKLDamageSus(true);
        return package;
    }
    return package;
}

DCPPackage* XmlFileReaderModule::readAssetmap(const QString &path)
{
    QFile file(path);
    if (file.open(QIODevice::ReadOnly)) {
        DCPPackage *package = new DCPPackage;
        QXmlStreamReader xml(&file);
        while (!xml.atEnd()) {
            QXmlStreamReader::TokenType token = xml.readNext();
            if (xml.hasError()) return nullptr;
            if (token == QXmlStreamReader::StartElement) {
                if (xml.name().toString() == "Asset") {
                    package->push_back(readAssetmapData(xml, path));
                }
            }
        }
        return package;
    }
    return nullptr;
}

DCPPackage* XmlFileReaderModule::readPKL(const QString &path)
{
    QFile file(path);
    if (file.open(QIODevice::ReadOnly)) {
        DCPPackage *package = new DCPPackage;
        QXmlStreamReader xml(&file);
        while (!xml.atEnd()) {
            QXmlStreamReader::TokenType token = xml.readNext();
            if (xml.hasError()) return nullptr;
            if (token == QXmlStreamReader::StartElement) {
                if (xml.name().toString() == "Asset") {
                    package->push_back(readPKLData(xml));
                }
            }
        }
        return package;
    }
    return nullptr;
}

bool XmlFileReaderModule::assetPKL(const QString &path, DCPPackage *package)
{
    QFile file(path);
    DCPPackage bufferData = *package;
    if (file.exists() && file.open(QIODevice::ReadOnly)) {
        QList<int> readedIndex;
        QXmlStreamReader xml(&file);
        while (!xml.atEnd()) {
            QXmlStreamReader::TokenType token = xml.readNext();
            if (xml.hasError()) {
                *package = bufferData;
                return false;
            }
            if (token == QXmlStreamReader::StartElement) {
                if (xml.name().toString() == "Asset") {
                    assetPKLData(xml, package, readedIndex);
                }
            }
        }
        return true;
    }
    return false;
}

Asset XmlFileReaderModule::readAssetmapData(QXmlStreamReader &xml, const QString &path)
{
    Asset asset;
    qCritical() << "is read data";
    while (!xml.atEnd() && !xml.hasError()) {
        QXmlStreamReader::TokenType token = xml.readNext();
        if (token == QXmlStreamReader::StartElement) {
            QString name = xml.name().toString();
            if (name == "Id") {
                asset.id = xml.readElementText().mid(9);
            } else if (name == "Path") {
                asset.path = xml.readElementText();
                if (asset.path.contains("cpl.", Qt::CaseInsensitive)|| asset.path.contains("cpl_", Qt::CaseInsensitive)) asset.type = Asset::CompositionPlaylist;
                if (asset.path.contains("pkl.", Qt::CaseInsensitive)|| asset.path.contains("pkl_", Qt::CaseInsensitive)) asset.type = Asset::PackingList;
                int index;
                while ((index = asset.path.indexOf('\\')) != -1) {
                    asset.path.remove(index, 1);
                    asset.path.insert(index - 1, "/");
                }
                asset.path = path.mid(0, path.lastIndexOf('/')) + '/' + asset.path;
            }
        } else if (token == QXmlStreamReader::EndElement && xml.name().toString() == "Asset") {
            return asset;
        }
    }
    return asset;
}

Asset XmlFileReaderModule::readPKLData(QXmlStreamReader &xml)
{
    Asset asset;
    while (!xml.atEnd() && !xml.hasError()) {
        QXmlStreamReader::TokenType token = xml.readNext();
        if (token == QXmlStreamReader::StartElement) {
            QString name = xml.name().toString();
            if (name == "Id") {
                asset.id = xml.readElementText().mid(9);
            } else if (name == "Hash") {
                asset.hash = xml.readElementText();
            }
        } else if (token == QXmlStreamReader::EndElement && xml.name().toString() == "Asset") {
            asset.inPkl = true;
            return asset;
        }
    }
    return asset;
}

void XmlFileReaderModule::assetPKLData(QXmlStreamReader &xml, DCPPackage *package, QList<int> &readedIndex)
{
    int index = -1;
    QString hashBuffer = "";
    while (!xml.atEnd() && !xml.hasError()) {
        QXmlStreamReader::TokenType token = xml.readNext();
        if (token == QXmlStreamReader::StartElement) {
            QString name = xml.name().toString();
            if (name == "Id") {
                QString id = xml.readElementText().mid(9);
                for (int i = 0; i < package->size(); ++i) {
                    if (readedIndex.contains(i)) continue;
                    if ((*package)[i]->id == id) {
                        if (hashBuffer.isEmpty()) {
                            index = i;
                        } else {
                            (*package)[i]->id = hashBuffer;
                        }
                        readedIndex.push_back(i);
                        break;
                    }
                }
            } else if (name == "Hash") {
                if (index != -1) {
                    (*package)[index]->hash = xml.readElementText();
                } else hashBuffer = xml.readElementText();
            }
        } else if (token == QXmlStreamReader::EndElement && xml.name().toString() == "Asset") {
            if (index != -1) (*package)[index]->inPkl = true;
            return;
        }
    }
}
