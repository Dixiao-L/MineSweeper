#include "dialogrank.h"
#include "ui_dialogrank.h"

DialogRank::DialogRank(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogRank)
{
    ui->setupUi(this);
}

DialogRank::~DialogRank()
{
    delete ui;
}
