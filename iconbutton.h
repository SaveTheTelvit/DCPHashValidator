#ifndef ICONBUTTON_H
#define ICONBUTTON_H

#include <QPushButton>
#include <QMouseEvent>
#include <QGraphicsOpacityEffect>

class IconButton: public QPushButton
{
    Q_OBJECT
public:
    IconButton(QWidget *parent = nullptr);
    ~IconButton();

private:
    QGraphicsOpacityEffect *opacity;
    bool inside = false;

protected:
    void mousePressEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;

#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
    void enterEvent(QEnterEvent *event) override;
#else
    void enterEvent(QEvent *event) override;
#endif

    void leaveEvent(QEvent *event) override;
};

#endif // ICONBUTTON_H
