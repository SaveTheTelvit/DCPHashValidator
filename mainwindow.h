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
#include <QIcon>
#include <QDateTime>
#include <QListWidgetItem>

#include "verticalscrollbox.h"
#include "filehashinfo.h"
#include "vscrollboxelement.h"
#include "hashcalculatorelement.h"
#include "hashcalculator.h"
#include "errorelement.h"
#include "structs.h"
#include "xmlfilereadermodule.h"
#include "connectioncontroller.h"
#include "settings.h"
#include "dcppackage.h"

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
    void on_settingsButton_clicked();

signals:
    void calculateNext();

private:
    VerticalScrollBox *scrollBox;
    ConnectionController *controller;
    Settings *settings;
    QString currentPath;
    QThread *hashThread = nullptr;
    QThread::Priority priority = QThread::NormalPriority;
    Ui::MainWindow *ui;
    int cou = 0;
    bool processed = false;
    int lastViewportWidth = 0 , items = 0;
    QVector<int> speeds;
    QDateTime timeStamp;
    void calculateHashes(DCPPackage *package);
    void setVerison(QString version);
    void setupOnChanges(QList<QPair<int, bool>> changes);
    void enableDevTools(bool enable = true);

protected:
    void closeEvent(QCloseEvent *event) override;
};
#endif // MAINWINDOW_H
