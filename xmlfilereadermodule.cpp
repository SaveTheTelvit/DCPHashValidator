#include "xmlfilereadermodule.h"

DCPPackage* XmlFileReaderModule::getFullAsset(const QString &path, int mode)
{
    DCPPackage *package = readAssetmap(path);
    if (!package) return nullptr;
    if (package->isPKLExist()) {
        if (assetPKL(package->path() + '/' + package->PKL()->path, package)) {
            if (mode == WithVolindex) {
                Asset volAsset;
                volAsset.path = package->path() + "/VOLINDEX" + ((path.mid(path.size() - 4) == ".xml") ? ".xml" : "");
                volAsset.type = Asset::AdditionalFiles;
                package->push_back(volAsset);
            }
        } else package->setPKLDamagedState(true);
        return package;
    }
    return package;
}

DCPPackage* XmlFileReaderModule::readAssetmap(const QString &path)
{
    QFile file(path);
    if (file.open(QIODevice::ReadOnly)) {
        DCPPackage *package = new DCPPackage;
        package->setPath(path.mid(0, path.lastIndexOf('/')));
        QXmlStreamReader xml(&file);
        while (!xml.atEnd()) {
            QXmlStreamReader::TokenType token = xml.readNext();
            if (xml.hasError()) {
                delete package;
                return nullptr;
            }
            if (token == QXmlStreamReader::StartElement) {
                if (xml.name().toString() == "Asset") {
                    Asset buffer = readAssetmapData(xml);
                    if (buffer.id.isEmpty()) {
                        delete package;
                        return nullptr;
                    }
                    package->push_back(buffer);
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
    QList<QPair<int, QString>> changes;
    if (file.exists() && file.open(QIODevice::ReadOnly)) {
        QList<int> readedIndex;
        QXmlStreamReader xml(&file);
        while (!xml.atEnd()) {
            QXmlStreamReader::TokenType token = xml.readNext();
            if (xml.hasError()) {
                return false;
            }
            if (token == QXmlStreamReader::StartElement) {
                if (xml.name().toString() == "Asset") {
                    QPair<int, QString> change = assetPKLData(xml, package, readedIndex);
                    if (change.first == -1 && change.second.isEmpty()) return false;
                    changes.push_back(change);
                }
            }
        }
        for (int i = 0; i < changes.count(); ++i) {
            (*package)[changes[i].first]->inPkl = true;
            (*package)[changes[i].first]->hash = changes[i].second;
        }
        return true;
    }
    return false;
}

Asset XmlFileReaderModule::readAssetmapData(QXmlStreamReader &xml)
{
    Asset asset;
    while (!xml.atEnd()) {
        QXmlStreamReader::TokenType token = xml.readNext();
        if (xml.hasError()) return Asset();
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
            }
        } else if (token == QXmlStreamReader::EndElement && xml.name().toString() == "Asset") {
            return asset;
        }
    }
    return asset;
}

QPair<int, QString> XmlFileReaderModule::assetPKLData(QXmlStreamReader &xml, DCPPackage *package, QList<int> &readedIndex)
{
    QPair<int, QString> change(-1, "");
    while (!xml.atEnd()) {
        QXmlStreamReader::TokenType token = xml.readNext();
        if (xml.hasError()) return QPair<int, QString>(-1, "");
        if (token == QXmlStreamReader::StartElement) {
            QString name = xml.name().toString();
            if (name == "Id") {
                QString id = xml.readElementText().mid(9);
                for (int i = 0; i < package->size(); ++i) {
                    if (readedIndex.contains(i)) continue;
                    if ((*package)[i]->id == id) {
                        change.first = i;
                        readedIndex.push_back(i);
                    }
                }
            } else if (name == "Hash") {
                change.second = xml.readElementText();
            }
        } else if (token == QXmlStreamReader::EndElement && xml.name().toString() == "Asset") {
            return change;
        }
    }
    return change;
}
