#ifndef ERRORELEMENT_H
#define ERRORELEMENT_H

#include <QWidget>
#include <QLabel>
#include <QPixmap>
#include <QFont>

#include "vscrollboxelement.h"

class ErrorElement : public VScrollBoxElement
{
    Q_OBJECT
public:
    ErrorElement(QString path, QString error, VScrollBoxElement *parent = nullptr);

private:
    QString filePath , errorStr;
};

#endif // ERRORELEMENT_H
