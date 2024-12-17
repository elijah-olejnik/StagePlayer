#include "keybindings.h"
#include "ui_keybindings.h"

KeyBindings::KeyBindings(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::KeyBindings)
{
    ui->setupUi(this);
    connect(ui->pushButton,SIGNAL(clicked(bool)),this,SLOT(close()));
}

KeyBindings::~KeyBindings()
{
    delete ui;
}
