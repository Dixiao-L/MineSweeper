#include "dialogsize.h"
#include "ui_dialogsize.h"

DialogSize::DialogSize(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogSize)
{
    ui->setupUi(this);
}

DialogSize::~DialogSize()
{
    delete ui;
}

int DialogSize::rowCount() {
    return ui->spinBoxRow->value();
}

int DialogSize::columnCount() {
    return ui->spinBoxColumn->value();
}

int DialogSize::mineCount() {
    return ui->spinBoxMine->value();
}
