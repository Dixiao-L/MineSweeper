#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QPainter>
#include <QMouseEvent>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setWindowTitle(tr("扫雷"));
    mineMap.Init();
    offsetx = 7;
    offsety = 20;
    ccol = -3;
    crow = -3;
    setFixedSize(mineMap.columnNum * 20 + offsetx * 2, mineMap.rowNum * 20 + offsety + 48);

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::paintEvent(QPaintEvent *){
    //QPixmap
    QPixmap mapbmp(":blocks");

    //Qpainter对象
    QPainter painter(this);

    //绘制
    if (mineMap.IsLose == 0 && mineMap.IsWin == 0) { //游戏中
        for (int i = 0; i < mineMap.rowNum; i++) {
            for (int j = 0; j < mineMap.columnNum; j++) {
                if (mineMap.Map[i][j][STATE] == FLAT)   //未挖开
                {
                    if (tog == 0) {
                        if (i != crow || j != ccol)
                            painter.drawPixmap(j * 20 + offsetx, i * 20 + 40 + offsety, mapbmp, 10 * 20, 0, 20, 20);
                        else
                            painter.drawPixmap(j * 20 + offsetx, i * 20 + 40 + offsety, mapbmp, 0, 0, 20, 20);
                    }
                    else {
                        if (i < crow - 1 || i > crow + 1 || j < ccol - 1 || j > ccol + 1)
                            painter.drawPixmap(j * 20 + offsetx, i * 20 + 40 + offsety, mapbmp, 10 * 20, 0, 20, 20);
                        else
                            painter.drawPixmap(j * 20 + offsetx, i * 20 + 40 + offsety, mapbmp, 0, 0, 20, 20);
                    }
                }
                else if (mineMap.Map[i][j][STATE] == DIGGED)    //挖开
                    painter.drawPixmap(j * 20 + offsetx, i * 20 + 40 + offsety, mapbmp, mineMap.Map[i][j][NUM] * 20, 0, 20, 20);
                else if (mineMap.Map[i][j][STATE] == FLAGGED)   //插旗
                    painter.drawPixmap(j * 20 + offsetx, i * 20 + 40 + offsety, mapbmp, 11 * 20, 0, 20, 20);
            }
        }
    }
    else if (mineMap.IsLose == 1) { //输了
        for (int i = 0; i < mineMap.rowNum; i++) {
            for (int j = 0; j < mineMap.columnNum; j++) {
                if (mineMap.Map[i][j][MINE] == 1) {
                    if (mineMap.Map[i][j][STATE] == FLAGGED)
                        painter.drawPixmap(j * 20 + offsetx, i * 20 + 40 + offsety, mapbmp, 11 * 20, 0, 20, 20);
                    else
                        painter.drawPixmap(j * 20 + offsetx, i * 20 + 40 + offsety, mapbmp, 9 * 20, 0, 20, 20);
                }
                else if (mineMap.Map[i][j][STATE] == FLAT)
                    painter.drawPixmap(j * 20 + offsetx, i * 20 + 40 + offsety, mapbmp, 10 * 20, 0, 20, 20);
                else if (mineMap.Map[i][j][STATE] == DIGGED)
                    painter.drawPixmap(j * 20 + offsetx, i * 20 + 40 + offsety, mapbmp, mineMap.Map[i][j][NUM] * 20, 0, 20, 20);
                else if (mineMap.Map[i][j][STATE] == FLAGGED)
                    painter.drawPixmap(j * 20 + offsetx, i * 20 + 40 + offsety, mapbmp, 12 * 20, 0, 20, 20);
            }
        }
    }
    else if (mineMap.IsWin == 1) { //赢了
        for (int i = 0; i < mineMap.rowNum; i++) {
            for (int j = 0; j < mineMap.columnNum; j++) {
                if (mineMap.Map[i][j][STATE] == FLAT && mineMap.Map[i][j][MINE] == 0)   //未挖开
                    painter.drawPixmap(j * 20 + offsetx, i * 20 + 40 + offsety, mapbmp, 10 * 20, 0, 20, 20);
                else if (mineMap.Map[i][j][STATE] == DIGGED)    //挖开
                    painter.drawPixmap(j * 20 + offsetx, i * 20 + 40 + offsety, mapbmp, mineMap.Map[i][j][NUM] * 20, 0, 20, 20);
                else   //自动插旗
                    painter.drawPixmap(j * 20 + offsetx, i * 20 + 40 + offsety, mapbmp, 11 * 20, 0, 20, 20);
            }
        }
    }
    update();
}

void MainWindow::mousePressEvent(QMouseEvent *event){
    int px = event->x() - offsetx;
    int py = event->y() - offsety;
    tog = 0;

    if (event->buttons() == (Qt::LeftButton|Qt::RightButton)) {
        tog = 1;
        ccol = px / 20;
        crow = py / 20 - 2;
        if (crow >= 0 && crow < mineMap.rowNum && ccol >= 0 && ccol < mineMap.columnNum){
            update();
        }
    }
    else if (event->button() == Qt::LeftButton || event->button() == Qt::RightButton) {
        ccol = px / 20;
        crow = py / 20 - 2;
        if (crow >= 0 && crow < mineMap.rowNum && ccol >= 0 && ccol < mineMap.columnNum){
            update();
        }
    }

}

void MainWindow::mouseMoveEvent(QMouseEvent *event){
    int px = event->x() - offsetx;
    int py = event->y() - offsety;

    ccol = px / 20;
    crow = py / 20 - 2;
    if (crow >= 0 && crow < mineMap.rowNum && ccol >= 0 && ccol < mineMap.columnNum){
        update();
    }
}

void MainWindow::mouseReleaseEvent(QMouseEvent *event){
    int px = event->x() - offsetx;
    int py = event->y() - offsety;
    int col = px / 20;
    int row = py / 20 - 2;
    ccol = -3;
    crow = -3;

    //点击响应
    if (mineMap.IsLose == 0 && mineMap.IsWin == 0) {
        //双键连挖
        if (tog) {
            if (row >= 0 && row < mineMap.rowNum && col >= 0 && col < mineMap.columnNum && mineMap.Map[row][col][STATE] == DIGGED)
                mineMap.MultiDigMine(row, col);
        }
        //左键挖块
        else if (event->button() == Qt::LeftButton) {
            if (row >= 0 && row < mineMap.rowNum && col >= 0 && col < mineMap.columnNum)
                mineMap.DigMine(row, col);
        }
        //右键操作旗子
        else if (event->button() == Qt::RightButton) {
            if (row >= 0 && row < mineMap.rowNum && col >= 0 && col < mineMap.columnNum && mineMap.Map[row][col][STATE] == FLAT)
                mineMap.FlagIt(row, col);
            else if (row >= 0 && row < mineMap.rowNum && col >= 0 && col < mineMap.columnNum && mineMap.Map[row][col][STATE] == FLAGGED)
                mineMap.UnFlagIt(row, col);
        }
    }

    //判胜
    if (mineMap.rowNum * mineMap.columnNum - mineMap.digNum == mineMap.mineNum)
        mineMap.IsWin = 1;

    else if (mineMap.cancelNum == mineMap.mineNum)
        mineMap.IsWin = 1;

    update();
}

void MainWindow::on_action_Newgame_triggered() {
    mineMap.Init();
    update();
}

void MainWindow::on_action_Beginner_triggered() {
    mineMap.rowNum = 9;
    mineMap.columnNum = 9;
    mineMap.mineNum = 10;
    mineMap.Init();
    setFixedSize(mineMap.columnNum * 20 + offsetx * 2, mineMap.rowNum * 20 + offsety + 48);
    update();
}

void MainWindow::on_action_Intermediate_triggered() {
    mineMap.rowNum = 16;
    mineMap.columnNum = 16;
    mineMap.mineNum = 40;
    mineMap.Init();
    setFixedSize(mineMap.columnNum * 20 + offsetx * 2, mineMap.rowNum * 20 + offsety + 48);
    update();
}

void MainWindow::on_action_Senior_triggered() {
    mineMap.rowNum = 16;
    mineMap.columnNum = 30;
    mineMap.mineNum = 99;
    mineMap.Init();
    setFixedSize(mineMap.columnNum * 20 + offsetx * 2, mineMap.rowNum * 20 + offsety + 48);
    update();
}

void MainWindow::on_action_Manual_triggered() {

}
