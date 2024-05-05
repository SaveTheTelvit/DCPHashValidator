#ifndef XMLFILEREADERMODULE_H
#define XMLFILEREADERMODULE_H

#include <QString>
#include <QObject>
#include <QXmlStreamReader>
#include <QFile>
#include <QDebug>

#include "structs.h"
#include "dcppackage.h"

class XmlFileReaderModule
{
public:
    enum {
        NoAdditional = 0,
        WithVolindex = 1
    };
    static DCPPackage* getFullAsset(const QString& path, int mode = NoAdditional);
    static DCPPackage* readAssetmap(const QString& path);
    static bool assetPKL(const QString& path, DCPPackage *package);

private:
    static Asset readAssetmapData(QXmlStreamReader &xml);
    static QPair<int, QString> assetPKLData(QXmlStreamReader &xml, DCPPackage *package, QList<int> &readedIndex);
};

#endif // XMLFILEREADERMODULE_H
