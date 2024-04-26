#include "iconbutton.h"

IconButton::IconButton(QWidget *parent)
    :QPushButton(parent)
{
    opacity = new QGraphicsOpacityEffect(this);
    opacity->setOpacity(0.5);
    setGraphicsEffect(opacity);
    setFlat(true);
    setStyleSheet("QPushButton {border: none; outline: none;}");
}

IconButton::~IconButton()
{
    delete opacity;
}

void IconButton::mousePressEvent(QMouseEvent *event)
{
    QPushButton::mousePressEvent(event);
    opacity->setOpacity(0.7);
}

void IconButton::mouseReleaseEvent(QMouseEvent *event)
{
    QPushButton::mouseReleaseEvent(event);
    if (inside) {
        opacity->setOpacity(0.3);
    } else {
        opacity->setOpacity(0.5);
    }
}

#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
    void IconButton::enterEvent(QEnterEvent *event)
#else
    void IconButton::enterEvent(QEvent *event)
#endif
{
    QPushButton::enterEvent(event);
    opacity->setOpacity(0.3);
    inside = true;
}

void IconButton::leaveEvent(QEvent *event)
{
    QPushButton::leaveEvent(event);
    opacity->setOpacity(0.5);
    inside = false;
}
