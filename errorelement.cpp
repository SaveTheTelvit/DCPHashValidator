#include "errorelement.h"

ErrorElement::ErrorElement(QString path, QString error, VScrollBoxElement *parent)
    : filePath(path)
    , errorStr(error)
    , VScrollBoxElement(parent)
{
    QLabel *imgLabel = new QLabel;
    setHeight(70);
    imgLabel->resize(50, 50);
    QPixmap pixmap(":/status/error.png");
    imgLabel->setPixmap(pixmap.scaled(imgLabel->size(), Qt::KeepAspectRatio));
    addInterfaceWidget(imgLabel, 10, 10, VScrollBoxElement::StaticPolicy);
    QFont font("Cantarell", 7);
    QLabel *pathLabel = new QLabel;
    pathLabel->setText("Путь: " + filePath);
    pathLabel->resize(0, 10);
    pathLabel->setFont(font);
    addInterfaceWidget(pathLabel, 80, 15, VScrollBoxElement::ResizeToRightBorder);
    QLabel *errorLabel = new QLabel;
    errorLabel->setText("Ошибка: " + errorStr);
    errorLabel->resize(0, 10);
    errorLabel->setFont(font);
    addInterfaceWidget(errorLabel, 80, 45, VScrollBoxElement::ResizeToRightBorder);
}
