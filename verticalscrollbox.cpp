#include "verticalscrollbox.h"

VerticalScrollBox::VerticalScrollBox(QWidget *parent)
    : QScrollArea(parent)
{
    contentWidget = new QWidget(this);
    contentWidget->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Expanding);
    content = new QVBoxLayout(contentWidget);
    content->setDirection(QBoxLayout::BottomToTop);
    content->setAlignment(Qt::AlignTop);
    content->setAlignment(Qt::AlignHCenter);
    content->setSpacing(10);
    content->setSizeConstraint(QLayout::SetMinimumSize);
    setWidget(contentWidget);
    setBackgroundRole(QPalette::Dark);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    viewport()->installEventFilter(this);
}

VerticalScrollBox::~VerticalScrollBox()
{
    delete content;
    delete contentWidget;
}

void VerticalScrollBox::addWidget(QWidget *w)
{
    content->addWidget(w);
    w->setMinimumSize(viewport()->width() - widthModificator, w->height());
    w->resize(viewport()->width() - widthModificator, w->height());
    w->setMaximumSize(viewport()->width() - widthModificator, w->height());
    if (widgetsHeight != 0) {
        widgetsHeight += w->height() + content->spacing();
    } else widgetsHeight = w->height();
}

void VerticalScrollBox::resizeAllElements(int w)
{
    contentWidget->resize(0, 0);
    if (w > lastWidgetWidth) {
        for (int i = 0; i < content->count(); ++i) {
            QWidget* wi = content->itemAt(i)->widget();
            if (wi != nullptr) {
                wi->setMinimumWidth(w);
                wi->resize(w, wi->height());
                wi->setMaximumWidth(w);
            }
        }
    } else if (w < lastWidgetWidth) {
        for (int i = 0; i < content->count(); ++i) {
            QWidget* wi = content->itemAt(i)->widget();
            if (wi != nullptr) {
                wi->setMaximumWidth(w);
                wi->resize(w, wi->height());
                wi->setMinimumWidth(w);
            }
        }
    }
    lastWidgetWidth = w;
}

bool VerticalScrollBox::eventFilter(QObject *obj, QEvent *event)
{
    if (event->type() == QEvent::Resize) {
        if (obj == viewport()) {
            QResizeEvent *e = static_cast<QResizeEvent*>(event);
            resizeAllElements(e->size().width() - widthModificator);
            contentWidget->adjustSize();
        }
    }
    return QObject::eventFilter(obj, event);
}

void VerticalScrollBox::deleteLast()
{
    int items = content->count();
    if (items > 0) {
        QWidget* w = content->itemAt(items - 1)->widget();
        if (w != nullptr) {
            content->removeWidget(w);
            delete w;
            contentWidget->adjustSize();
        }
    }
}

void VerticalScrollBox::clear()
{
    while (content->count() > 0) {
        deleteLast();
    }
}

int VerticalScrollBox::count() {
    return content->count();
}
