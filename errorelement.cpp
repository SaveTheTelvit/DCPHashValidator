#include "errorelement.h"

ErrorElement::ErrorElement(QString path, QString error, VScrollBoxElement *parent)
    : filePath(path)
    , errorStr(error)
    , VScrollBoxElement(parent)
{
    QLabel *imgLabel = new QLabel;
    //setHeight(200);
    imgLabel->resize(50, 50);
    QPixmap pixmap(":/status/error.png");
    imgLabel->setPixmap(pixmap.scaled(imgLabel->size(), Qt::KeepAspectRatio));
    addInterfaceWidget(imgLabel, 10, 10, VScrollBoxElement::StaticPolicy);
    QLabel *pathLabel = new QLabel;
    pathLabel->setText(filePath);
    addInterfaceWidget(pathLabel, 70, 20, VScrollBoxElement::ResizeToRightBorder);
    QLabel *errorLabel = new QLabel;
    errorLabel->setText("Ошибка: " + errorStr);
    addInterfaceWidget(errorLabel, 70, 50, VScrollBoxElement::ResizeToRightBorder);
}
