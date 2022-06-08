#include "dialog.hh"
#include "ui_dialog.h"
#include <QDebug>
#include <QPropertyAnimation>


Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog)
{
    ui->setupUi(this);
}

Dialog::~Dialog()
{
    delete ui;
}

void Dialog::on_pushButton_toggled(bool checked)
{
    if(checked)
    {
        ui->textEdit->hide();
        qDebug() << "hide";

        QPropertyAnimation *animation = new QPropertyAnimation(ui->textEdit, "geometry");
                  animation->setDuration(1000);
                  animation->setStartValue(QRect(-100, 50, ui->textEdit->width(), ui->textEdit->height()));
                  animation->setEndValue(QRect(50, 50, ui->textEdit->width(), ui->textEdit->height()));
                  animation->start();
    }

    else
    {
        ui->textEdit->show();
        qDebug() << "show";

        QPropertyAnimation *animation = new QPropertyAnimation(ui->textEdit, "geometry");
                  animation->setDuration(1000);
                  animation->setStartValue(QRect(50, 50, ui->textEdit->width(), ui->textEdit->height()));
                  animation->setEndValue(QRect(-100, 50, ui->textEdit->width(), ui->textEdit->height()));
                  animation->start();
    }
}
