#include "dialogabout.h"
#include "ui_dialogabout.h"

DialogAbout::DialogAbout(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogAbout)
{
    ui->setupUi(this);
}

DialogAbout::~DialogAbout()
{
    delete ui;
}

void DialogAbout::aboutShow() {
    QPixmap *Tab = new QPixmap(":picTab");
    QPixmap *Wec = new QPixmap(":Wec");
    Tab->scaled(ui->labelFig->size(), Qt::KeepAspectRatio);
    Wec->scaled(ui->labelWechat->sizeHint(), Qt::KeepAspectRatio);
    ui->labelFig->setScaledContents(true);
    ui->labelWechat->setScaledContents(true);
    ui->labelFig->setPixmap(*Tab);
    ui->labelWechat->setPixmap(*Wec);

    ui->labelLink->setText("<a style='color: green;' href=\"https://github.com/Dixiao-L\">GitHub主页");
    ui->labelTg->setText("Telegram: <a style='color: blue;' href=\"https://t.me/LiuDixiao\">@LiuDixiao");
}


void DialogAbout::on_labelLink_linkActivated(const QString &link)
{
    QDesktopServices::openUrl(QUrl(link));
}

void DialogAbout::on_labelTg_linkActivated(const QString &link)
{
    QDesktopServices::openUrl(QUrl(link));
}
