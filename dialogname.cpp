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

QString DialogName::nameSet() {
    return ui->lineEditName->text();
}

void DialogName::timeShow(double msec) {
    ui->label_Time->setText(QString::number(msec / 1000) + " s");
}
