#ifndef WORKER_H
#define WORKER_H

/*!
* \file
* \brief Definicja klasy Worker
*
* Plik zawiera definicję klasy Worker, która
* jest odpwiedzialana za obsługę wszystkich informacji
* przychodzących z serwera ISSTracker.
*/

#include <QObject>
#include <QDebug>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <windows.h>
#include <QCoreApplication>

class Worker  : public QObject
{
    Q_OBJECT
public:
    explicit Worker(QObject *parent = nullptr);

    /*!
     * \brief Wstrzymanie na czas 1s
     *
     * Pozwala na wstrzymanie wysyłania zapytania do serwera
     * na czas 1s bez zatrzymywania działania programu.
     */
    void delay()
    {
        QTime dieTime= QTime::currentTime().addSecs(1);
        while (QTime::currentTime() < dieTime)
            QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
    }

    /*!
     * \brief Parsowanie stringa
     *
     * Parsuje otrzymanego z serwera stringa w formacie JSON,
     * aby móc wyciagnąć z niego dane współrzędnych aktualnego
     * położenia stacji.
     * \param[in] reply - odpowiedź z serwa w formacie JSON.
     */
    void parseJSON_ISS(QString reply);

    /*!
     * \brief Parsowanie stringa
     *
     * Parsuje otrzymanego z serwera stringa w formacie JSON,
     * aby móc wyciagnąć z niego dane już przebytej ścieżki
     * przez stację.
     * \param[in] reply - odpowiedź z serwa w formacie JSON.
     */
    void parseJSON_Path(QString reply);

public slots:

    /*!
     * \brief Wysyłanie żądania do serwera
     * Metoda wysyła żądanie do serwera, żeby móc pobrane dane
     * aktualnego położenia stacji.
     *
     * \param[in] location - adres URL, z którego chcemy pobrać dane.
     */
    void get(QString location);

    /*!
     * \brief Wysyłanie żądania do serwera
     * Metoda wysyła żądanie do serwera, żeby móc pobrane dane
     * już przebytej trasy.
     *
     * \param[in] location -adres URL, z którego chcemy pobrać dane.
     */
    void get_Path(QString location);

signals:

    /*!
     * \brief Sygnał otrzymania współrzędnych
     * Przekazuje informacje o ISS, aby móc je odczytać
     * na animacji do wypisywania danych w oknie.
     *
     * \param[in] lon - szerokość geograficzna [stopnie].
     * \param[in] lat - długość geograficzna [stopnie].
     * \param[in] vel - prędkość stacji [km/h].
     * \param[in] alt - wysokość nad powierzchnią Ziemii [km].
     */
    void receivedInfoCoords(float &lon, float &lat, int& vel, float &alt);


    /*!
     * \brief Sygnał z danymi do wykresu
     * Przekazuje informacje o ISS, aby móc je odczytać
     * na wykresie wysokości w panelu bocznym.
     *
     * \param[in] time - znak czasu [s].
     * \param[in] alt - wysokość nad powierzchnią Ziemii [km].
     */
    void receivedInfoChart(int &time, float &alt);

    /*!
     * \brief Sygnał przekazania przbytej trasy
     * Przekazuje informacje przbytej trasy ISS, aby móc je
     * odczytać na głownym wykresie.
     *
     * \param[in] time - znak czasu [s].
     * \param[in] alt - wysokość na powierzchnią Ziemii [km].
     */
    void reveivedInfoPath(float &lon, float &lat);

    /*!
     * \brief Inicjalizacja przebytej trasy
     * Inicjalizuje nawiązanie połączenia z serwerem
     * aby móc odebrać dane o przebytej trasie na podstawie
     * aktualnego znaku czasu.
     *
     * \param[in] time - znak czasu [s].
     */
    void initPath(int &time);

private slots:
    void readyRead();

    void readyRead_Path();


private:
    QNetworkAccessManager manager;

    /*!
     * \param[out] latitudeISS - długość geograficzna [stopnie].
     */
    float latitudeISS;

    /*!
     * \param[out] longitudeISS - szerokość geograficzna [stopnie].
     */
    float longitudeISS;

    /*!
     * \param[out] altitudeISS - wysokość nad powierzchnią Ziemii [km].
     */
    float altitudeISS;

    /*!
     * \param[out] velocityISS - prędkość stacji [km/h].
     */
    int velocityISS;

    /*!
     * \param[out] timestampISS - znak czasu [s].
     */
    int timestampISS;

    /*!
     * \param[in] cos - zmienna pomocnicza do jednokrotnego wywołania slotu.
     */
    bool cos = 0;
};

#endif // WORKER_H
