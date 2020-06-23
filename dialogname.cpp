#include "dialogname.h"
#include "ui_dialogname.h"

DialogName::DialogName(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogName)
{
    ui->setupUi(this);
}

DialogName::~DialogName()
{
    delete ui;
}
