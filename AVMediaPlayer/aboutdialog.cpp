#include "aboutdialog.h"
#include "ui_aboutdialog.h"

AboutDialog::AboutDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AboutDialog)
{
    ui->setupUi(this);

    ui->lblAuthor->setText(tr("Author: ") + AUTHOR);
    ui->lblVersion->setText(tr("Version: ") + VERSION);
    ui->lblBuild->setText(tr("Build date: ") + BUILD_DATE);
    ui->lblCore->setText(tr("Core: ") + CORE);
    ui->lblPlat->setText(tr("Platform: ") + QSysInfo().prettyProductName());
}

AboutDialog::~AboutDialog()
{
    delete ui;
}
