#include "errorelement.h"

ErrorElement::ErrorElement(QString path, QString error, VScrollBoxElement *parent)
    : filePath(path)
    , errorStr(error)
    , VScrollBoxElement(parent)
{
    QLabel *imgLabel = new QLabel;
    setHeight(70);
    setPolicySpace(10);
    imgLabel->resize(50, 50);
    QPixmap pixmap(":/status/error.png");
    imgLabel->setPixmap(pixmap.scaled(imgLabel->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
    addInterfaceWidget(imgLabel, 10, 10, VScrollBoxElement::StaticPolicy);
    QFont font("Cantarell", 7);
    DynamicDeleteLabel *pathLabel = new DynamicDeleteLabel;
    pathLabel->setDeleteIndex(6);
    pathLabel->setContent("Путь: " + filePath);
    pathLabel->resize(pathLabel->sizeHint());
    pathLabel->setFont(font);
    addInterfaceWidget(pathLabel, 80, 15, VScrollBoxElement::ResizeToRightBorder);
    DynamicDeleteLabel *errorLabel = new DynamicDeleteLabel;
    errorLabel->setDeleteIndex(6);
    errorLabel->setContent("Ошибка: " + errorStr);
    errorLabel->resize(errorLabel->sizeHint());
    errorLabel->setFont(font);
    addInterfaceWidget(errorLabel, 80, 40, VScrollBoxElement::ResizeToRightBorder);
}
