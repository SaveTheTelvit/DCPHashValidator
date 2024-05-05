#ifndef DCPPACKAGE_H
#define DCPPACKAGE_H

#include <QString>
#include <QObject>
#include <QList>
#include <QFile>
#include <QFileSystemWatcher>

#include "structs.h"

class DCPPackage : public QObject
{
    Q_OBJECT
public:
    DCPPackage(QObject *parent = nullptr);
    ~DCPPackage();
    Asset* operator[](int i);
    Asset* PKL();
    Asset* CPL();
    QString& path();
    QString pathOfAsset(int index);
    QString pathOfAsset(Asset* asset);
    void push_back(Asset asset);
    void setPath(QString str);
    void setPKLDamagedState(bool state);
    int size();
    bool isPKLExist();
    bool isCPLExist();
    bool isPKLDamaged();

private:
    QList<Asset> assets;
    QString packagePath;
    int pklPos = -1;
    int cplPos = -1;
    bool pklDamaged = false;
    bool pklExist = false;
    bool cplExist = false;
};

#endif // DCPPACKAGE_H
