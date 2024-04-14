#ifndef VSCROLLBOXELEMENT_H
#define VSCROLLBOXELEMENT_H

#include <QWidget>
#include <QSize>
#include <QRect>
#include <QPaintEvent>
#include <QPainter>
#include <QResizeEvent>

class VScrollBoxElement : public QWidget
{
    Q_OBJECT
public:
    enum {
        MovingPolicy = 0,
        MovingToRightBorder = 1,
        ResizePolicy = 2,
        ResizeWithAspectRatio = 3,
        ResizeToRightBorder = 4,
        StaticPolicy = 5

    };
    VScrollBoxElement(QWidget *parent = nullptr);
    VScrollBoxElement(int h, QWidget *parent = nullptr);
    void addInterfaceWidget(QWidget *widget, int x, int y, int horizontalPolicy);
    void setHeight(int h);
    void setPolicySpace(int space);

private:
    struct InterfaceWidgetInfo
    {
        InterfaceWidgetInfo(QWidget* w, int policy) : widget(w), horizontalPolicy(policy) {desiredSize = w->size(); desiredPoint = w->pos();}
        QWidget* widget;
        int horizontalPolicy = StaticPolicy;
        QSize desiredSize;
        QPoint desiredPoint;
    };
    QList<InterfaceWidgetInfo> interfaceWidgets;
    int policySpace = 0;

protected:
    void paintEvent(QPaintEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;
};

#endif // VSCROLLBOXELEMENT_H
