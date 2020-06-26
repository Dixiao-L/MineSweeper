#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QPainter>
#include <QMouseEvent>
#include <QMessageBox>
#include <QTextStream>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setWindowTitle("扫雷");
    mineMap.Init();
    offsetx = 7;
    offsety = 30;
    offsetcol = 0;
    ccol = -3;
    crow = -3;
    duration = 0;
    level = 1;
    sfx = true;
    bgmb = true;
    bgmName = "bgm.mp3";
    //bgmName = "http://m10.music.126.net/20200626161800/012db663bc4adcee98d74f430edc3ff3/ymusic/obj/w5zDlMODwrDDiGjCn8Ky/2892241091/b1f7/68a7/5b0b/e80919208e2dd73c533d5f9b8a690751.mp3";
    setFixedSize(mineMap.columnNum * 20 + offsetx * 2, mineMap.rowNum * 20 + offsety + 48);
    runtime = new QTimer(this);
    connect(runtime, SIGNAL(timeout()), this, SLOT(on_sec()));
    bgm = new QMediaPlayer;
    bgm->setMedia(QUrl::fromLocalFile(bgmName));
    bgm->play();
    clicksfx = new QSoundEffect(this);
    clicksfx->setSource(QUrl("qrc:clicksfx"));
    bombsfx = new QSoundEffect(this);
    bombsfx->setSource(QUrl("qrc:lost"));
    wonsfx = new QSoundEffect(this);
    wonsfx->setSource(QUrl("qrc:won"));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::paintEvent(QPaintEvent *){
    //QPixmap
    QPixmap mapbmp(":blocks");
    QPixmap numbmp(":num");
    QPixmap framebmp(":frame");
    QPixmap winbmp(":win");
    QPixmap losebmp(":lose");
    QPixmap clickbmp(":click");
    QPixmap statbmp(":stat");

    //Qpainter对象
    QPainter painter(this);

    //如果地图列数小于9需要留余地
    if (mineMap.columnNum < 9)
        offsetcol = (9 - mineMap.columnNum) * 10;
    else
        offsetcol = 0;

    //绘制框架
    painter.drawPixmap(offsetx, offsety, framebmp, 0, 0, 70, 40);
    painter.drawPixmap(qMax(9, mineMap.columnNum) * 10 - 20 + offsetx, offsety, framebmp, 80, 0, 40, 40);
    painter.drawPixmap(qMax(9, mineMap.columnNum) * 20 - 70 + offsetx, offsety, framebmp, 130, 0, 70, 40);
    painter.drawPixmap(70 + offsetx, offsety, qMax(9, mineMap.columnNum) * 10 - 90, 40, framebmp, 70, 0, 10, 40);
    painter.drawPixmap(qMax(9, mineMap.columnNum) * 10 + 20 + offsetx, offsety, qMax(9, mineMap.columnNum) * 10 -90, 40, framebmp, 70, 0, 10, 40);

    //绘制剩余雷数
    int rm = mineMap.mineNum - mineMap.flagNum;
    if (rm < 0) rm =0;
    painter.drawPixmap(6 + offsetx, 5 + offsety, numbmp, rm / 100 * 20, 0, 20, 28);
    if (rm >= 100) rm %= 100;
    painter.drawPixmap(26 + offsetx, 5 + offsety, numbmp, rm / 10 * 20, 0, 20, 28);
    painter.drawPixmap(46 + offsetx, 5 + offsety, numbmp, rm % 10 * 20, 0, 20, 28);

    //绘制时间
    int rt = mineMap.timer;
    if (rt >= 1000) rt %= 1000;
    painter.drawPixmap(qMax(9, mineMap.columnNum) * 20 - 66 + offsetx, 5 + offsety, numbmp, rt / 100 * 20, 0, 20, 28);
    if (rt >= 100) rt %= 100;
    painter.drawPixmap(qMax(9, mineMap.columnNum) * 20 - 46 + offsetx, 5 + offsety, numbmp, rt / 10 * 20, 0, 20, 28);
    painter.drawPixmap(qMax(9, mineMap.columnNum) * 20 - 26 + offsetx, 5 + offsety, numbmp, rt % 10 * 20, 0, 20, 28);

    //动态内容
    if (mineMap.IsLose == 0 && mineMap.IsWin == 0) { //游戏中
        //绘制地图
        for (int i = 0; i < mineMap.rowNum; i++) {
            for (int j = 0; j < mineMap.columnNum; j++) {
                if (mineMap.Map[i][j][STATE] == FLAT)   //未挖开
                {
                    if (tog == 0) {
                        if (i != crow || j != ccol)
                            painter.drawPixmap(j * 20 + offsetx + offsetcol, i * 20 + 40 + offsety, mapbmp, 10 * 20, 0, 20, 20);
                        else
                            painter.drawPixmap(j * 20 + offsetx + offsetcol, i * 20 + 40 + offsety, mapbmp, 0, 0, 20, 20);
                    }
                    else {
                        if (i < crow - 1 || i > crow + 1 || j < ccol - 1 || j > ccol + 1)
                            painter.drawPixmap(j * 20 + offsetx + offsetcol, i * 20 + 40 + offsety, mapbmp, 10 * 20, 0, 20, 20);
                        else
                            painter.drawPixmap(j * 20 + offsetx + offsetcol, i * 20 + 40 + offsety, mapbmp, 0, 0, 20, 20);
                    }
                }
                else if (mineMap.Map[i][j][STATE] == DIGGED)    //挖开
                    painter.drawPixmap(j * 20 + offsetx + offsetcol, i * 20 + 40 + offsety, mapbmp, mineMap.Map[i][j][NUM] * 20, 0, 20, 20);
                else if (mineMap.Map[i][j][STATE] == FLAGGED)   //插旗
                    painter.drawPixmap(j * 20 + offsetx + offsetcol, i * 20 + 40 + offsety, mapbmp, 11 * 20, 0, 20, 20);
                else if (mineMap.Map[i][j][STATE] == QUES)
                    painter.drawPixmap(j * 20 + offsetx + offsetcol, i * 20 + 40 + offsety, mapbmp, 13 * 20, 0, 20, 20);
            }
        }
        //绘制状态
        if (ccol != -3) {
            painter.drawPixmap(mineMap.columnNum * 10 - 12 + offsetx + offsetcol, 7 + offsety, clickbmp, 0, 0, 24, 24);
        }
        else
            painter.drawPixmap(mineMap.columnNum * 10 - 12 + offsetx + offsetcol, 7 + offsety, statbmp, 0, 0, 24, 24);
    }
    else if (mineMap.IsLose == 1) { //输了
        //计时器停止
        if (runtime->isActive()) {
            runtime->stop();
        }

        //绘制地图
        for (int i = 0; i < mineMap.rowNum; i++) {
            for (int j = 0; j < mineMap.columnNum; j++) {
                if (mineMap.Map[i][j][MINE] == 1) {
                    if (mineMap.Map[i][j][STATE] == FLAGGED)
                        painter.drawPixmap(j * 20 + offsetx + offsetcol, i * 20 + 40 + offsety, mapbmp, 11 * 20, 0, 20, 20);
                    else
                        painter.drawPixmap(j * 20 + offsetx + offsetcol, i * 20 + 40 + offsety, mapbmp, 9 * 20, 0, 20, 20);
                }
                else if (mineMap.Map[i][j][STATE] == FLAT)
                    painter.drawPixmap(j * 20 + offsetx + offsetcol, i * 20 + 40 + offsety, mapbmp, 10 * 20, 0, 20, 20);
                else if (mineMap.Map[i][j][STATE] == DIGGED)
                    painter.drawPixmap(j * 20 + offsetx + offsetcol, i * 20 + 40 + offsety, mapbmp, mineMap.Map[i][j][NUM] * 20, 0, 20, 20);
                else if (mineMap.Map[i][j][STATE] == FLAGGED)
                    painter.drawPixmap(j * 20 + offsetx + offsetcol, i * 20 + 40 + offsety, mapbmp, 12 * 20, 0, 20, 20);
                else if (mineMap.Map[i][j][STATE] == QUES)
                    painter.drawPixmap(j * 20 + offsetx + offsetcol, i * 20 + 40 + offsety, mapbmp, 13 * 20, 0, 20, 20);
            }
        }
        //绘制表情状态
        painter.drawPixmap(qMax(9, mineMap.columnNum) * 10 - 12 + offsetx, 7 + offsety, losebmp, 0, 0, 24, 24);
    }
    else if (mineMap.IsWin == 1) { //赢了
        //计时器停止
        if (runtime->isActive()) {
            runtime->stop();
        }
        //绘制地图
        for (int i = 0; i < mineMap.rowNum; i++) {
            for (int j = 0; j < mineMap.columnNum; j++) {
                if (mineMap.Map[i][j][MINE] == 0)   //自动挖开
                    painter.drawPixmap(j * 20 + offsetx + offsetcol, i * 20 + 40 + offsety, mapbmp, mineMap.Map[i][j][NUM] * 20, 0, 20, 20);
                else {  //自动插旗
                    mineMap.flagNum = mineMap.mineNum;
                    painter.drawPixmap(j * 20 + offsetx + offsetcol, i * 20 + 40 + offsety, mapbmp, 11 * 20, 0, 20, 20);
                }
            }
        }
        //绘制表情状态
        painter.drawPixmap(qMax(9, mineMap.columnNum) * 10 - 12 + offsetx, 7 + offsety, winbmp, 0, 0, 24, 24);
    }
}

//鼠标按下动画
void MainWindow::mousePressEvent(QMouseEvent *event){
    int px = event->x() - offsetx - offsetcol;
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
        if ((crow >= 0 && crow < mineMap.rowNum && ccol >= 0 && ccol < mineMap.columnNum) || (px > qMax(9, mineMap.columnNum) * 10 - 12 && px < qMax(9, mineMap.columnNum) * 10 + 12 && py > 7 && py < 7 + 24)){
            update();
        }
    }
}

void MainWindow::mouseMoveEvent(QMouseEvent *event){
    int px = event->x() - offsetx - offsetcol;
    int py = event->y() - offsety;

    ccol = px / 20;
    crow = py / 20 - 2;
    if (crow >= 0 && crow < mineMap.rowNum && ccol >= 0 && ccol < mineMap.columnNum){
        update();
    }
}

void MainWindow::mouseReleaseEvent(QMouseEvent *event){

    int px = event->x() - offsetx - offsetcol;
    int py = event->y() - offsety;

    int col = px / 20;
    int row = py / 20 - 2;
    ccol = -3;
    crow = -3;

    //点击表情重启游戏
    if (px > qMax(9, mineMap.columnNum) * 10 - 12 && px < qMax(9, mineMap.columnNum) * 10 + 12 && py > 7 && py < 7 + 24) {
        runtime->stop();
        mineMap.FreeMap();
        mineMap.Init();
        update();
    }
    else {
        if (sfx == true) clicksfx->play();
        //点击响应
        if (mineMap.IsLose == 0 && mineMap.IsWin == 0) {
            //开始计时
            if (!mineMap.digNum) {
                beginTime = QTime::currentTime();
                if (!(runtime)->isActive())
                    runtime->start(1000);
            }
            //双键连挖
            if (tog) {
                if (row >= 0 && row < mineMap.rowNum && col >= 0 && col < mineMap.columnNum && mineMap.Map[row][col][STATE] == DIGGED)
                    mineMap.MultiDigMine(row, col);
            }
            //左键挖块
            else if (event->button() == Qt::LeftButton) {
    //            while (mineMap.Map[row][col][NUM] != 0)
    //                mineMap.Init();
                if (row >= 0 && row < mineMap.rowNum && col >= 0 && col < mineMap.columnNum)
                    mineMap.DigMine(row, col);
            }
            //右键操作旗子
            else if (event->button() == Qt::RightButton) {
                if (row >= 0 && row < mineMap.rowNum && col >= 0 && col < mineMap.columnNum && mineMap.Map[row][col][STATE] == FLAT)
                    mineMap.FlagIt(row, col);
                else if (row >= 0 && row < mineMap.rowNum && col >= 0 && col < mineMap.columnNum && mineMap.Map[row][col][STATE] == FLAGGED)
                    mineMap.QuesIt(row, col);
                else if (row >= 0 && row < mineMap.rowNum && col >= 0 && col < mineMap.columnNum && mineMap.Map[row][col][STATE] == QUES)
                    mineMap.FlatIt(row, col);
            }
        }

        //判胜
        if (mineMap.rowNum * mineMap.columnNum - mineMap.digNum == mineMap.mineNum) {
            mineMap.IsWin = 1;
            QTime endTime = QTime::currentTime();
            duration = beginTime.msecsTo(endTime) + mineMap.tZero;
            //mineMap.tZero = 0;
        }


        update();

        QPixmap winbmp(":win");
        QPixmap losebmp(":lose");

        //Messagebox提示
        if (mineMap.IsLose) {
            if (sfx == true) bombsfx->play();
            QMessageBox message(QMessageBox::NoIcon, "哎", "你输了！要继续游戏吗？", QMessageBox::Yes | QMessageBox::No, NULL);
            message.setIconPixmap(losebmp);
            if(message.exec() == QMessageBox::Yes) {
                mineMap.FreeMap();
                mineMap.Init();
            }
            else
                exit(0);
        }
        else if (mineMap.IsWin) {
            if (sfx == true) wonsfx->play();
            if (level == 0) {
                QMessageBox message(QMessageBox::NoIcon, "tql", "你赢了！再来一局？", QMessageBox::Yes | QMessageBox::No, NULL);
                message.setIconPixmap(winbmp);
                if (message.exec() == QMessageBox::Yes) {
                    duration = 0;
                    mineMap.FreeMap();
                    mineMap.Init();
                }
                else
                    exit(0);
            }
            else {
                QMessageBox message(QMessageBox::NoIcon, "tql", "你赢了！要将成绩记入龙虎榜吗？", QMessageBox::Yes | QMessageBox::No, NULL);
                message.setIconPixmap(winbmp);
                if (message.exec() == QMessageBox::Yes)
                {
                    //输入姓名
                    DialogName *dlgName = new DialogName(this);
                    Qt::WindowFlags flags = dlgName->windowFlags();
                    dlgName->setWindowFlags(flags|Qt::MSWindowsFixedSizeDialogHint);
                    dlgName->timeShow(duration);

                    int ret = dlgName->exec();
                    if (ret == QDialog::Accepted) {
                        //存储龙虎榜
                        QFile rankFile("rank");
                        if(!rankFile.open(QIODevice::ReadWrite | QIODevice::Text | QIODevice::Append))
                            return;
                        QTextStream ranking(&rankFile);

                        QString strLine, strNew;

                        //文件完整性校验
                        int complete = 1;
                        if (rankFile.pos() == 0) complete = 0;
                        else {
                            rankFile.seek(0);
                            int flag = 0;
                            while (!ranking.atEnd()) {
                                strLine = ranking.readLine();
                                if (strLine == "#BEGIN" || strLine == "#INTER" || strLine == "#SENIOR" || strLine == "#RANKEND")
                                    ++flag;
                            }
                            if (flag != 4) complete = 0;
                        }

                        int flag = 0;
                        if (complete == 0) {
                            rankFile.remove();
                            if(!rankFile.open(QIODevice::WriteOnly | QIODevice::Text))
                                return;
                            strNew += "#BEGIN\n";
                            //ranking << "#BEGIN" << endl;
                            if (level == 1)
                                strNew +=  QString::number(duration) + ',' + dlgName->nameSet() + '\n';
                                //ranking << duration << ',' << dlgName->nameSet() << endl;
                            strNew += "#INTER\n";
                            //ranking << "#INTER" << endl;
                            if (level == 2)
                                strNew +=  QString::number(duration) + ',' + dlgName->nameSet() + '\n';
                                //ranking << duration << ',' << dlgName->nameSet() << endl;
                            strNew += "#SENIOR\n";
                            //ranking << "#SENIOR" << endl;
                            if (level == 3)
                                strNew +=  QString::number(duration) + ',' + dlgName->nameSet() + '\n';
                                //ranking << duration << ',' << dlgName->nameSet() << endl;
                            strNew += "#RANKEND\n";
                            //ranking << "#RANKEND" << endl;
                        }
                        else {
                            rankFile.seek(0);
                            //int diff = 1;
                            int i = 0;
                            strLine = ranking.readLine();   //strLine == "#BEGIN"
                            while (true) {

                                if (level == 1) {
                                    if (strLine != "#INTER"){
                                        if (duration < strLine.section(',', 0, 0).toDouble() && flag == 1) {
                                            strNew += QString::number(duration) + ',' + dlgName->nameSet() + '\n';
                                            ++i;
                                            flag = 0;
                                        }

                                    }
                                    else {
                                        if (i <= 5 && flag == 1){
                                            strNew += QString::number(duration) + ',' + dlgName->nameSet() + '\n';
                                            ++i;
                                            flag = 0;
                                        }
                                    }
                                    if (strLine == "#BEGIN") {
                                        flag = 1;
                                    }
                                }
                                else if (level == 2) {
                                    if (strLine != "#SENIOR"){
                                        if (duration < strLine.section(',', 0, 0).toDouble() && flag == 1) {
                                            strNew += QString::number(duration) + ',' + dlgName->nameSet() + '\n';
                                            ++i;
                                            flag = 0;
                                        }

                                    }
                                    else {
                                        if (i <= 5 && flag == 1){
                                            strNew += QString::number(duration) + ',' + dlgName->nameSet() + '\n';
                                            ++i;
                                            flag = 0;
                                        }
                                    }
                                    if (strLine == "#INTER") {
                                        flag = 1;
                                    }
                                }
                                else if (level == 3) {
                                    if (strLine != "#RANKEND"){
                                        if (duration < strLine.section(',', 0, 0).toDouble() && flag == 1) {
                                            strNew += QString::number(duration) + ',' + dlgName->nameSet() + '\n';
                                            ++i;
                                            flag = 0;
                                        }

                                    }
                                    else {
                                        if (i <= 5 && flag == 1){
                                            strNew += QString::number(duration) + ',' + dlgName->nameSet() + '\n';
                                            ++i;
                                            flag = 0;
                                        }
                                    }
                                    if (strLine == "#SENIOR") {
                                        flag = 1;
                                    }
                                }
                                if (strLine == "#INTER" || strLine == "#SENIOR" || strLine == "#RANKEND") {
                                    i = 0;
                                }
                                if (i == 6) strLine = ranking.readLine();
                                strNew += strLine + '\n';
                                if (ranking.atEnd()) break;
                                strLine = ranking.readLine();
                                ++i;

                            }
                            //strNew += strLine + '\n';
                        }

                        rankFile.remove();
                        if(!rankFile.open(QIODevice::ReadWrite | QIODevice::Text))
                            return;
                        ranking << strNew;
                        rankFile.close();

                        if (flag == 0){
                            QMessageBox message(QMessageBox::NoIcon, "tql", dlgName->nameSet() + " tql。");
                            message.setIconPixmap(winbmp);
                            message.exec();
                        }
                        else{
                            QMessageBox message(QMessageBox::NoIcon, "tql", "很遗憾，你没进前五，但是还是tql。");
                            message.setIconPixmap(losebmp);
                            message.exec();
                        }
                        //显示龙虎榜
                        DialogRank *dlgRank = new DialogRank(this);
                        dlgRank->rankShow();
                        dlgRank->exec();
                    }
                }

                duration = 0;
                mineMap.FreeMap();
                mineMap.Init();
            }
        }
    }

}

void MainWindow::on_action_Newgame_triggered() {
    mineMap.FreeMap();
    //计时器停止
    if (runtime->isActive()) {
        runtime->stop();
    }
    mineMap.Init();
    update();
}

void MainWindow::on_action_Beginner_triggered() {
    mineMap.FreeMap();
    mineMap.rowNum = 9;
    mineMap.columnNum = 9;
    mineMap.mineNum = 10;
    level = 1;
    //计时器停止
    if (runtime->isActive()) {
        runtime->stop();
    }
    mineMap.Init();
    setFixedSize(mineMap.columnNum * 20 + offsetx * 2, mineMap.rowNum * 20 + offsety + 48);
    update();
}

void MainWindow::on_action_Intermediate_triggered() {
    mineMap.FreeMap();
    mineMap.rowNum = 16;
    mineMap.columnNum = 16;
    mineMap.mineNum = 40;
    level = 2;
    //计时器停止
    if (runtime->isActive()) {
        runtime->stop();
    }
    mineMap.Init();
    setFixedSize(mineMap.columnNum * 20 + offsetx * 2, mineMap.rowNum * 20 + offsety + 48);
    update();
}

void MainWindow::on_action_Senior_triggered() {
    mineMap.FreeMap();
    mineMap.rowNum = 16;
    mineMap.columnNum = 30;
    mineMap.mineNum = 99;
    level = 3;
    //计时器停止
    if (runtime->isActive()) {
        runtime->stop();
    }
    mineMap.Init();
    setFixedSize(mineMap.columnNum * 20 + offsetx * 2, mineMap.rowNum * 20 + offsety + 48);
    update();
}

void MainWindow::on_action_Manual_triggered() {
    DialogSize *dlgSize = new DialogSize(this);
    Qt::WindowFlags flags = dlgSize->windowFlags();
    dlgSize->setWindowFlags(flags|Qt::MSWindowsFixedSizeDialogHint);
    int ret = dlgSize->exec();
    if (ret == QDialog::Accepted) {
        if (dlgSize->rowCount() * dlgSize->columnCount() <= dlgSize->mineCount()) {
            QMessageBox::information(this, "小伙你是不有点飘", "你整这老些雷想咋的呢？");
            MainWindow::on_action_Manual_triggered();
        }
        else {
            mineMap.FreeMap();
            mineMap.rowNum = dlgSize->rowCount();
            mineMap.columnNum = dlgSize->columnCount();
            mineMap.mineNum = dlgSize->mineCount();
            level = 0;
            //计时器停止
            if (runtime->isActive()) {
                runtime->stop();
            }
            mineMap.Init();
            setFixedSize(qMax(9, mineMap.columnNum) * 20 + offsetx * 2, mineMap.rowNum * 20 + offsety + 48);
            update();
            delete dlgSize;
        }
    }
}

void MainWindow::on_action_Exit_triggered() {
    mineMap.FreeMap();
    exit(0);
}

void MainWindow::on_sec() {
        ++mineMap.timer;
        update();
}

void MainWindow::on_action_Rank_triggered() {
    //显示龙虎榜
    DialogRank *dlgRank = new DialogRank(this);
    dlgRank->rankShow();
    dlgRank->exec();

}

void MainWindow::on_action_About_triggered() {
    DialogAbout *dlgAbout = new DialogAbout(this);
    dlgAbout->aboutShow();
    dlgAbout->exec();
}

void MainWindow::on_action_L_triggered() {
    QString fileName = QFileDialog::getOpenFileName(this, "载入", "", "存档文件(*.mine)");
    if (fileName.isEmpty())
        return;
    else {
        QFile file(fileName);
        if(!file.open(QIODevice::ReadOnly | QIODevice::Text))
            return;

        mineMap.FreeMap();
        mineMap.IsWin = 0;
        mineMap.IsLose = 0;

        QTextStream readfile(&file);
        QString strLine;
        strLine = readfile.readLine();
        mineMap.flagNum = strLine.section(',', 0, 0).toInt();
        mineMap.digNum = strLine.section(',', 1, 1).toInt();
        mineMap.timer = strLine.section(',', 2, 2).toInt();
        mineMap.tZero = strLine.section(',', 3, 3).toInt();
        mineMap.columnNum = strLine.section(',', 4, 4).toInt();
        mineMap.rowNum = strLine.section(',', 5, 5).toInt();
        level = strLine.section(',', 6, 6).toInt();

        mineMap.Map = (int ***)malloc(mineMap.rowNum * sizeof (int **));
        for (int i = 0; i < mineMap.rowNum; i++) {
            mineMap.Map[i] = (int **)malloc(mineMap.columnNum * sizeof (int *));
            for (int j = 0; j < mineMap.columnNum; j++) {
                strLine = readfile.readLine();
                mineMap.Map[i][j] = (int *)malloc(NOS * sizeof (int));
                mineMap.Map[i][j][MINE] = strLine.section(',', 0, 0).toInt();
                mineMap.Map[i][j][NUM] = strLine.section(',', 1, 1).toInt();
                mineMap.Map[i][j][STATE] = strLine.section(',', 2, 2).toInt();
            }
        }
        file.close();

        if (!runtime->isActive()) {
            runtime->start(1000);
        }
        setFixedSize(qMax(9, mineMap.columnNum) * 20 + offsetx * 2, mineMap.rowNum * 20 + offsety + 48);
        update();
    }
}

void MainWindow::on_action_S_triggered() {
    if (mineMap.IsWin == 0 && mineMap.IsLose == 0) {
        //暂存duration
        QTime endTime = QTime::currentTime();
        int due = beginTime.msecsTo((endTime));

        //计时器停止
//        if (runtime->isActive()) {
//            runtime->stop();
//        }

        QString fileName = QFileDialog::getSaveFileName(this, "存档", QDateTime::currentDateTime().toString("yyyyMMddhhmmss"), "存档文件(*.mine)");
        if (fileName.isEmpty())
            return;
        else {
            QFile file(fileName);
            if(!file.open(QIODevice::WriteOnly | QIODevice::Text))
                return;
            QTextStream save(&file);

            //文档结构 flagnum, dignum, timer, tzero, columnNum, rowNum, level

            save << mineMap.flagNum << ',';
            save << mineMap.digNum << ',';
            save << mineMap.timer << ',';
            save << due << ',';
            save << mineMap.columnNum << ',';
            save << mineMap.rowNum <<',';
            save << level << endl;

            for (int i = 0; i < mineMap.rowNum; i++) {
                for (int j = 0; j<mineMap.columnNum; j++) {
                    save << mineMap.Map[i][j][MINE] << ',';
                    save << mineMap.Map[i][j][NUM] << ',';
                    save << mineMap.Map[i][j][STATE] << endl;
                }
            }
            file.close();
        }
    }
    else
        QMessageBox::warning(NULL, "警告", "游戏已结束，不能存档。", QMessageBox::Ok, QMessageBox::Ok);
//    if (!runtime->isActive())
//        runtime->start(1000);
}

void MainWindow::on_action_Sound_triggered() {
    if (sfx == true) sfx = false;
    else sfx = true;
}

void MainWindow::on_action_B_triggered() {
    if (bgmb == false) {
        bgmb = true;
        bgm->play();
    }
    else {
        bgmb = false;
        bgm->stop();
    }

}

void MainWindow::on_action_Local_triggered() {
    bgmName = QFileDialog::getOpenFileName(this, "选择背景音乐", "bgm.mp3", "音频文件(*.*)");
    if (bgmName.isEmpty())
        return;
    else {
        bgm->setMedia(QUrl::fromLocalFile(bgmName));
        bgm->play();
    }
    connect(bgm,
            static_cast<void(QMediaPlayer::*)(QMediaPlayer::Error)>(&QMediaPlayer::error),
            [=](QMediaPlayer::Error error){
        switch (error) {
        case 0:
            break;
        case 1:
            QMessageBox::critical(NULL, "错误", "无法解析媒体文件", QMessageBox::Yes, NULL);
            break;
        case 2:
            QMessageBox::critical(NULL, "错误", "媒体文件格式错误", QMessageBox::Yes, NULL);
            break;
        case 3:
            QMessageBox::critical(NULL, "错误", "网络错误", QMessageBox::Yes, NULL);
            break;
        case 4:
            QMessageBox::critical(NULL, "错误", "文件拒绝访问", QMessageBox::Yes, NULL);
            break;
        case 5:
            QMessageBox::critical(NULL, "错误", "找不到媒体播放器", QMessageBox::Yes, NULL);
            break;
        case 6:
            break;
        }
    });
}

void MainWindow::on_action_URL_triggered() {
    bool ok = false;
    bgmName = QInputDialog::getText(this, "播放在线音乐", "请输入媒体资源URL:", QLineEdit::Normal, "", &ok);
    bgm->setMedia(QUrl::fromUserInput(bgmName));
    connect(bgm,
            static_cast<void(QMediaPlayer::*)(QMediaPlayer::Error)>(&QMediaPlayer::error),
            [=](QMediaPlayer::Error error){
        switch (error) {
        case 0:
            break;
        case 1:
            QMessageBox::critical(NULL, "错误", "无法解析媒体文件", QMessageBox::Yes, NULL);
            break;
        case 2:
            QMessageBox::critical(NULL, "错误", "媒体文件格式错误", QMessageBox::Yes, NULL);
            break;
        case 3:
            QMessageBox::critical(NULL, "错误", "网络错误", QMessageBox::Yes, NULL);
            break;
        case 4:
            QMessageBox::critical(NULL, "错误", "文件拒绝访问", QMessageBox::Yes, NULL);
            break;
        case 5:
            QMessageBox::critical(NULL, "错误", "找不到媒体播放器", QMessageBox::Yes, NULL);
            break;
        case 6:
            break;
        }
    });
    bgm->play();
}

