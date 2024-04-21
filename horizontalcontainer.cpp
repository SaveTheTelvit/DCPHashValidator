#include "horizontalcontainer.h"

HorizontalContainer::HorizontalContainer(QWidget *parent)
    : QWidget(parent)
{
    setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Expanding);
}

void HorizontalContainer::addLabel(QLabel *label)
{
    label->setParent(this);
    labels.push_back(label);
}

void HorizontalContainer::resizeEvent(QResizeEvent *event)
{
    QWidget::resizeEvent(event);
    if (labels.count() > 0) {
        int widthOnLabel = event->size().width() / labels.count() + ((event->size().width() % labels.count() > labels.count() / 2) ? 1 : 0);
        for (int i = 0; i < labels.count(); ++i) {
            QLabel* label = labels[i];
            if (label != nullptr) {
                if (i != labels.count() - 1) {
                    label->resize(widthOnLabel, label->height());
                } else {
                    label->resize(event->size().width() - i * widthOnLabel, label->height());
                }
                label->move(i * widthOnLabel, label->y());
            }
        }
    }
}
