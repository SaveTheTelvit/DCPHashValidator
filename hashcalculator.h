#ifndef HASHCALCULATOR_H
#define HASHCALCULATOR_H

#include <QObject>
#include <QString>
#include <QCryptographicHash>
#include <QByteArray>
#include <QFile>
#include <QTimer>
#include <QDebug>

#include "structs.h"
#include "dcppackage.h"

class HashCalculator: public QObject
{
    Q_OBJECT
public:
    HashCalculator();
    void setAssetList(DCPPackage *pack);
    void calculateNext();
    void toBegin();

signals:
    void hashCalculated(int index, const QString &hash);
    void errorOccured(int index, const QString &error);
    void processingProcess(int percent);
    void endReached();

private:
    DCPPackage *package;
    int currentIndex = 0;
    qint64 processed = 0;
};

#endif // HASHCALCULATOR_H
