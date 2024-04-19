#include "xmlfilereadermodule.h"

QList<Asset>* XmlFileReaderModule::getFullAsset(const QString &path, int mode)
{
    QList<Asset> *assets = readAssetmap(path);
    for (int i = 0; i < assets->size(); ++i) {
        if ((*assets)[i].type == Asset::PackingList) {
            assetPKL((*assets)[i].path, assets);
            break;
        }
    }
    if (mode == WithVolindex) {
        Asset volAsset;
        volAsset.path = path.mid(0, path.lastIndexOf('/')) + "/VOLINDEX";
        volAsset.type = Asset::AdditionalFiles;
        assets->push_back(volAsset);
    };
    return assets;
}

QList<Asset>* XmlFileReaderModule::readAssetmap(const QString &path)
{
    QFile file(path);
    if (file.open(QIODevice::ReadOnly)) {
        QList<Asset> *assetList = new QList<Asset>;
        QXmlStreamReader xml(&file);
        while (!xml.atEnd() && !xml.hasError()) {
            QXmlStreamReader::TokenType token = xml.readNext();
            if (token == QXmlStreamReader::StartElement) {
                if (xml.name().toString() == "Asset") {
                    assetList->push_back(readAssetmapData(xml, path));
                }
            }
        }
        return assetList;
    }
    return nullptr;
}

QList<Asset>* XmlFileReaderModule::readPKL(const QString &path)
{
    QFile file(path);
    if (file.open(QIODevice::ReadOnly)) {
        QList<Asset> *assetList = new QList<Asset>;
        QXmlStreamReader xml(&file);
        while (!xml.atEnd() && !xml.hasError()) {
            QXmlStreamReader::TokenType token = xml.readNext();
            if (token == QXmlStreamReader::StartElement) {
                if (xml.name().toString() == "Asset") {
                    assetList->push_back(readPKLData(xml));
                }
            }
        }
        return assetList;
    }
    return nullptr;
}

QList<Asset>* XmlFileReaderModule::assetPKL(const QString &path, QList<Asset> *assetList)
{
    QFile file(path);
    if (file.exists() && file.open(QIODevice::ReadOnly)) {
        QList<int> readedIndex;
        QXmlStreamReader xml(&file);
        while (!xml.atEnd() && !xml.hasError()) {
            QXmlStreamReader::TokenType token = xml.readNext();
            if (token == QXmlStreamReader::StartElement) {
                if (xml.name().toString() == "Asset") {
                    assetPKLData(xml, assetList, readedIndex);
                }
            }
        }
        return assetList;
    }
    return assetList;
}

Asset XmlFileReaderModule::readAssetmapData(QXmlStreamReader &xml, const QString &path)
{
    Asset asset;
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
            asset.pklExist = true;
            return asset;
        }
    }
    return asset;
}

void XmlFileReaderModule::assetPKLData(QXmlStreamReader &xml, QList<Asset> *assetList, QList<int> &readedIndex)
{
    int index = -1;
    QString hashBuffer = "";
    while (!xml.atEnd() && !xml.hasError()) {
        QXmlStreamReader::TokenType token = xml.readNext();
        if (token == QXmlStreamReader::StartElement) {
            QString name = xml.name().toString();
            if (name == "Id") {
                QString id = xml.readElementText().mid(9);
                for (int i = 0; i < assetList->size(); ++i) {
                    if (readedIndex.contains(i)) continue;
                    if ((*assetList)[i].id == id) {
                        if (hashBuffer.isEmpty()) {
                            index = i;
                        } else {
                            (*assetList)[i].id = hashBuffer;
                        }
                        readedIndex.push_back(i);
                        break;
                    }
                }
            } else if (name == "Hash") {
                if (index != -1) {
                    (*assetList)[index].hash = xml.readElementText();
                } else hashBuffer = xml.readElementText();
            }
        } else if (token == QXmlStreamReader::EndElement && xml.name().toString() == "Asset") {
            if (index != -1) (*assetList)[index].pklExist = true;
            return;
        }
    }
}
