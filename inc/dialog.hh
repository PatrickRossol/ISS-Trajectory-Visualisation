#ifndef DIALOG_HH
#define DIALOG_HH

#include <QDialog>


namespace Ui {
class Dialog;
}

class Dialog : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog(QWidget *parent = nullptr);
    ~Dialog();

private slots:
    void on_pushButton_toggled(bool checked);

private:
    Ui::Dialog *ui;
};

#endif // DIALOG_HH
