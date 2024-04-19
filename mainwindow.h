#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileDialog>
#include <QString>
#include <QCryptographicHash>
#include <QByteArray>
#include <QFileSystemModel>
#include <QSortFilterProxyModel>
#include <QDebug>
#include <QEvent>
#include <QResizeEvent>
#include <QThread>

#include "verticalscrollbox.h"
#include "filehashinfo.h"
#include "vscrollboxelement.h"
#include "hashcalculatorelement.h"
#include "hashcalculator.h"
#include "errorelement.h"
#include "structs.h"
#include "xmlfilereadermodule.h"

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

signals:
    void calculateNext();

private:
    VerticalScrollBox *scrollBox;
    QString currentPath;
    Ui::MainWindow *ui;
    int cou = 0;
    int lastViewportWidth = 0 , items = 0;
    void calculateHashes(QList<Asset> *assets);
};
#endif // MAINWINDOW_H
