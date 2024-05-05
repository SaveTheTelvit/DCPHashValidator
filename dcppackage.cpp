#include "dcppackage.h"

#include <QDebug>

DCPPackage::DCPPackage(QObject *parent)
    : QObject(parent)
{

}

DCPPackage::~DCPPackage()
{

}

Asset* DCPPackage::operator[](int i)
{
    return &assets[i];
}

Asset* DCPPackage::PKL()
{
    if (pklPos == -1) return nullptr;
    return &assets[pklPos];
}

Asset* DCPPackage::CPL()
{
    if (cplPos == -1) return nullptr;
    return &assets[cplPos];
}

QString& DCPPackage::path()
{
    return packagePath;
}

QString DCPPackage::pathOfAsset(int index)
{
    return packagePath + '/' + assets[index].path;
}

QString DCPPackage::pathOfAsset(Asset* asset)
{
    return packagePath + '/' + asset->path;
}

void DCPPackage::push_back(Asset asset)
{
    assets.push_back(asset);
    if (asset.type == Asset::PackingList) {
        pklPos = assets.size() - 1;
        QFile file(packagePath + '/' + asset.path);
        if (file.exists() && file.open(QIODevice::ReadOnly)) {
            pklExist = true;
            file.close();
        }
    } else if (asset.type == Asset::CompositionPlaylist) {
        cplPos = assets.size() - 1;
        QFile file(packagePath + '/' + asset.path);
        if (file.exists() && file.open(QIODevice::ReadOnly)) {
            pklExist = true;
            file.close();
        }
    }
}

void DCPPackage::setPKLDamagedState(bool state)
{
    pklDamaged = state;
}

void DCPPackage::setPath(QString str)
{
    packagePath = str;
}

int DCPPackage::size()
{
    return assets.size();
}

bool DCPPackage::isPKLExist()
{
    return pklExist;
}

bool DCPPackage::isCPLExist()
{
    return cplExist;
}

bool DCPPackage::isPKLDamaged()
{
    return pklDamaged;
}
