#ifndef SETTINGS_H
#define SETTINGS_H

#include <QDialog>
#include <QCloseEvent>

namespace Ui {
class Settings;
}

class Settings : public QDialog
{
    Q_OBJECT

public:
    enum {
        TopHint = 0,
        HighPriority = 1
    };
    explicit Settings(QWidget *parent = nullptr);
    ~Settings();

signals:
    void changeSettings(QList<QPair<int, bool>>);

private slots:
    void on_discardButton_clicked();
    void on_acceptButton_clicked();

private:
    Ui::Settings *ui;
    bool topHintSetting = false;
    bool highPrioritySetting = false;

protected:
    void closeEvent(QCloseEvent *event) override;
};

#endif // SETTINGS_H
