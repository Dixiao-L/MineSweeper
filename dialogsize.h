#ifndef DIALOGSIZE_H
#define DIALOGSIZE_H

#include <QDialog>

namespace Ui {
class DialogSize;
}

class DialogSize : public QDialog
{
    Q_OBJECT

public:
    explicit DialogSize(QWidget *parent = nullptr);
    ~DialogSize();
    int rowCount();
    int columnCount();
    int mineCount();

private:
    Ui::DialogSize *ui;
};

#endif // DIALOGSIZE_H
