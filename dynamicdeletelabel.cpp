#include "dynamicdeletelabel.h"

DynamicDeleteLabel::DynamicDeleteLabel(QWidget *parent)
    :QLabel(parent)
{
}

void DynamicDeleteLabel::setContent(const QString &str)
{
    content = str;
    textOnWidth();
}

void DynamicDeleteLabel::setDeleteIndex(int i)
{
    if (i >= 0) deleteIndex = i;
    textOnWidth();
}

void DynamicDeleteLabel::textOnWidth()
{
    textOnWidth(width());
}

void DynamicDeleteLabel::textOnWidth(int w)
{
    QFontMetrics metrics(font());
    int textWidth = metrics.boundingRect(content).width();
    if (textWidth > w) {
        QString shortedText = content.insert(deleteIndex, "...");
        while (metrics.boundingRect(shortedText).width() > w) {
            shortedText.remove(deleteIndex + 3, 1);
        }
        setText(shortedText);
        return;
    }
    setText(content);
}

void DynamicDeleteLabel::resizeEvent(QResizeEvent *event)
{
    QLabel::resizeEvent(event);
    textOnWidth(event->size().width());
}
