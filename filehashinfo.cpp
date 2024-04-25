#include "filehashinfo.h"

FileHashInfo::FileHashInfo(QString path, QString h1, QString h2, QWidget *parent)
    : filePath(path)
    , hash1(h1)
    , hash2(h2)
    , VScrollBoxElement(parent)
{
    QLabel *imageLabel = new QLabel;
    setHeight(70);
    imageLabel->resize(50, 50);
    setPolicySpace(10);
    if (hash1 == hash2) {
        QPixmap pixmap(":/status/check.png");
        imageLabel->setPixmap(pixmap.scaled(imageLabel->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
    } else {
        QPixmap pixmap(":/status/cross.png");
        imageLabel->setPixmap(pixmap.scaled(imageLabel->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
    }
    addInterfaceWidget(imageLabel, 10, 10, VScrollBoxElement::StaticPolicy);
    QFont font("Cantarell", 7);
    DynamicDeleteLabel *pathLabel = new DynamicDeleteLabel;
    pathLabel->setDeleteIndex(6);
    pathLabel->setContent("Путь: " + filePath);
    pathLabel->resize(pathLabel->sizeHint());
    pathLabel->setFont(font);
    addInterfaceWidget(pathLabel, 80, 15, VScrollBoxElement::ResizeToRightBorder);
    HorizontalContainer *container = new HorizontalContainer;
    DynamicDeleteLabel *hash1Label = new DynamicDeleteLabel;
    hash1Label->setContent("Хеш-эталон: " + hash1);
    hash1Label->setFont(font);
    hash1Label->setDeleteIndex(12);
    hash1Label->resize(hash1Label->sizeHint());
    container->addLabel(hash1Label);
    DynamicDeleteLabel *hash2Label = new DynamicDeleteLabel;
    hash2Label->setContent("Хеш-значение: " + hash2);
    hash2Label->setFont(font);
    hash2Label->setDeleteIndex(14);
    hash2Label->resize(hash2Label->sizeHint());
    container->addLabel(hash2Label);
    addInterfaceWidget(container, 80, 40, VScrollBoxElement::ResizeToRightBorder);
}
