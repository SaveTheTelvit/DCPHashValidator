#include "hashcalculator.h"

HashCalculator::HashCalculator() {}

void HashCalculator::setAssetList(QList<Asset> *assetList)
{
    assets = assetList;
    toBegin();
}

void HashCalculator::calculateNext()
{
    if (currentIndex < assets->count()) {
        if (!((*assets)[currentIndex].type == Asset::AdditionalFiles || (*assets)[currentIndex].type == Asset::PackingList)) {
            QFile file((*assets)[currentIndex].path);
            if (!file.open(QIODevice::ReadOnly)) {
                emit errorOccured(currentIndex++, "Файл не найден");
                return;
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
            processed = 0;
            emit hashCalculated(currentIndex++, QString(hash.result().toBase64()));
        } else {
            ++currentIndex;
            calculateNext();
        }
    } else emit endReached();
}

void HashCalculator::toBegin()
{
    currentIndex = 0;
}
