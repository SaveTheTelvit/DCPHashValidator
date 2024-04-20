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
#include <QCloseEvent>
#include <QMessageBox>

#include "verticalscrollbox.h"
#include "filehashinfo.h"
#include "vscrollboxelement.h"
#include "hashcalculatorelement.h"
#include "hashcalculator.h"
#include "errorelement.h"
#include "structs.h"
#include "xmlfilereadermodule.h"
#include "connectioncontroller.h"

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
    void on_lineEdit_returnPressed();

signals:
    void calculateNext();

private:
    VerticalScrollBox *scrollBox;
    ConnectionController *controller;
    QString currentPath;
    Ui::MainWindow *ui;
    int cou = 0;
    bool processed = false;
    int lastViewportWidth = 0 , items = 0;
    void calculateHashes(QList<Asset> *assets);
protected:
    void closeEvent(QCloseEvent *event) override;
};
#endif // MAINWINDOW_H
