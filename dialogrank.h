#ifndef DIALOGRANK_H
#define DIALOGRANK_H

#include <QDialog>
#include <QFile>
#include <QTextStream>

namespace Ui {
class DialogRank;
}

class DialogRank : public QDialog
{
    Q_OBJECT

public:
    explicit DialogRank(QWidget *parent = nullptr);
    ~DialogRank();
    void rankShow();

private:
    Ui::DialogRank *ui;
};

#endif // DIALOGRANK_H
