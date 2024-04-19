#ifndef VERTICALSCROLLBOX_H
#define VERTICALSCROLLBOX_H

#include <QScrollArea>
#include <QVBoxLayout>
#include <QWidget>
#include <QLabel>
#include <QSize>
#include <QScrollBar>
#include <QResizeEvent>
#include <QDebug>

class VerticalScrollBox : public QScrollArea
{
    Q_OBJECT
public:
    VerticalScrollBox(QWidget *parent = nullptr);
    ~VerticalScrollBox();
    void addWidget(QWidget *w);
    void deleteLast();
    void clear();
    //void deleteWidget();

private:
    QVBoxLayout *content;
    QWidget *contentWidget;
    qint16 elements = 0;
    qint16 widthModificator = 18;
    int widgetsHeight = 0;
    int widgetWidth = 0, lastWidgetWidth = -1;
    void resizeAllElements(int w);

protected:
    bool eventFilter(QObject *obj, QEvent *event) override;
};

#endif // VERTICALSCROLLBOX_H
