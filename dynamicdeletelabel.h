#ifndef DYNAMICDELETELABEL_H
#define DYNAMICDELETELABEL_H

#include <QLabel>
#include <QString>
#include <QResizeEvent>
#include <QFont>
#include <QFontMetrics>

class DynamicDeleteLabel : public QLabel
{
    Q_OBJECT
public:
    DynamicDeleteLabel(QWidget *parent = nullptr);
    void setContent(const QString &str);
    void setDeleteIndex(int i);

private:
    void textOnWidth();
    void textOnWidth(int w);
    QString content;
    int deleteIndex = 0;

protected:
    void resizeEvent(QResizeEvent *event) override;
};

#endif // DYNAMICDELETELABEL_H
