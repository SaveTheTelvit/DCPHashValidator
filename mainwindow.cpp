#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowTitle("Провека целостности DCP");
    scrollBox = new VerticalScrollBox(this);
    scrollBox->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    ui->gridLayout->addWidget(scrollBox, 0, 0);
    ui->gridLayout->addWidget(ui->widget, 0, 1);
}

void MainWindow::calculateHashes(QList<Asset> *assets)
{
    QThread *thread = new QThread;
    HashCalculator *hasher = new HashCalculator;
    hasher->setAssetList(assets);
    hasher->moveToThread(thread);
    QMetaObject::Connection *progressConnection = new QMetaObject::Connection;
    connect(thread, &QThread::started, this, [=](){
        HashCalculatorElement *progress = new HashCalculatorElement;
        scrollBox->addWidget(progress);
        *progressConnection = connect(hasher, &HashCalculator::processingProcess, progress, &HashCalculatorElement::setValue);
        emit calculateNext();
    });
    connect(hasher, &HashCalculator::endReached, this, [=](){
        disconnect(*progressConnection);
        scrollBox->deleteLast();
        delete progressConnection;
    });
    connect(hasher, &HashCalculator::errorOccured, this, [=](int index, const QString error) {
        disconnect(*progressConnection);
        scrollBox->deleteLast();
        scrollBox->addWidget(new ErrorElement((*assets)[index].path, error));
        HashCalculatorElement *progress = new HashCalculatorElement;
        scrollBox->addWidget(progress);
        *progressConnection = connect(hasher, &HashCalculator::processingProcess, progress, &HashCalculatorElement::setValue);
        emit calculateNext();
    });
    connect(hasher, &HashCalculator::hashCalculated, this, [=](int index, const QString calculatedHash){
        disconnect(*progressConnection);
        scrollBox->deleteLast();
        scrollBox->addWidget(new FileHashInfo((*assets)[index].path, (*assets)[index].hash, calculatedHash));
        HashCalculatorElement *progress = new HashCalculatorElement;
        scrollBox->addWidget(progress);
        *progressConnection = connect(hasher, &HashCalculator::processingProcess, progress, &HashCalculatorElement::setValue);
        emit calculateNext();
    });
    connect(this, &MainWindow::calculateNext, hasher, &HashCalculator::calculateNext);
    thread->start();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
    QFileDialog dialog;
    dialog.setFilter(QDir::NoDotAndDotDot | QDir::Files);
    dialog.setFileMode(QFileDialog::ExistingFile);
    dialog.setNameFilter("ASSETMAP (ASSETMAP)");
    dialog.setWindowTitle("Выберете ASSETMAP файл DCP пакета");
    if (dialog.exec()) {
        currentPath = dialog.directory().path();
        QString filePath = dialog.selectedFiles().at(0);
        if (filePath != "") {
            scrollBox->clear();
            calculateHashes(XmlFileReaderModule::getFullAsset(filePath));
            // QList<Asset> *assets = XmlFileReaderModule::getFullAsset(filePath, XmlFileReaderModule::WithVolindex);
            // for (int i = 0; i < assets->count(); ++i) {
            //     Asset a = (*assets)[i];
            //     qCritical() << a.hash + ' ' + a.id + ' ' + a.path + ' ' + (a.pklExist ? "true" : "false");
            //}
        }
    }
}

void MainWindow::on_pushButton_2_clicked()
{
    if (ui->lineEdit->text() == "key") {
        scrollBox->addWidget(new VScrollBoxElement(70));
    } else if (ui->lineEdit->text() == "progress") {
        scrollBox->addWidget(new HashCalculatorElement);
    } else {
        QString path = "/home/uservbox/dev/programs/build-DCPHashValidator-Desktop-Debug/DCPHashValidator/DCPHashValidator.exe";
        scrollBox->addWidget(new FileHashInfo(path, "qw" + QString::number(items++)));
    }
}

void MainWindow::on_pushButton_3_clicked()
{
    scrollBox->clear();
}

