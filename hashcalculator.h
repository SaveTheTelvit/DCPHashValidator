#ifndef HASHCALCULATOR_H
#define HASHCALCULATOR_H

#include <QObject>
#include <QString>
#include <QCryptographicHash>
#include <QByteArray>
#include <QFile>
#include <QTimer>
#include <QDebug>

class HashCalculator: public QObject
{
    Q_OBJECT
public:
    HashCalculator();
    void setPath(QString str);
    void calculate();

signals:
    void hashCalculated(const QString& hash);
    void errorOccured(const QString &filePath, const QString& error);
    void processingProcess(int percent);

private:
    QString path;
    qint64 processed = 0;
};

#endif // HASHCALCULATOR_H
