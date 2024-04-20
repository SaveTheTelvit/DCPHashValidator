#include "filehashinfo.h"

FileHashInfo::FileHashInfo(QString path, QString h1, QString h2, QWidget *parent)
    : filePath(path)
    , hash1(h1)
    , hash2(h2)
    , VScrollBoxElement(parent)
{
    QLabel *imageLabel = new QLabel;
    imageLabel->resize(50, 50);
    setHeight(70);
    if (hash1 == hash2) {
        QPixmap pixmap(":/status/check.png");
        imageLabel->setPixmap(pixmap.scaled(imageLabel->size(), Qt::KeepAspectRatio));
    } else {
        QPixmap pixmap(":/status/cross.png");
        imageLabel->setPixmap(pixmap.scaled(imageLabel->size(), Qt::KeepAspectRatio));
    }
    addInterfaceWidget(imageLabel, 10, 10, VScrollBoxElement::StaticPolicy);
    QFont font("Cantarell", 7);
    DynamicDeleteLabel *pathLabel = new DynamicDeleteLabel;
    pathLabel->setContent("Путь: " + filePath);
    pathLabel->setFont(font);
    pathLabel->setDeleteIndex(6);
    pathLabel->resize(pathLabel->sizeHint());
    addInterfaceWidget(imageLabel, 80, 10, VScrollBoxElement::ResizeToRightBorder);
    DynamicDeleteLabel *hash1Label = new DynamicDeleteLabel;
    hash1Label->setContent("Хэш-эталон: " + hash1);
    hash1Label->setFont(font);
    hash1Label->setDeleteIndex(12);
    hash1Label->resize(hash1Label->sizeHint());
    addInterfaceWidget(hash1Label, 80, 40, VScrollBoxElement::ResizeToRightBorder);
    DynamicDeleteLabel *hash2Label = new DynamicDeleteLabel;
    hash2Label->setContent("Хеш-значение: " + hash2);
    hash2Label->setFont(font);
    hash2Label->setDeleteIndex(14);
    hash2Label->setAlignment(Qt::AlignRight);
    hash2Label->resize(hash2Label->sizeHint());
    addInterfaceWidget(hash2Label, 80, 40, VScrollBoxElement::ResizeToRightBorder);

}
