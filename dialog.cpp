#include "dialog.h"
#include "ui_dialog.h"

Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog)
{
    ui->setupUi(this);
}

Dialog::~Dialog()
{
    delete ui;
}

int Dialog::getRows()
{
    return ui->sp_rows->value();
}

int Dialog::getColumns()
{
    return ui->sp_cols->value();
}
