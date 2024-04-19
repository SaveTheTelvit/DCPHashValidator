#ifndef XMLFILEREADERMODULE_H
#define XMLFILEREADERMODULE_H

#include <QString>
#include <QObject>
#include <QXmlStreamReader>
#include <QFile>
#include <QDebug>

#include "structs.h"

class XmlFileReaderModule
{
public:
    enum {
        NoAdditional = 0,
        WithVolindex = 1
    };
    static QList<Asset>* getFullAsset(const QString& path, int mode = NoAdditional);
    static QList<Asset>* readAssetmap(const QString& path);
    static QList<Asset>* readPKL(const QString& path);
    static QList<Asset>* assetPKL(const QString& path, QList<Asset> *assetList);

private:
    static Asset readAssetmapData(QXmlStreamReader &xml, const QString& path);
    static Asset readPKLData(QXmlStreamReader &xml);
    static void assetPKLData(QXmlStreamReader &xml, QList<Asset> *assetList, QList<int> &readedIndex);
};

#endif // XMLFILEREADERMODULE_H
