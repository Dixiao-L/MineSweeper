#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "minemap.h"

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

protected:
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);

private:
    Ui::MainWindow *ui;

    MineMap mineMap;
    int offsetx, offsety;
    int ccol, crow;
    bool tog;

private slots:
    void on_action_Newgame_triggered();
    void on_action_Beginner_triggered();
    void on_action_Intermediate_triggered();
    void on_action_Senior_triggered();
    void on_action_Manual_triggered();

};
#endif // MAINWINDOW_H
