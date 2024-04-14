#ifndef FILEHASHINFO_H
#define FILEHASHINFO_H

#include <QRandomGenerator>
#include <QWidget>
#include <QPaintEvent>
#include <QPainter>
#include <QPixmap>
#include <QFontMetrics>

namespace Ui {
class FileHashInfo;
}

class FileHashInfo : public QWidget
{
    Q_OBJECT

public:
    explicit FileHashInfo(QString path, QString h1, QString h2 = "", QWidget *parent = nullptr);
    ~FileHashInfo();
    void pathResize(int w);

private:
    QString filePath;
    QString hash1;
    QString hash2;
    Ui::FileHashInfo *ui;

protected:
    void paintEvent(QPaintEvent *event) override {
        Q_UNUSED(event);

        QPainter painter(this);
        painter.setPen(QPen(Qt::black, 2)); // Устанавливаем черную кисть шириной 2 пикселя
        painter.fillRect(rect(), Qt::white);
        int width = this->width();
        int height = this->height();

        // Рисуем линию по краю виджета
        painter.drawLine(0, 0, width, 0); // Верхний край
        painter.drawLine(width, 0, width, height); // Правый край
        painter.drawLine(width, height, 0, height); // Нижний край
        painter.drawLine(0, height, 0, 0); // Левый край
    }
};

#endif // FILEHASHINFO_H
