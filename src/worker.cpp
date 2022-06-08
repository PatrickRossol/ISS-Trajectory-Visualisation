#include "worker.hh"



Worker::Worker(QObject *parent) : QObject(parent)
{

}

void Worker::parseJSON_ISS(QString reply)
{
     QString test = "[" + reply + "]";

     QJsonParseError jsonError;
     QJsonDocument flowerJson = QJsonDocument::fromJson(test.toUtf8(),&jsonError);

     if (jsonError.error != QJsonParseError::NoError)
         qDebug() << jsonError.errorString();

     QList<QVariant> list = flowerJson.toVariant().toList();
     QMap<QString, QVariant> map = list[0].toMap();

     this->longitudeISS = map["longitude"].toFloat();
     this->latitudeISS = map["latitude"].toFloat();
     this->velocityISS = map["velocity"].toInt();
     this->altitudeISS = map["altitude"].toFloat();
     this->timestampISS = map["timestamp"].toInt();

     emit initPath(this->timestampISS);

     if(cos)
     {
         emit receivedInfoCoords(this->longitudeISS, this->latitudeISS, this->velocityISS, this->altitudeISS);
         emit receivedInfoChart(this->timestampISS, this->altitudeISS);

         delay();
         get("https://api.wheretheiss.at/v1/satellites/25544");
     }

     this->cos = 1;
}

void Worker::parseJSON_Path(QString reply)
{
     QString test = reply;

     QJsonParseError jsonError;
     QJsonDocument flowerJson = QJsonDocument::fromJson(test.toUtf8(),&jsonError);

     if (jsonError.error != QJsonParseError::NoError)
         qDebug() << jsonError.errorString();

     QList<QVariant> list = flowerJson.toVariant().toList();
     QMap<QString, QVariant> map = list[0].toMap();

     this->longitudeISS = map["longitude"].toFloat();
     this->latitudeISS = map["latitude"].toFloat();

     emit reveivedInfoPath( this->longitudeISS, this->latitudeISS);
}

void Worker::get(QString location)
{
    qInfo() << "Getting from server...";
    QNetworkReply* reply = manager.get(QNetworkRequest(QUrl(location)));
    connect(reply, &QNetworkReply::readyRead, this,&Worker::readyRead);
}

void Worker::get_Path(QString location)
{
    qInfo() << "Getting from server...Path";
    QNetworkReply* reply = manager.get(QNetworkRequest(QUrl(location)));
    connect(reply, &QNetworkReply::readyRead, this,&Worker::readyRead_Path);
}

void Worker::readyRead()
{
   qInfo() << "ReadyRead";

   QString strReply;
   QNetworkReply* reply = qobject_cast<QNetworkReply*>(sender());

   if(reply)
   {
       strReply = reply->readAll();
       qDebug() << strReply;
   }

    parseJSON_ISS(strReply);
}

void Worker::readyRead_Path()
{
   qInfo() << "ReadyRead_Path";

   QString strReply;
   QNetworkReply* reply = qobject_cast<QNetworkReply*>(sender());

   if(reply)
   {
       strReply = reply->readAll();
       qDebug() << strReply + "Path";
   }

    parseJSON_Path(strReply);
}
/*
int mult = 1;
int offset= 0;
int size_xc = x.size();

for(int i = 0; x[size_x-i] == x[0]; i++)
{
    x[size_x-i] -= offset;

    if((x[size_x-i-1] > 0) && (x[size_x-i] < 0))
        mult++;

    offset = 360 * mult;
}*/


