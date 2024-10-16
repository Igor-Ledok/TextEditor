#include "settingsdialog.h"
#include "ui_settingsdialog.h"

SettingsDialog::SettingsDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SettingsDialog)
{
    ui->setupUi(this);

    QIcon icon(":/image/resources/images/icon-settings.png");
    this->setWindowIcon(icon);

    if (ui->comboBox->count() == 0) {
        ui->comboBox->addItems({"Light", "Dark"});
    }
}

SettingsDialog::~SettingsDialog()
{
    delete ui;
}


int SettingsDialog::getTextSize() const {
    return ui->spinBox->value();
}

QString SettingsDialog::getTheme() const {
    return ui->comboBox->currentText();
}


void SettingsDialog::setTextSize(int size) {
    ui->spinBox->setValue(size);
}

void SettingsDialog::setTheme(const QString &theme) {
    int index = ui->comboBox->findText(theme);
    if (index >= 0) {
        ui->comboBox->setCurrentIndex(index);
    }
}

void SettingsDialog::on_buttonBox_accepted() {
    accept();
}
