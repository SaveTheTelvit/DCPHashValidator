#include "filehashinfo.h"
#include "ui_filehashinfo.h"

FileHashInfo::FileHashInfo(QString path, QString h1, QString h2, QWidget *parent)
    : filePath(path)
    , hash1(h1)
    , hash2(h2)
    , QWidget(parent)
    , ui(new Ui::FileHashInfo)
{
    ui->setupUi(this);
    setBackgroundRole(QPalette::Light);
    if (hash1 == hash2) {
        QPixmap pixmap(":/status/check.png");
        ui->imageLabel->setPixmap(pixmap.scaled(ui->imageLabel->size(), Qt::KeepAspectRatio));
    } else {
        QPixmap pixmap(":/status/cross.png");
        ui->imageLabel->setPixmap(pixmap.scaled(ui->imageLabel->size(), Qt::KeepAspectRatio));
    }
    pathResize(width());
    ui->hash1Label->setText("Хэш-эталон: " + hash1);
    ui->hash2Label->setText("Хеш-значение: " + hash2);
}

FileHashInfo::~FileHashInfo()
{
    delete ui;
}

void FileHashInfo::pathResize(int w)
{
    int diff = w - width();
    if (diff != 0) {
        ui->pathLabel->resize(ui->pathLabel->width() + diff, ui->pathLabel->height());
    }
    int maxLength = ui->pathLabel->width();
    QFontMetrics fontM(ui->pathLabel->font());
    QString str = "Путь: " + filePath;
    if (fontM.boundingRect(str).width() > maxLength) {
        int textWidth = fontM.boundingRect(str).width();
        int charsToRemove = str.size() * (textWidth - maxLength) / textWidth;
        str = str.right(str.size() - charsToRemove - 9);
        str.prepend("Путь: ...");
    }
    ui->pathLabel->setText(str);
}
