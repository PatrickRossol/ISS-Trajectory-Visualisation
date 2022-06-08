#ifndef MAINWINDOW_HH
#define MAINWINDOW_HH

#include <QMainWindow>
#include <QtGui>
#include <QtCore>
#include <QFile>
#include <QTextStream>
#include <QMessageBox>
#include <QGraphicsScene>
#include <QGraphicsPixmapItem>
#include <QGraphicsItem>
#include <QAbstractAnimation>

#include "worker.hh"
#include "qcustomplot.h"
#define BAR 65


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    /*!
     * \brief Ustawianie przycisków
     *
     * Ustawianie wyglądu, wymiarów i położenie przycisków.
     */
    void set_Button();

    /*!
     * \brief Ustawianie UI
     *
     * Ustawanie układu i wyglądu interfejsu użytkownika.
     */
    void set_UI();

    /*!
     * \brief Inicjalizacja wykresu wysokości stacji
     *
     * Jest wykonywana jednokrotnie tylko przy starcie
     * programu w celu zaincjalizowania oraz ustawienia
     * wykresu.
     */
    void initAltChart();

    /*!
     * \brief Inicjalizacja wykresu położenia stacji
     *
     * Jest wykonywana jednokrotnie tylko przy starcie
     * programu w celu zaincjalizowania oraz ustawienia
     * wykresu.
     */
    void initISSChart();

    /*!
     * \brief Tło wykresu położenia stacji
     *
     * Ustawia tło o zadanych współrzędnych, aby odpowiednio
     * wypełnić ekran.
     */
    void setBackr_ISSChart();

    /*!
     * \brief Plotowanie wykresu wysokości
     */
    void plotAltChart();

    /*!
     * \brief Plotowanie wykresu położenia
     */
    void plotISSChart();


    /*!
     * \brief Inicjalizacja przebytej trasy
     *
     * Wysyła zapytanie do serwera o przebytą wcześniej trasę.
     * \param[in] time - znak czasu [s].
     */
    void initObiektPath(int &time);

    /*!
     * \brief Otrzymanie przebytej trasy
     *
     * Ładuje otrzymane informacje o położeniu do QVectora,
     * który następnie jest przekazywany do plotowania
     * \param[in] lon - szerokość geograficzna [stopnie].
     * \param[in] lat - długość geograficzna [stopnie].
     */
    void getPath(float &lon, float &lat);

public slots:

    /*!
     * \brief Wypisuje dane otrzymane z serwera do odpowiednich etykiet na głównym ekranie.
     *
     * \param[in] lon - szerokość geograficzna [stopnie].
     * \param[in] lat - długość geograficzna [stopnie].
     * \param[in] vel - prędkość stacji [km\h]
     * \param[in] alt - wysokość nad powierzchnią Ziemii [km]
     */
    void setInfo(float &lon, float &lat, int& vel, float &alt);

    /*!
     * \brief Definiuje funkcjonalność wykresu położenia
     *
     * \param[in] lon - szerokość geograficzna [stopnie].
     * \param[in] lat - długość geograficzna [stopnie].
     */
    void setISSChart(float &lon, float &lat);

    /*!
     * \brief Definiuje funkcjonalność wykresu wysokości
     *
     * \param[in] time - znak czasu [s].
     * \param[in] alt - wysokość nad powierzchnią Ziemii [km].
     */
    void setAltChart(int &time, float &alt);

private slots:

    /*!
     * \brief Uruchamianie widgetu
     *
     * Po wciśnięciu przycisku ukazuje się okno,
     * w którym można sprawdzić godziny przelotów nad
     * danym punktem.
     */
    void on_pushButton_Dialog_clicked();

    /*!
     * \brief Wysuwanie/chowanie panelu bocznego
     *
     * Po wciśnięciu przycisku wysuwa/chowa się boczny
     * panel z dalszymi informacjami.
     *
     * \param[in] checked - czy przycisk zpstał wciśnięty.
     */
    void on_pushButton_Panel_clicked(bool checked);

    /*!
     * \brief Zmiana widoku na głównym ekranie
     *
     * Wciśnięcie przycisku powoduje zmianę widoku z
     * przybliżonego na stację na widok mapy świata.
     * \param[in] checked - czy przycisk zpstał wciśnięty.
     */
    void on_pushButton_View_clicked(bool checked);

    /*!
     * \brief Pokazanie trasy
     *
     * Wciśnięcie przycisku pozwala na załączanie lub
     * wyłączanie wykresu przebytej trasy.
     * \param[in] checked - czy przycisk zpstał wciśnięty.
     */
    void on_pushButton_Path_clicked(bool checked);

    /*!
     * \brief  Środkowanie widoku
     *
     * Użytkownik posiada możliwość swobodnego poruszani się
     * głównym ekranie. Może przesuwać w osi x i y oraz oddalać i przybliżać.
     * Jeśli chce wrócić spowrotem to wycentrowanego widoku na stację,
     * wystarczy, że wciśnie ten przycisk.
     */
    void on_pushButton_Center_clicked();

    /*!
     * \brief Kliknięcie myszki
     *
     * Informuje o kliknięciu kursorem na wykresie.
     * \param[in] event - mouse event.
     */
    void clickedGraph(QMouseEvent *event);

    /*!
     * \brief Zamknięcie okna
     *
     * Kliknięcie przycisku powoduje zamknięcie okna
     * dialogowego.
     */
    void on_close_button_clicked();

protected:
    void resizeEvent(QResizeEvent *event);

private:
    Ui::MainWindow *ui;
    Worker *obiektISS, *obiektPath;
    QGraphicsScene *scene;
    QPixmap *pixISS, *pixEarth;
    QCPScatterStyle *myScatterISS;
    QGraphicsOpacityEffect *op, *op1;

    QVector<double>    qv_x, qv_y,
                    subv_x1, subv_y1,
                    vec_ISSx, vec_ISSy,
                    h_vec_ISSx, h_vec_ISSy,
                    sVec_ISSx, sVec_ISSy;

    int a = 0, b = 0, time_init = 0, space = 0;

    QPropertyAnimation *animButton;
    QPropertyAnimation *animation;
    QCPItemPixmap *backPixmap8;

    /*!
     * \brief Czy przycisk widoku został wciśnięty
     */
    bool viewISS = false;

    /*!
     * \brief Czy przycisk ścieżki został wciśnięty
     */
    bool pathISS = false;

    /*!
     * \brief Czy mapa została kliknięta przez kursos
     */
    bool mousePress = false;

    /*!
     * \brief Zmienna pomocnicza do zmiany grafiki na wykresie wysokości
     */
    int liczAlt = 1;

    //bool AnimOut = false;

    //bool AnimIn = false;

    //int tescik = 0;
};
#endif // MAINWINDOW_HH
