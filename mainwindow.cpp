#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    thread()->setPriority(QThread::NormalPriority);
    ui->settingsButton->setIcon(QIcon(":/icons/settings.png"));
    setVerison("1.2.1.0");
    setWindowTitle("Проверка целостности DCP");
    settings = new Settings(this);
    scrollBox = new VerticalScrollBox(this);
    scrollBox->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    ui->gridLayout->addWidget(scrollBox, 0, 0);
    ui->gridLayout->addWidget(ui->widget, 0, 1);
    controller = new ConnectionController(this);
    enableDevTools(false);
    ui->lineEdit->setContextMenuPolicy(Qt::NoContextMenu);
    connect(settings, &Settings::changeSettings, this, &MainWindow::setupOnChanges);
    connect(ui->speedList, &QListWidget::currentRowChanged, this, [=](int index){
        if (index != -1) {
            if (speeds[index] != -1) {
                ui->speedLabel->setText("Скорость: " + QString::number(speeds[index] / 10) + ',' + QString::number(speeds[index] % 10) + " Мб/с");
            } else {
                ui->speedLabel->setText("Скорость: Ошибка");
            }
        } else ui->speedLabel->clear();
    });
}

void MainWindow::calculateHashes(DCPPackage *package)
{
    hashThread = new QThread;
    HashCalculator *hasher = new HashCalculator;
    hasher->setAssetList(package);
    hasher->moveToThread(hashThread);
    controller->createConnection(hashThread, &QThread::started, this, [=](){
        HashCalculatorElement *progress = new HashCalculatorElement;
        scrollBox->addWidget(progress);
        controller->createConnection("progress", hasher, &HashCalculator::processingProcess, progress, &HashCalculatorElement::setValue);
        emit calculateNext();
    });
    controller->createConnection(hasher, &HashCalculator::endReached, this, [=](){
        controller->disconnectOnName("progress");
        scrollBox->deleteLast();
        ui->pushButton->setEnabled(true);
        controller->disconnectOnObject(this); // удаление всех соединений в которых завязан this
        hashThread->quit();
    });
    controller->createConnection(hasher, &HashCalculator::errorOccured, this, [=](int index, const QString error) {
        controller->disconnectOnName("progress");
        QListWidgetItem *item = new QListWidgetItem(QTime::currentTime().toString("hh:mm:ss ") + (*package)[index]->path);
        item->setFont(QFont("Cantarell", 7));
        speeds.push_front(-1);
        ui->speedList->insertItem(0, item);
        ui->speedList->setCurrentRow(0);
        scrollBox->swithLastWidget(new ErrorElement(package->pathOfAsset(index), error));
        HashCalculatorElement *progress = new HashCalculatorElement;
        scrollBox->addWidget(progress);
        controller->createConnection("progress", hasher, &HashCalculator::processingProcess, progress, &HashCalculatorElement::setValue);
        emit calculateNext();
    });
    controller->createConnection(hasher, &HashCalculator::hashCalculated, this, [=](int index, const QString calculatedHash){
        controller->disconnectOnName("progress");
        scrollBox->swithLastWidget(new FileHashInfo(package->pathOfAsset(index), (*package)[index]->hash, calculatedHash));
        speeds.push_front(static_cast<int>(((*package)[index]->size * 10.024f) / timeStamp.msecsTo(QDateTime::currentDateTime())));
        QListWidgetItem *item = new QListWidgetItem(QTime::currentTime().toString("hh:mm:ss ") + (*package)[index]->path);
        item->setFont(QFont("Cantarell", 7));
        ui->speedList->insertItem(0, item);
        ui->speedList->setCurrentRow(0);
        HashCalculatorElement *progress = new HashCalculatorElement;
        scrollBox->addWidget(progress);
        controller->createConnection("progress", hasher, &HashCalculator::processingProcess, progress, &HashCalculatorElement::setValue);
        emit calculateNext();
    });
    controller->createConnection(this, &MainWindow::calculateNext, hasher, &HashCalculator::calculateNext);
    controller->createConnection(hashThread, &QThread::finished, this, [=](){
        delete hashThread;
        controller->disconnectOnObject(this);
    });
    controller->createConnection(this, &MainWindow::calculateNext, this, [=](){
        timeStamp = QDateTime::currentDateTime();
    });
    hashThread->start(priority);
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
    dialog.setNameFilter("ASSETMAP (ASSETMAP ASSETMAP.xml)");
    dialog.setWindowTitle("Выберете ASSETMAP файл DCP пакета");
    if (dialog.exec()) {
        currentPath = dialog.directory().path();
        QString filePath = dialog.selectedFiles().at(0);
        if (filePath != "") {
            scrollBox->clear();
            scrollBox->setFocus();
            DCPPackage *package = XmlFileReaderModule::getFullAsset(filePath);
            if (!package) {
                scrollBox->addWidget(new ErrorElement(filePath, "Возникла ошибка при чтении файла " +  filePath.mid(filePath.lastIndexOf('/') + 1) + '.'));
            } else if (!package->isPKLExist()) {
                scrollBox->addWidget(new ErrorElement(package->pathOfAsset(package->PKL()), "PKL файл не обнаружен. Проверка невозможна."));
            } else if (package->isPKLDamaged()) {
                scrollBox->addWidget(new ErrorElement(package->pathOfAsset(package->PKL()), "Возникла ошибка при чтении PKL файла."));
            } else {
                ui->pushButton->setEnabled(false);
                calculateHashes(package);
            }
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
    if (ui->lineEdit->text() == "Full") {
        scrollBox->clear();
    } else scrollBox->deleteLast();
}

void MainWindow::setVerison(QString version)
{
    ui->label->setText("Version " + version);
}

void MainWindow::on_lineEdit_returnPressed()
{
    if (ui->lineEdit->text() == "DevTools = On") {
        enableDevTools();
    } else if (ui->lineEdit->text() == "DevTools = Off") {
        enableDevTools(false);
    }
}


void MainWindow::on_settingsButton_clicked()
{
    settings->exec();
}

void MainWindow::enableDevTools(bool enable)
{
    if (enable) {
        ui->pushButton_2->setVisible(true);
        ui->pushButton_3->setVisible(true);
        ui->lineEdit->setStyleSheet(QLineEdit().styleSheet());
        ui->lineEdit->clear();
        ui->speedLabel->setVisible(true);
        ui->speedList->setVisible(true);
    } else {
        ui->pushButton_2->setVisible(false);
        ui->pushButton_3->setVisible(false);
        ui->lineEdit->setStyleSheet("QLineEdit {selection-background-color: transparent; "
                                    "background-color: transparent; "
                                    "border: none; "
                                    "selection-color: rgba(0,0,0,0); "
                                    "color: rgba(0,0,0,0);}"
                                    );
        ui->speedLabel->setVisible(false);
        ui->speedList->setVisible(false);
        ui->lineEdit->clear();
        scrollBox->setFocus();
    }
}

void MainWindow::setupOnChanges(QList<QPair<int, bool>> changes)
{
    for (auto it = changes.begin(); it != changes.end(); ++it) {
        switch (it->first) {
        case Settings::TopHint:
            if (it->second) {
                setWindowFlag(Qt::WindowStaysOnTopHint);
                settings->setWindowFlag(Qt::WindowStaysOnTopHint);
            } else {
                setWindowFlag(Qt::WindowStaysOnTopHint, false);
                settings->setWindowFlag(Qt::WindowStaysOnTopHint, false);
            }
            setVisible(true);
            break;
        case Settings::HighPriority:
            if (it->second) {
                priority = QThread::NormalPriority;
            } else {
                priority = QThread::HighPriority;
            }
            if (hashThread && hashThread->isRunning()) hashThread->setPriority(priority);
        }
    }
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    if (scrollBox->count() > 0) {
        QMessageBox box(this);
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
