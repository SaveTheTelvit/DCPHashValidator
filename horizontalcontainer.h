#ifndef HORIZONTALCONTAINER_H
#define HORIZONTALCONTAINER_H

#include <QWidget>
#include <QLabel>
#include <QResizeEvent>
#include <QDebug>

class HorizontalContainer: public QWidget
{
    Q_OBJECT
public:
    HorizontalContainer(QWidget* parent = nullptr);
    void addLabel(QLabel* label);

private:
    QList<QLabel*> labels;

protected:
    void resizeEvent(QResizeEvent *event);
};

#endif // HORIZONTALCONTAINER_H
