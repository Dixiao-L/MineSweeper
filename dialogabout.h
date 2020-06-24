#ifndef DIALOGABOUT_H
#define DIALOGABOUT_H

#include <QDialog>
#include <QDesktopServices>
#include <QUrl>

namespace Ui {
class DialogAbout;
}

class DialogAbout : public QDialog
{
    Q_OBJECT

public:
    explicit DialogAbout(QWidget *parent = nullptr);
    ~DialogAbout();
    void aboutShow();

private slots:
    void on_labelLink_linkActivated(const QString &link);

    void on_labelTg_linkActivated(const QString &link);

private:
    Ui::DialogAbout *ui;
};

#endif // DIALOGABOUT_H
