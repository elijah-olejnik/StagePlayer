#include "exitdialog.h"
#include "ui_exitdialog.h"
#include "mainwindow.h"

exitDialog::exitDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::exitDialog)
{
    ui->setupUi(this);
    connect(ui->buttonBox,SIGNAL(accepted()),this,SLOT(closeaccept()));
    connect(ui->buttonBox,SIGNAL(rejected()),this,SLOT(close()));
    connect(ui->pushButton,SIGNAL(clicked()),this,SLOT(saveslot()));
    connect(ui->pushButton_2,SIGNAL(clicked()),this,SLOT(savesamplerslot()));
}

void exitDialog::closeaccept(){exit(0);}

void exitDialog::saveslot(){emit SaveSignal();}

void exitDialog::savesamplerslot(){emit SaveSampler();}

exitDialog::~exitDialog(){delete ui;}
