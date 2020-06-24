#ifndef DIALOGNAME_H
#define DIALOGNAME_H

#include <QDialog>

namespace Ui {
class DialogName;
}

class DialogName : public QDialog
{
    Q_OBJECT

public:
    explicit DialogName(QWidget *parent = nullptr);
    ~DialogName();
    QString nameSet();
    void timeShow(double msec);

private:
    Ui::DialogName *ui;
};

#endif // DIALOGNAME_H
