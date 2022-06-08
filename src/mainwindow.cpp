#include "mainwindow.hh"
#include "ui_mainwindow.h"



#include "dialog.hh"
#include "dialog_test.h"
#include "worker.hh"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    obiektISS = new Worker();
    obiektISS->get("https://api.wheretheiss.at/v1/satellites/25544");

    pixISS = new  QPixmap(":/images/ISS.png");
    myScatterISS = new QCPScatterStyle();

    backPixmap8 = new QCPItemPixmap(ui->plot);

    QGraphicsOpacityEffect *opF = new QGraphicsOpacityEffect(ui->chFlightOP);
    opF->setOpacity(0.75);
    ui->chFlightOP->setGraphicsEffect(opF);
    ui->chFlightOP->setAutoFillBackground(true);

    set_UI();

    ui->chFlight->hide();
    ui->chFlightOP->hide();

    initAltChart();
    initISSChart();

    connect(obiektISS, &Worker::initPath, this, &MainWindow::initObiektPath);

    connect(obiektISS, &Worker::receivedInfoCoords, this, &MainWindow::setInfo);
    connect(obiektISS, &Worker::receivedInfoCoords, this, &MainWindow::setISSChart);
    connect(obiektISS, &Worker::receivedInfoChart, this, &MainWindow::setAltChart);

    connect(obiektISS, &Worker::reveivedInfoPath, this, &MainWindow::getPath);
    connect(ui->plotISS, SIGNAL(mousePress(QMouseEvent*)), SLOT(clickedGraph(QMouseEvent*)));
 }

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::set_UI()
{
    ui->widget->resize(300, this->height());
    ui->widget->move(-ui->widget->width(),0);

    ui->widget_2->resize(ui->widget->width() + BAR, this->height());
    ui->widget_2->move(-ui->widget->width(),0);

    QString style = "QLabel {background-image: url(:/images/pobierz.png); font-size: 12px; font-weight: 450; border-style: hidden; opacity: 1;";
    QString style1 = style, style2 = style, style3 = style, style4 = style, style5 = style;

    ui->label_2->resize((ui->widget->width() - (BAR / 2)) / 2, 40);
    ui->label_2->move(BAR / 2, 12);
    style1 += "border-top-left-radius: 10px; border-bottom-left-radius: 10px}";
    ui->label_2->setStyleSheet(style1);

    ui->label_VelocityISS->resize(ui->label_2->width(),ui->label_2->height());
    ui->label_VelocityISS->move(BAR / 2 + ui->label_2->width(), 12);
    style2 += "border-top-right-radius: 10px; border-bottom-right-radius: 10px}";
    ui->label_VelocityISS->setStyleSheet(style2);

    ui->label_3->resize(ui->label_2->width(),ui->label_2->height());
    ui->label_3->move(BAR / 2, 80);
    ui->label_3->setStyleSheet(style1);

    ui->label_AltitudeISS->resize(ui->label_2->width(),ui->label_2->height());
    ui->label_AltitudeISS->move(BAR / 2 + ui->label_2->width(), 80);
    ui->label_AltitudeISS->setStyleSheet(style2);

    ui->label_4->resize(ui->label_2->width() * 2,ui->label_2->height());
    ui->label_4->move(BAR / 2, 150);
    style3 += "border-top-left-radius: 10px; border-top-right-radius: 10px}";
    ui->label_4->setStyleSheet(style3);

    ui->label_LatitudeISS->resize(ui->label_2->width(),ui->label_2->height());
    ui->label_LatitudeISS->move(BAR / 2, 150 + ui->label_2->height());
    style4 += "border-bottom-left-radius: 10px}";
    ui->label_LatitudeISS->setStyleSheet(style4);

    ui->label_LongitudeISS->resize(ui->label_2->width(),ui->label_2->height());
    ui->label_LongitudeISS->move(BAR / 2 + ui->label_2->width(), 150 + ui->label_2->height());
    style5 += "border-bottom-right-radius: 10px}";
    ui->label_LongitudeISS->setStyleSheet(style5);

    ui->label->move((BAR / 2)+25, 260);
    ui->label->setStyleSheet("QLabel {color: white}");

    ui->label_5->move((BAR / 2)+252, 528);
    ui->label_5->setStyleSheet("QLabel {color: white}");

    ui->plot->resize(ui->widget->width() - (BAR / 2), 300);
    ui->plot->move(BAR / 2, 260);
   //ui->plot->move(BAR / 2, ui->widget->height() - ui->plot->height() - 90);

    ui->pushButton_Dialog->resize(200,100);
    ui->pushButton_Dialog->move((BAR / 2) + ((ui->widget->width() - (BAR / 2)) / 2) - (ui->pushButton_Dialog->width() / 2), ui->widget->height() - ui->pushButton_Dialog->height() - 10);
    ui->pushButton_Dialog->setStyleSheet("background-image: url(:/images/pobierz.png); font-size: 12px; font-weight: 600; border-style: hidden; opacity: 1; border-radius: 10px");

    ui->chFlight->resize(400,250);
    ui->chFlight->move(((this->width() - ui->widget->width() - BAR) / 2) - (ui->chFlight->width() / 2) + ui->widget->width() + BAR, (this->height() / 2) - (ui->chFlight->height() / 2));

    ui->chFlightOP->resize(ui->chFlight->width(), ui->chFlight->height());
    ui->chFlightOP->move(((this->width() - ui->widget->width() - BAR) / 2) - (ui->chFlight->width() / 2) + ui->widget->width() + BAR, (this->height() / 2) - (ui->chFlight->height() / 2));
    ui->chFlightOP->setStyleSheet("background-image: url(:/images/pobierz.png); opacity: 0,5; border-radius: 15px");

    ui->check_button->resize(100,50);
    ui->check_button->move((ui->chFlight->width() - ui->check_button->width()) / 2, ui->chFlight->height() - ui->check_button->height()- 10);
    ui->check_button->setStyleSheet("background-image: url(:/images/pobierz.png); font-size: 12px; font-weight: 500; border-style: hidden; opacity: 1; border-radius: 10px");

    ui->close_button->resize(30,30);
    ui->close_button->move(ui->chFlight->width() - ui->close_button->width() - 10, 10);

    ui->des_label->resize(250, 50);
    ui->des_label->move((ui->chFlight->width() - ui->des_label->width()) / 2, 20);
    ui->des_label->setStyleSheet(style + "border-radius: 10px; font-size: 15px}");

    ui->lat_label->resize(100,30);
    ui->lat_label->move((ui->chFlight->width() / 2) - ui->lat_label->width() - 30, 90);
    ui->lat_label->setStyleSheet(style + "border-radius: 10px; font-size: 12px}");

    ui->long_label->resize(ui->lat_label->width(), ui->lat_label->height());
    ui->long_label->move((ui->chFlight->width() / 2) + 30, 90);
    ui->long_label->setStyleSheet(style + "border-radius: 10px; font-size: 12px}");

    ui->textEdit->resize(ui->lat_label->width(),ui->lat_label->height());
    ui->textEdit->move((ui->chFlight->width() / 2) - ui->lat_label->width() - 30, 90 + ui->lat_label->height() + 5);

    ui->textEdit_2->resize(ui->lat_label->width(),ui->lat_label->height());
    ui->textEdit_2->move((ui->chFlight->width() / 2) + 30, 90 + ui->lat_label->height() + 5);

    set_Button();
}

void MainWindow::set_Button()
{
    ui->pushButton_Center->resize(40,40);
    ui->pushButton_Path->resize(40,40);
    ui->pushButton_View->resize(40,40);

    ui->pushButton_Center->move(this->width()-45, this->height()-135);
    ui->pushButton_Path->move(this->width()-45, this->height()-90);
    ui->pushButton_View->move(this->width()-45, this->height()-45);
    ui->pushButton_Panel->move(ui->widget_2->width()-62, 2);

    QString buttonStyle = "background-color: white; border-style: hidden; border-width: 3px; border-radius: 7px;";
    #define button_size 4/6

    ui->pushButton_Panel->setCheckable(true);
    ui->pushButton_Panel->setIcon(QIcon(":/images/menu.jpg"));
    ui->pushButton_Panel->setIconSize(QSize( ui->pushButton_Panel->width()*1.5,  ui->pushButton_Panel->height()*1.5));
    ui->pushButton_Panel->setStyleSheet(buttonStyle);

    ui->pushButton_View->setCheckable(true);
    ui->pushButton_View->setIcon(QIcon(":/images/view.png"));
    ui->pushButton_View->setIconSize(QSize( ui->pushButton_View->width()*button_size,  ui->pushButton_View->height()*button_size));
    ui->pushButton_View->setStyleSheet(buttonStyle);

    ui->pushButton_Path->setCheckable(true);
    ui->pushButton_Path->setIcon(QIcon(":/images/path.png"));
    ui->pushButton_Path->setIconSize(QSize( ui->pushButton_Path->width()*button_size,  ui->pushButton_Path->height()*button_size));
    ui->pushButton_Path->setStyleSheet(buttonStyle);

    ui->pushButton_Center->setCheckable(true);
    ui->pushButton_Center->setIcon(QIcon(":/images/location.png"));
    ui->pushButton_Center->setIconSize(QSize( ui->pushButton_Center->width()*button_size,  ui->pushButton_Center->height()*button_size));
    ui->pushButton_Center->setStyleSheet(buttonStyle);

    ui->close_button->setIcon(QIcon(":/images/close.png"));
    ui->close_button->setIconSize(QSize( ui->close_button->width()*button_size,  ui->close_button->height()*button_size));
    ui->close_button->setStyleSheet("border-style: hidden; border-width: 3px; border-radius: 7px;");
}

void MainWindow::resizeEvent(QResizeEvent *event)
{
    ui->widget_2->resize(ui->widget->width()+BAR, this->height());
    ui->widget->resize(ui->widget->width(), this->height());

    ui->pushButton_Center->move(this->width()-45, this->height()-135);
    ui->pushButton_Path->move(this->width()-45, this->height()-90);
    ui->pushButton_View->move(this->width()-45, this->height()-45);
    ui->chFlight->move(((this->width() - ui->widget->width() - BAR) / 2) - (ui->chFlight->width() / 2) + ui->widget->width() + BAR, (this->height() / 2) - (ui->chFlight->height() / 2));
    ui->chFlightOP->move(((this->width() - ui->widget->width() - BAR) / 2) - (ui->chFlight->width() / 2) + ui->widget->width() + BAR, (this->height() / 2) - (ui->chFlight->height() / 2));


    //ui->plot->move(BAR / 2, ui->widget->height() - ui->plot->height() - 90);
    ui->pushButton_Dialog->move((BAR / 2) + ((ui->widget->width() - (BAR / 2)) / 2) - (ui->pushButton_Dialog->width() / 2), ui->widget->height() - ui->pushButton_Dialog->height() - 10);

    QWidget::resizeEvent(event);
}

void MainWindow::initAltChart()
{    
    ui->plot->addLayer("image", ui->plot->layer("main"), QCustomPlot::LayerInsertMode::limBelow);

    ui->plot->addGraph();
    ui->plot->addGraph();

    QFont pfont("Newyork",6);
    pfont.setStyleHint(QFont::SansSerif);
    pfont.setPointSize(3);
    ui->plot->xAxis->setTickLabelFont(pfont);
    ui->plot->yAxis->setTickLabelFont(pfont);

    QCPScatterStyle *myScatter = new QCPScatterStyle();
    myScatter->setPixmap(pixISS->scaledToWidth(50));
    ui->plot->graph(0)->setScatterStyle(*myScatter);
}

void MainWindow::initISSChart()
{
    setBackr_ISSChart();

    ui->plotISS->axisRect()->setAutoMargins(QCP::msNone);
    ui->plotISS->xAxis->setVisible(false);
    ui->plotISS->yAxis->setVisible(false);

    ui->plotISS->graph(1)->setPen(QPen(Qt::red, 3));
    ui->plotISS->graph(1)->setVisible(false);

    //ui->plotISS->setInteraction(QCP::iRangeDrag, true);
    //ui->plotISS->setInteraction(QCP::iRangeZoom, true);
}

void MainWindow::setBackr_ISSChart()
{
    QCPItemPixmap *backPixmap1 = new QCPItemPixmap(ui->plotISS);
    backPixmap1->setPixmap(QPixmap(":/images/ziemia_high.jpg"));
    backPixmap1->topLeft->setType(QCPItemPosition::ptPlotCoords);
    backPixmap1->bottomRight->setType(QCPItemPosition::ptPlotCoords);
    backPixmap1->topLeft->setCoords(-540,90);
    backPixmap1->bottomRight->setCoords(-180,-90);
    backPixmap1->setScaled(true,Qt::IgnoreAspectRatio);

    QCPItemPixmap *backPixmap2 = new QCPItemPixmap(ui->plotISS);
    backPixmap2->setPixmap(QPixmap(":/images/ziemia_high.jpg"));
    backPixmap2->topLeft->setType(QCPItemPosition::ptPlotCoords);
    backPixmap2->bottomRight->setType(QCPItemPosition::ptPlotCoords);
    backPixmap2->topLeft->setCoords(-180,90);
    backPixmap2->bottomRight->setCoords(180,-90);
    backPixmap2->setScaled(true,Qt::IgnoreAspectRatio);

    QCPItemPixmap *backPixmap3 = new QCPItemPixmap(ui->plotISS);
    backPixmap3->setPixmap(QPixmap(":/images/ziemia_high.jpg"));
    backPixmap3->topLeft->setType(QCPItemPosition::ptPlotCoords);
    backPixmap3->bottomRight->setType(QCPItemPosition::ptPlotCoords);
    backPixmap3->topLeft->setCoords(180,90);
    backPixmap3->bottomRight->setCoords(540,-90);
    backPixmap3->setScaled(true,Qt::IgnoreAspectRatio);

    QCPItemPixmap *backPixmap4 = new QCPItemPixmap(ui->plotISS);
    backPixmap4->setPixmap(QPixmap(":/images/ziemia_high.jpg"));
    backPixmap4->topLeft->setType(QCPItemPosition::ptPlotCoords);
    backPixmap4->bottomRight->setType(QCPItemPosition::ptPlotCoords);
    backPixmap4->topLeft->setCoords(-900,90);
    backPixmap4->bottomRight->setCoords(-540,-90);
    backPixmap4->setScaled(true,Qt::IgnoreAspectRatio);

    QCPItemPixmap *backPixmap5 = new QCPItemPixmap(ui->plotISS);
    backPixmap5->setPixmap(QPixmap(":/images/ziemia_high.jpg"));
    backPixmap5->topLeft->setType(QCPItemPosition::ptPlotCoords);
    backPixmap5->bottomRight->setType(QCPItemPosition::ptPlotCoords);
    backPixmap5->topLeft->setCoords(900,90);
    backPixmap5->bottomRight->setCoords(540,-90);
    backPixmap5->setScaled(true,Qt::IgnoreAspectRatio);

    ui->plotISS->addLayer("image", ui->plotISS->layer("main"), QCustomPlot::LayerInsertMode::limBelow);

    ui->plotISS->addGraph();
    ui->plotISS->addGraph();
}

void MainWindow::plotAltChart()
{
    while(qv_x.size()> 200)
        qv_x.erase(qv_x.begin());

    while(qv_y.size()> 200)
        qv_y.erase(qv_y.begin());

    auto subv_x1 = qv_x.mid(qv_x.size()-1);
    auto subv_y1 = qv_y.mid(qv_y.size()-1);

    ui->plot->graph(0)->setData(subv_x1, subv_y1);
    ui->plot->graph(1)->setData(qv_x, qv_y);

    ui->plot->replot();
    ui->plot->update();
}

void MainWindow::plotISSChart()
{
    while(vec_ISSx.size()> 6000)
        vec_ISSx.erase(vec_ISSx.begin());

    while(vec_ISSy.size()> 6000)
        vec_ISSy.erase(vec_ISSy.begin());

    auto sVec_ISSx = vec_ISSx.mid(vec_ISSx.size()-1);
    auto sVec_ISSy = vec_ISSy.mid(vec_ISSy.size()-1);

    double offset = 0, mult = 0;
    int size_x = vec_ISSx.size() - 1;

    for(int i = 0; size_x - i != (-2); i++)
    {
        if(!(size_x - i - 1 < 0))
            if((vec_ISSx[size_x - i - 1] > 0) && (vec_ISSx[size_x - i] < 0))
                mult++;

        offset = 360 * mult;

        if(!(size_x - i + 2 > size_x - 1))
            h_vec_ISSx[size_x - i + 1] = vec_ISSx[size_x - i + 1] - offset;
    }

    ui->plotISS->graph(0)->setData(sVec_ISSx, sVec_ISSy);
    ui->plotISS->graph(1)->setData(h_vec_ISSx, h_vec_ISSy);

    ui->plotISS->replot();
    ui->plotISS->update();
}

void MainWindow::initObiektPath(int &time)
{
    QString req;

    int mult = 60, inter = 150; //60
    int mult2 = 5, inter2 = 10;

    if( disconnect(obiektISS, &Worker::initPath, this, &MainWindow::initObiektPath))
    {
       this->time_init = time;
       this->a = time - (mult * inter + mult2 * inter2);
       this->b = time - (mult2 * inter2);

       qDebug() << "rozłączyłololololololololololo";
    }

    if(a < time_init - (mult2 * inter2))
    {
        req = "https://api.wheretheiss.at/v1/satellites/25544/positions?timestamps=" + QString::number(a);
        qDebug() << req;
        qDebug() << "reqtescior" << "AAA ----->" << a;
        obiektISS->get_Path(req);
        a+= inter;
    }

    if(space)
    {
        if(b <= time_init)
        {
            req = "https://api.wheretheiss.at/v1/satellites/25544/positions?timestamps=" + QString::number(b);
            qDebug() << req;
            qDebug() << "reqtescior" << "BBBB ----->" << b;
            obiektISS->get_Path(req);
            b += inter2;
        }
    }

    if(!(a < time_init - (mult2 * inter2)) and this->space == 0)
        this->space = 1;

    if(space == 2)
        obiektISS->get("https://api.wheretheiss.at/v1/satellites/25544");

     if(!(b <= time_init))
         this->space = 2;
}

void MainWindow::getPath(float &lon, float &lat)
{
    double dLon = (double)lon;
    double dLat = (double)lat;
    vec_ISSx.append(dLon);
    vec_ISSy.append(dLat);
    h_vec_ISSx.append(dLon);
    h_vec_ISSy.append(dLat);
    qDebug() << "test apendu hehehehe" << dLon << "---" << dLat;

    int i = 0;

    initObiektPath(i);
}

void MainWindow::setInfo(float &lon, float &lat, int& vel, float &alt)
{
    ui->label_VelocityISS->setText(QString::number(vel) + " km/h");
    ui->label_AltitudeISS->setText(QString::number(alt) + " km");
    ui->label_LongitudeISS->setText(QString::number(lon));
    ui->label_LatitudeISS->setText(QString::number(lat));
}

void MainWindow::setISSChart(float &lon, float &lat)
{
    double dLon = (double)lon;
    double dLat = (double)lat;
    vec_ISSx.append(dLon);
    vec_ISSy.append(dLat);
    h_vec_ISSx.append(dLon);
    h_vec_ISSy.append(dLat);

    if(this->viewISS)
    {        
        ui->plotISS->setInteraction(QCP::iRangeDrag, true);
        ui->plotISS->setInteraction(QCP::iRangeZoom, false);
        ui->plotISS->axisRect(0)->setRangeDrag(Qt::Horizontal);

        if(!this->mousePress)
        {
            ui->plotISS->xAxis->setRange(-180, 180);
            ui->plotISS->yAxis->setRange(-90, 90);
        }

        myScatterISS->setPixmap(pixISS->scaledToWidth(100));
    }

    else
    {
        ui->plotISS->setInteraction(QCP::iRangeDrag, true);
        ui->plotISS->setInteraction(QCP::iRangeZoom, true);
        ui->plotISS->axisRect(0)->setRangeDrag(Qt::Horizontal | Qt::Vertical);

        if(!this->mousePress)
        {
            ui->plotISS->xAxis->setRange(dLon - 10, dLon + 10);
            ui->plotISS->yAxis->setRange(dLat - 5, dLat + 5);
        }

        myScatterISS->setPixmap(pixISS->scaledToWidth(140));
    }

    ui->plotISS->graph(0)->setScatterStyle(*myScatterISS);

    plotISSChart();
}

void MainWindow::setAltChart(int &time, float &alt)
{
    double dTime = (double)time;
    double dAlt = (double)alt;
    qv_x.append(dTime);
    qv_y.append(dAlt);

    if(liczAlt == 1) backPixmap8->setPixmap(QPixmap(":/images/1.jpg"));
    if(liczAlt == 2) backPixmap8->setPixmap(QPixmap(":/images/3.jpg"));
    if(liczAlt == 3) backPixmap8->setPixmap(QPixmap(":/images/2.jpg"));
    if(liczAlt == 4) backPixmap8->setPixmap(QPixmap(":/images/4.jpg"));

    if(liczAlt < 4) liczAlt++;
    else liczAlt = 1;

    backPixmap8->topLeft->setType(QCPItemPosition::ptPlotCoords);
    backPixmap8->bottomRight->setType(QCPItemPosition::ptPlotCoords);
    backPixmap8->topLeft->setCoords(dTime-30-150,dAlt+2);
    backPixmap8->bottomRight->setCoords(dTime+2+150,dAlt-2-1);
    backPixmap8->setScaled(true,Qt::IgnoreAspectRatio);

    ui->plot->axisRect()->setAutoMargins(QCP::msNone);
    ui->plot->axisRect()->setMargins(QMargins(22,0,0,12));

  //  ui->plot->yAxis->setLabel("km");

    ui->plot->xAxis->setRange(dTime-30, dTime+2);
    ui->plot->yAxis->setRange(dAlt-2, dAlt+2);
    plotAltChart();
}

void MainWindow::on_pushButton_Dialog_clicked()
{
    /*Dialog_test dialog;
    dialog.setModal(true);
    dialog.exec();*/

    ui->chFlight->show();
    ui->chFlightOP->show();
}

void MainWindow::on_pushButton_Panel_clicked(bool checked)
{
   // this->AnimOut = false;
   // this->AnimIn = false;

    QPropertyAnimation *animation = new QPropertyAnimation(ui->widget, "geometry");
    animation->setDuration(500);
    QPropertyAnimation *animButton = new QPropertyAnimation(ui->widget_2, "geometry");
    animButton->setDuration(500);

    op = new QGraphicsOpacityEffect(ui->widget);
    op1 = new QGraphicsOpacityEffect(ui->widget_2);

    QPropertyAnimation *animationOP = new QPropertyAnimation(op, "opacity");
    animationOP->setDuration(500);
    QPropertyAnimation *animationOP1 = new QPropertyAnimation(op1, "opacity");
    animationOP->setDuration(500);

    if(checked)
    {
        animation->setStartValue(QRect(-ui->widget->width(), 0, ui->widget->width(), ui->widget->height()));
        animation->setEndValue(QRect(0, 0, ui->widget->width(), ui->widget->height()));
        animation->start();

        animButton->setStartValue(QRect(-ui->widget->width(), 0, ui->widget_2->width(), ui->widget_2->height()));
        animButton->setEndValue(QRect(0, 0, ui->widget_2->width(), ui->widget_2->height()));
        animButton->start();

        ui->widget_2->setGraphicsEffect(op1);
        ui->widget_2->setAutoFillBackground(true);

        animationOP1->setStartValue(1);
        animationOP1->setEndValue(0.75);
        animationOP1->start();
    }

    else
    {
        animation->setStartValue(QRect(0, 0, ui->widget->width(), ui->widget->height()));
        animation->setEndValue(QRect(-ui->widget->width(), 0, ui->widget->width(), ui->widget->height()));
        animation->start();

        animButton->setStartValue(QRect(0, 0, ui->widget_2->width(), ui->widget_2->height()));
        animButton->setEndValue(QRect(-ui->widget->width(), 0, ui->widget_2->width(), ui->widget_2->height()));
        animButton->start();

        ui->widget_2->setGraphicsEffect(op1);
        ui->widget_2->setAutoFillBackground(true);

        animationOP1->setStartValue(0.75);
        animationOP1->setEndValue(1);
        animationOP1->start();
    }
}

void MainWindow::on_pushButton_View_clicked(bool checked)
{
    if(checked)
    {
        this->viewISS = 1;
        ui->pushButton_Center->hide();
    }
    else
    {
        this->viewISS = 0;
        ui->pushButton_Center->show();
    }
    this->mousePress = 0;
}

void MainWindow::on_pushButton_Path_clicked(bool checked)
{
    if(checked)
    {
        this->pathISS = 1;
        ui->plotISS->graph(1)->setVisible(true);
    }
    else
    {
        this->pathISS = 0;
        ui->plotISS->graph(1)->setVisible(false);
    }
}

void MainWindow::on_pushButton_Center_clicked()
{
    this->mousePress = 0;
}

void MainWindow::clickedGraph(QMouseEvent *event)
{
    this->mousePress = 1;
    QPoint point = event->pos();
    qDebug() << ui->plotISS->xAxis->pixelToCoord(point.x()) << ui->plotISS->yAxis->pixelToCoord(point.y());
}

void MainWindow::on_close_button_clicked()
{
    ui->chFlight->hide();
    ui->chFlightOP->hide();
}
