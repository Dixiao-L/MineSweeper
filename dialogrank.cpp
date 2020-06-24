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

void DialogRank::rankShow() {

    QFile rankFile("rank");
    if(!rankFile.open(QIODevice::ReadOnly | QIODevice::Text))
        return;
    QTextStream in(&rankFile);
    QString strLine, strShow;

    //文件完整性校验
    int flag = 0;
    while (!in.atEnd()) {
        strLine = in.readLine();
        if (strLine == "#BEGIN" || strLine == "#INTER" || strLine == "#SENIOR" || strLine == "#RANKEND")
            ++flag;
    }
//    ui->label_Senior->setNum(flag);
    if (flag == 4)
    {
        in.seek(0);
        strLine = in.readLine();

        if (strLine == "#BEGIN") {
            strLine = in.readLine();
            while (strLine != "#INTER") {
                strShow += '\t' + strLine.section(',', 1, 1) + '\t' + '\t' + QString::number(strLine.section(',', 0, 0).toDouble() / 1000) + " s\n";
                strLine = in.readLine();
            }
            ui->label_Begin->setText(strShow);
            strShow.clear();
            strLine = in.readLine();
            while (strLine != "#SENIOR") {
                strShow += '\t' + strLine.section(',', 1, 1) + '\t' + '\t' + QString::number(strLine.section(',', 0, 0).toDouble() / 1000) + " s\n";
                strLine = in.readLine();
            }
            ui->label_Intermediate->setText(strShow);
            strShow.clear();
            strLine = in.readLine();
            while (strLine != "#RANKEND") {
                strShow += '\t' + strLine.section(',', 1, 1) + '\t' + '\t' + QString::number(strLine.section(',', 0, 0).toDouble() / 1000) + " s\n";
                strLine = in.readLine();
                if (in.atEnd())
                    break;
            }
            ui->label_Senior->setText(strShow);
        }
        rankFile.close();
    }
    else rankFile.remove();
}
