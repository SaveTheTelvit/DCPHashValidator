#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileDialog>
#include <QString>
#include <QCryptographicHash>
#include <QByteArray>
#include <QFileSystemModel>
#include <QSortFilterProxyModel>
#include <QRegularExpression>
#include <QXmlStreamReader>
#include <QDebug>
#include <QEvent>
#include <QResizeEvent>
#include <QThread>
#include <QMutex>

#include "verticalscrollbox.h"
#include "filehashinfo.h"
#include "vscrollboxelement.h"
#include "hashcalculatorelement.h"
#include "hashcalculator.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_pushButton_clicked();
    void on_pushButton_2_clicked();
    void on_pushButton_3_clicked();

private:
    VerticalScrollBox *scrollBox;
    QString currentPath;
    Ui::MainWindow *ui;
    int cou = 0;
    int lastViewportWidth = 0 , items = 0;
    QList<QPair<QString, QString>> assets;
    void calculteHash(const QString& filePath, const QString& hash);
    bool readPKL(const QString& filePath);
    QPair<QString, QString> readAsset(QXmlStreamReader &xml);
};
#endif // MAINWINDOW_H
