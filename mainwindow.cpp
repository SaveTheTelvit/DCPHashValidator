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

bool MainWindow::readPKL(const QString &filePath)
{
    QFile *file = new QFile(filePath);
    if (file->open(QIODevice::ReadOnly)) {
        QXmlStreamReader xml(file);
        while (!xml.atEnd() && !xml.hasError()) {
            QXmlStreamReader::TokenType token = xml.readNext();
            if (token == QXmlStreamReader::StartElement) {
                if (xml.name().toString() == "Asset") {
                    QPair<QString, QString> data = readAsset(xml);
                    QString path = currentPath + '/' + data.second;
                    data.second = path;
                    assets.push_back(data);
                }
            }
        }
        return true;
    }
    return false;
}

QPair<QString, QString> MainWindow::readAsset(QXmlStreamReader &xml)
{
    QPair<QString, QString> data;
    while (!xml.atEnd() && !xml.hasError()) {
        QXmlStreamReader::TokenType token = xml.readNext();
        if (token == QXmlStreamReader::StartElement) {
            if (xml.name().toString() == "Hash") {
                data.first = xml.readElementText();
            } else if (xml.name().toString() == "OriginalFileName") {
                data.second = xml.readElementText();
                int index;
                while ((index = data.second.indexOf('\\')) != -1) {
                    data.second.remove(index, 1);
                    data.second.insert(index - 1, "/");
                }
            }
        } else if (token == QXmlStreamReader::EndElement) {
            if (xml.name().toString() == "Asset") {
                return data;
            }
        }
    }
}

void MainWindow::calculteHash(const QString &filePath, const QString& hash)
{
    ++cou;
    QThread *thread = new QThread;
    HashCalculator *hasher = new HashCalculator;
    hasher->setPath(filePath);
    hasher->moveToThread(thread);
    HashCalculatorElement * progress = new HashCalculatorElement;
    scrollBox->addWidget(progress);
    connect(thread, &QThread::started, hasher, &HashCalculator::calculate);
    connect(hasher, &HashCalculator::processingProcess, progress, &HashCalculatorElement::setValue);
    connect(hasher, &HashCalculator::errorOccured, this, [=](QString path, QString error){
        thread->quit(); scrollBox->deleteLast();
        scrollBox->addWidget(new ErrorElement(path, error));
        if (cou < assets.count()) calculteHash(assets[cou].second, assets[cou].first);
    });
    connect(hasher, &HashCalculator::hashCalculated, this, [=](QString str){
        thread->quit(); scrollBox->deleteLast();
        scrollBox->addWidget(new FileHashInfo(filePath, hash, str));
        if (cou < assets.count()) calculteHash(assets[cou].second, assets[cou].first);
    });
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
    dialog.setNameFilter("PKL (pkl_*.xml *.pkl.xml PKL_*.xml *.pkl.xml)");
    dialog.setWindowTitle("Выберете PKL файл DCP пакета");
    if (dialog.exec()) {
        currentPath = dialog.directory().path();
        QString filePath = dialog.selectedFiles().at(0);
        if (filePath != "") {
            readPKL(filePath);
        }
    }
    calculteHash(assets[cou].second, assets[cou].first);
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
    scrollBox->deleteLast();
}

