#include "warningdialog.h"
#include "ui_warningdialog.h"

WarningDialog::WarningDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::WarningDialog)
{
    ui->setupUi(this);
    connect(ui->pushButton,&QPushButton::clicked,this,&WarningDialog::close);
}

WarningDialog::~WarningDialog()
{
    delete ui;
}

void WarningDialog::display(QString text)
{
    ui->textBrowser->setText(text);
}
