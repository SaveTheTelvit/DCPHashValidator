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

            break;
        case MovingToRightBorder:
        {
            wi->move(event->size().width() - wi->width() - policySpace, wi->y());
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
        }
            break;
        case ResizeWithAspectRatio:

            break;
        case ResizeToRightBorder:
        {
            wi->resize(event->size().width() - wi->x() - policySpace, wi->height());
        }
            break;
        default:

            break;
        }
    }
}
