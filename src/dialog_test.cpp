#include "dialog_test.h"
#include "ui_dialog_test.h"

Dialog_test::Dialog_test(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog_test)
{
    ui->setupUi(this);
    ui->pushButton->setCheckable(true);

    ui->widget->hide();
}

Dialog_test::~Dialog_test()
{
    delete ui;
}



void Dialog_test::on_pushButton_clicked(bool checked)
{
    //ui->widget_2->setFixedWidth(ui->widget->width()+65);
   // ui->widget_2->setMaximumWidth(ui->widget->width()+65);
   ui->widget_2->setMaximumSize(10000000,1000000);
    QPoint posButton = ui->widget_2->mapToParent(ui->widget_2->rect().topLeft());

    QPropertyAnimation *animation = new QPropertyAnimation(ui->widget, "geometry");
    animation->setDuration(500);

    animButton = new QPropertyAnimation(ui->widget_2, "geometry");
    animButton->setDuration(500);

    op = new QGraphicsOpacityEffect(ui->widget);
    op1 = new QGraphicsOpacityEffect(ui->widget_2);

    QPropertyAnimation *animationOP = new QPropertyAnimation(op, "opacity");
    animationOP->setDuration(500);

    QPropertyAnimation *animationOP1 = new QPropertyAnimation(op1, "opacity");
    animationOP->setDuration(500);

    if(checked)
    {
        ui->widget->show();

        animation->setStartValue(QRect(-ui->widget->width(), 0, ui->widget->width(), ui->widget->height()));
        animation->setEndValue(QRect(0, 0, ui->widget->width(), ui->widget->height()));
        animation->start();

        animButton->setStartValue(QRect(-ui->widget->width(), 0, ui->widget->width()+65, ui->widget->height()));
        animButton->setEndValue(QRect(0, 0, ui->widget->width()+65, ui->widget->height()));
        animButton->start();

       /* ui->widget->setGraphicsEffect(op);
        ui->widget->setAutoFillBackground(true);

        animationOP->setStartValue(1);
        animationOP->setEndValue(0.1);
        animationOP->start();*/

        ui->widget_2->setGraphicsEffect(op1);
        ui->widget_2->setAutoFillBackground(true);

        animationOP1->setStartValue(1);
        animationOP1->setEndValue(0.4);
        animationOP1->start();
    }

    else
    {
        animation->setStartValue(QRect(0, 0, ui->widget->width(), ui->widget->height()));
        animation->setEndValue(QRect(-ui->widget->width(), 0, ui->widget->width(), ui->widget->height()));
        animation->start();

        animButton->setStartValue(QRect(0, 0, ui->widget->width()+65, ui->widget->height()));
        animButton->setEndValue(QRect(-ui->widget->width(), 0, ui->widget->width()+65, ui->widget->height()));
        animButton->start();

        /*ui->widget->setGraphicsEffect(op);
        ui->widget->setAutoFillBackground(true);

        animationOP->setStartValue(0.1);
        animationOP->setEndValue(1);
        animationOP->start();*/

        ui->widget_2->setGraphicsEffect(op1);
        ui->widget_2->setAutoFillBackground(true);

        animationOP1->setStartValue(0.4);
        animationOP1->setEndValue(1);
        animationOP1->start();

    }
}

void Dialog_test::on_pushButton_clicked()
{

}

void Dialog_test::on_pushButton_2_clicked()
{
    QPoint posButton = ui->widget_2->mapToParent(ui->widget_2->rect().topLeft());
    ui->widget_2->setGeometry(posButton.x() - ui->widget->width(), posButton.y(), ui->widget->width(), ui->widget_2->height());
}
