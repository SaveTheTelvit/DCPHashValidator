#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setVerison("1.0.0");
    setWindowTitle("Провека целостности DCP");
    scrollBox = new VerticalScrollBox(this);
    scrollBox->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    ui->gridLayout->addWidget(scrollBox, 0, 0);
    ui->gridLayout->addWidget(ui->widget, 0, 1);
    controller = new ConnectionController(this);
    ui->pushButton_2->setVisible(false);
    ui->pushButton_3->setVisible(false);
    ui->lineEdit->setStyleSheet("QLineEdit {selection-background-color: transparent; "
                                "background-color: transparent; "
                                "border: none; "
                                "selection-color: rgba(0,0,0,0); "
                                "color: rgba(0,0,0,0);}"
                                );
}

void MainWindow::calculateHashes(QList<Asset> *assets)
{
    QThread *thread = new QThread;
    HashCalculator *hasher = new HashCalculator;
    hasher->setAssetList(assets);
    hasher->moveToThread(thread);
    controller->createConnection(thread, &QThread::started, this, [=](){
        HashCalculatorElement *progress = new HashCalculatorElement;
        scrollBox->addWidget(progress);
        controller->createConnection("progress", hasher, &HashCalculator::processingProcess, progress, &HashCalculatorElement::setValue);
        emit calculateNext();
    });
    controller->createConnection(hasher, &HashCalculator::endReached, this, [=](){
        controller->disconnectOnName("progress");
        scrollBox->deleteLast();
        controller->disconnectOnObject(this); // удаление всех соединений в которых завязан this
        ui->pushButton->setEnabled(true);
        thread->quit();
        thread->deleteLater();
    });
    controller->createConnection(hasher, &HashCalculator::errorOccured, this, [=](int index, const QString error) {
        controller->disconnectOnName("progress");
        scrollBox->deleteLast();
        scrollBox->addWidget(new ErrorElement((*assets)[index].path, error));
        HashCalculatorElement *progress = new HashCalculatorElement;
        scrollBox->addWidget(progress);
        controller->createConnection("progress", hasher, &HashCalculator::processingProcess, progress, &HashCalculatorElement::setValue);
        emit calculateNext();
    });
    controller->createConnection(hasher, &HashCalculator::hashCalculated, this, [=](int index, const QString calculatedHash){
        controller->disconnectOnName("progress");
        scrollBox->deleteLast();
        scrollBox->addWidget(new FileHashInfo((*assets)[index].path, (*assets)[index].hash, calculatedHash));
        HashCalculatorElement *progress = new HashCalculatorElement;
        scrollBox->addWidget(progress);
        controller->createConnection("progress", hasher, &HashCalculator::processingProcess, progress, &HashCalculatorElement::setValue);
        emit calculateNext();
    });
    controller->createConnection(this, &MainWindow::calculateNext, hasher, &HashCalculator::calculateNext);
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
            ui->pushButton->setEnabled(false);
            scrollBox->setFocus();
            calculateHashes(XmlFileReaderModule::getFullAsset(filePath));
        }
    }
}

void MainWindow::on_pushButton_2_clicked()
{
    if (ui->lineEdit->text() == "key") {
        scrollBox->addWidget(new VScrollBoxElement(70));
    } else if (ui->lineEdit->text() == "progress") {
        scrollBox->addWidget(new HashCalculatorElement);
    } else if (ui->lineEdit->text() == "error") {
        scrollBox->addWidget(new ErrorElement("/home/uservbox/dev/escodopolnitelniy_razmer_dobavlu/programs/build-DCPHashValidator-Desktop-Debug/DCPHashValidator/DCPHashValidator.exe", "test"));
    } else {
        QString path = "/home/uservbox/dev/programs/build-DCPHashValidator-Desktop-Debug/DCPHashValidator/DCPHashValidator.exe";
        scrollBox->addWidget(new FileHashInfo(path, "qw" + QString::number(items++)));
    }
}

void MainWindow::on_pushButton_3_clicked()
{
    scrollBox->clear();
}

void MainWindow::setVerison(QString version)
{
    ui->label->setText("Version " + version);
}

void MainWindow::on_lineEdit_returnPressed()
{
    if (ui->lineEdit->text() == "DevTools = On") {
        ui->pushButton_2->setVisible(true);
        ui->pushButton_3->setVisible(true);
        ui->lineEdit->setStyleSheet(QLineEdit().styleSheet());
        ui->lineEdit->clear();
    } else if (ui->lineEdit->text() == "DevTools = Off") {
        ui->pushButton_2->setVisible(false);
        ui->pushButton_3->setVisible(false);
        ui->lineEdit->setStyleSheet("QLineEdit {selection-background-color: transparent; "
                                    "background-color: transparent; "
                                    "border: none; "
                                    "selection-color: rgba(0,0,0,0); "
                                    "color: rgba(0,0,0,0);}"
                                    );
        ui->lineEdit->clear();
    }
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    if (scrollBox->count() > 0) {
        QMessageBox box;
        box.setWindowTitle("Подтверждение выхода");
        box.setText("Вы уверены что хотите закрыть программу?\n"
                    "Все результаты вычислений будут потеряны.");
        box.setIcon(QMessageBox::Warning);
        QPushButton *yes = box.addButton("Да", QMessageBox::ActionRole);
        box.addButton("Нет", QMessageBox::ActionRole);
        box.exec();
        if (box.clickedButton() == yes) {
            event->accept();
        } else event->ignore();
    } else event->accept();
}
