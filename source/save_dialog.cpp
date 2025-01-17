#include "save_dialog.h"
#include "ui_save_dialog.h"

#include "logging.h"
#include <QInputDialog>
#include <QMessageBox>
#include <QFileInfo>

Save_dialog::Save_dialog(QWidget *parent) : QDialog(parent), ui(new Ui::Save_dialog)
{
    ui->setupUi(this);

    emit ui->checkBox_hists->toggle();
    emit ui->checkBox_det->toggle();
    emit ui->checkBox_qb->toggle();
    emit ui->checkBox_stats->toggle();

    this->setWindowFlags(Qt::WindowStaysOnTopHint);

    connect(this, &Save_dialog::sig_fileExists, this, &Save_dialog::warnFileExists);
}

Save_dialog::~Save_dialog()
{
    delete ui;
}

void Save_dialog::on_checkBox_hists_toggled(bool checked)
{
    if (checked == true) {
        ui->checkBox_hist_time->setChecked(true);
        ui->checkBox_hist_phase_ok->setChecked(true);
        ui->checkBox_hist_phase_bad->setChecked(true);
    } else {
        ui->checkBox_hist_time->setChecked(false);
        ui->checkBox_hist_phase_ok->setChecked(false);
        ui->checkBox_hist_phase_bad->setChecked(false);
    }
}

void Save_dialog::on_checkBox_det_toggled(bool checked)
{
    if (checked == true) {
        ui->checkBox_h1->setChecked(true);
        ui->checkBox_h2->setChecked(true);
        ui->checkBox_h3->setChecked(true);
    } else {
        ui->checkBox_h1->setChecked(false);
        ui->checkBox_h2->setChecked(false);
        ui->checkBox_h3->setChecked(false);
    }
}

void Save_dialog::on_checkBox_qb_toggled(bool checked)
{
    if (checked == true) {
        ui->checkBox_early->setChecked(true);
        ui->checkBox_late->setChecked(true);
        ui->checkBox_phase->setChecked(true);
    } else {
        ui->checkBox_early->setChecked(false);
        ui->checkBox_late->setChecked(false);
        ui->checkBox_phase->setChecked(false);
    }
}

void Save_dialog::on_checkBox_stats_toggled(bool checked)
{
    if (checked == true) {
        ui->checkBox_stat_time->setChecked(true);
        ui->checkBox_stat_phase->setChecked(true);
        ui->checkBox_stat_voltage->setChecked(true);
    } else {
        ui->checkBox_stat_time->setChecked(false);
        ui->checkBox_stat_phase->setChecked(false);
        ui->checkBox_stat_voltage->setChecked(false);
    }
}

void Save_dialog::on_buttonBox_accepted()
{

    if (fileExists(this->file_path)) {
        emit sig_fileExists(this->file_path);
    } else {
        emit savePressed(this->file_path,
                         ui->checkBox_hist_time->isChecked(), ui->checkBox_hist_phase_ok->isChecked(), ui->checkBox_hist_phase_bad->isChecked(),
                         ui->checkBox_h1->isChecked(), ui->checkBox_h2->isChecked(), ui->checkBox_h3->isChecked(),
                         ui->checkBox_early->isChecked(), ui->checkBox_late->isChecked(), ui->checkBox_phase->isChecked(),
                         ui->checkBox_stat_time->isChecked(), ui->checkBox_stat_phase->isChecked(), ui->checkBox_stat_voltage->isChecked());
        this->close();
    }
}

void Save_dialog::on_buttonBox_rejected()
{
    this->close();
}

void Save_dialog::on_pushButton_file_released()
{
    bool ok;
    QString text_input = QInputDialog::getText(this, tr("Record DATA"), tr("Insert a name for the HDF5 file"), QLineEdit::Normal, this->file_name, &ok);

    if (ok && !text_input.isEmpty()) {
        this->file_name = text_input;
        this->file_path = "../data/" + text_input + ".h5";
        if (fileExists(file_path)) {
            emit sig_fileExists(file_path);
        }
    }
}

bool Save_dialog::fileExists(QString path) {
    QFileInfo check_file(path);
    // check if file exists and is not a directory
    if (check_file.exists() && check_file.isFile()) {
        return true;
    } else {
        return false;
    }
}

void Save_dialog::warnFileExists(QString path) {
    QMessageBox::warning(this, "File Exists", QString("Can't write to %1 as it already exists.").arg(path));
}
