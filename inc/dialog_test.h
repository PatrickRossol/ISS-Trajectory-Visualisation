#ifndef DIALOG_TEST_H
#define DIALOG_TEST_H

#include <QDialog>
#include <QPropertyAnimation>
#include <QDebug>
#include <QGraphicsOpacityEffect>

namespace Ui {
class Dialog_test;
}

class Dialog_test : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog_test(QWidget *parent = nullptr);
    ~Dialog_test();

private slots:


    void on_pushButton_clicked(bool checked);

    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

private:
    Ui::Dialog_test *ui;
    QGraphicsOpacityEffect *op, *op1;
    QPropertyAnimation *animButton;
};

#endif // DIALOG_TEST_H
