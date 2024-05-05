#include "settings.h"
#include "ui_settings.h"

Settings::Settings(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::Settings)
{
    ui->setupUi(this);
    setWindowTitle("Настройки");
    setModal(true);
}

Settings::~Settings()
{
    delete ui;
}

void Settings::on_discardButton_clicked()
{
    close();
}

void Settings::on_acceptButton_clicked()
{
    QList<QPair<int, bool>> changes;
    if (topHintSetting != ui->topHintBox->isChecked()) changes.push_back({TopHint, (topHintSetting = ui->topHintBox->isChecked())});
    if (highPrioritySetting != ui->highPriorityBox->isChecked()) changes.push_back({HighPriority, (highPrioritySetting = ui->highPriorityBox->isChecked())});
    emit changeSettings(changes);
    close();
}

void Settings::closeEvent(QCloseEvent *event)
{
    ui->topHintBox->setChecked(topHintSetting);
    ui->highPriorityBox->setChecked(highPrioritySetting);
    QDialog::closeEvent(event);
}
