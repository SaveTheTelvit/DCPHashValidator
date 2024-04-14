#include "vscrollboxelement.h"

VScrollBoxElement::VScrollBoxElement(QWidget *parent)
    : QWidget(parent)
{
    setBackgroundRole(QPalette::Light);
}

VScrollBoxElement::VScrollBoxElement(int h, QWidget *parent)
    : QWidget(parent)
{
    setHeight(h);
    setBackgroundRole(QPalette::Light);
}

void VScrollBoxElement::addInterfaceWidget(QWidget *widget, int x, int y, int horizontalPolicy)
{
    widget->setParent(this);
    widget->move(x, y);
    interfaceWidgets.push_back(InterfaceWidgetInfo(widget, horizontalPolicy));
}

void VScrollBoxElement::setHeight(int h)
{
    setMinimumHeight(h);
    setMaximumHeight(h);
}

void VScrollBoxElement::setPolicySpace(int space)
{
    if (space >= 0) {
        policySpace = space;
    }
}

void VScrollBoxElement::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    QPainter painter(this);
    painter.setPen(QPen(Qt::black, 2));
    painter.fillRect(rect(), Qt::white);
    int width = this->width();
    int height = this->height();
    painter.drawLine(0, 0, width, 0);
    painter.drawLine(width, 0, width, height);
    painter.drawLine(width, height, 0, height);
    painter.drawLine(0, height, 0, 0);
}

void VScrollBoxElement::resizeEvent(QResizeEvent *event)
{
    QWidget::resizeEvent(event);
    for (int i = 0; i < interfaceWidgets.count(); ++i) {
        QWidget* wi = interfaceWidgets[i].widget;
        switch (interfaceWidgets[i].horizontalPolicy) {
        case MovingPolicy:
        {
            if (wi->x() + wi->width() + policySpace >= event->size().width()) {
                int newXPos = event->size().width() - wi->width() - policySpace;
                if (newXPos < policySpace) newXPos = policySpace;
                wi->move(newXPos, wi->y());
            } else if (interfaceWidgets[i].desiredPoint.x() > wi->x()) {
                if (interfaceWidgets[i].desiredPoint.x() > event->size().width() - policySpace) {
                    int newXPos = event->size().width() - wi->width() - policySpace;
                    if (newXPos < policySpace) newXPos = policySpace;
                    wi->move(newXPos, wi->y());
                } else {
                    wi->move(interfaceWidgets[i].desiredPoint);
                }
            }
            break;
        }
        case MovingToRightBorder:
        {
            int newXPos = event->size().width() - wi->width() - policySpace;
            if (newXPos < policySpace) newXPos = policySpace;
            wi->move(newXPos, wi->y());
            break;
        }
        case ResizePolicy:
        {
            int lastX = wi->x() + wi->width();
            if (lastX > event->size().width() - policySpace) {
                wi->resize(event->size().width() - wi->x() - policySpace, wi->height());
            } else if (lastX < event->size().width()) {
                if (interfaceWidgets[i].desiredSize.width() + wi->x() > event->size().width() - policySpace) {
                    wi->resize(event->size().width() - wi->x() - policySpace, wi->height());
                } else wi->resize(interfaceWidgets[i].desiredSize.width(), wi->height());
            }
            break;
        }
        case ResizeWithAspectRatio:
        {
            int lastX = wi->x() + wi->width();
            if (lastX > event->size().width() - policySpace) {
                float ratio = wi->width() / wi->height();
            }
            break;
        }
        case ResizeToRightBorder:
        {
            wi->resize(event->size().width() - wi->x() - policySpace, wi->height());
            break;
        }
        default:
            break;
        }
    }
}
