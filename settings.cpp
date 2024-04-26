#include "settings.h"
#include "ui_settings.h"

Settings::Settings(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::Settings)
{
    ui->setupUi(this);
    setWindowTitle("Настройки");
    setModal(true);
    setWindowFlag(Qt::WindowStaysOnTopHint);
}

Settings::~Settings()
{
    delete ui;
}

void Settings::setupSettings()
{
    if (topHintSetting) ui->topHintBox->setChecked(true);
}

void Settings::on_discardButton_clicked()
{
    close();
}

void Settings::on_acceptButton_clicked()
{
    QList<QPair<int, bool>> changes;
    if (topHintSetting != ui->topHintBox->isChecked()) {
        changes.push_back({TopHint, (topHintSetting = ui->topHintBox->isChecked())});
    }
    emit changeSettings(changes);
    close();
}

void Settings::closeEvent(QCloseEvent *event)
{
    ui->topHintBox->setChecked(topHintSetting);
    QDialog::closeEvent(event);
}
