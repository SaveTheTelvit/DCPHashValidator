#include "hashcalculator.h"

HashCalculator::HashCalculator() {}

void HashCalculator::setPath(QString str)
{
    path = str;
}

void HashCalculator::calculate()
{
    QFile file(path);
    if (!file.open(QIODevice::ReadOnly)) {
        emit errorOccured("Файл не найден");
    }
    const int bufferSize = 4096;
    qint64 fileSize = file.size() / 1024;
    qint64 buffers = fileSize / 4 + 2;
    QByteArray buffer;
    QCryptographicHash hash(QCryptographicHash::Sha1);
    while(!file.atEnd()) {
        if (processed % 1000 == 0) emit processingProcess (static_cast<int>((processed * 1000) / buffers));
        buffer = file.read(bufferSize);
        hash.addData(buffer);
        ++processed;
    }
    file.close();
    emit hashCalculated(QString(hash.result().toBase64()));
}
