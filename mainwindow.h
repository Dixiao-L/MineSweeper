#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QThread>
#include <QTime>
#include <QTimer>
#include <QFileDialog>

#include "minemap.h"
#include "dialogsize.h"
#include "dialogname.h"
#include "dialogrank.h"
#include "dialogabout.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void paintEvent(QPaintEvent *);
    QTime beginTime;    //开始时间戳

protected:
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);

private:
    Ui::MainWindow *ui;

    MineMap mineMap;
    int offsetx, offsety, offsetcol;
    int ccol, crow; //当前行列
    bool tog;   //是否双键
    int duration;   //用时
    int level;  //0-3   自定义，简单，中级，高级
    QTimer *runtime;

private slots:
    void on_action_Newgame_triggered();
    void on_action_Beginner_triggered();
    void on_action_Intermediate_triggered();
    void on_action_Senior_triggered();
    void on_action_Manual_triggered();
    void on_action_Exit_triggered();
    void on_action_S_triggered();
    void on_action_L_triggered();
    void on_sec();  //计时

    void on_action_Rank_triggered();
    void on_action_About_triggered();

};
#endif // MAINWINDOW_H
