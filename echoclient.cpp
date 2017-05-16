#include "echoclient.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>
#include <QJsonArray>
#include <QJsonObject>
#include <QtCore/QDebug>
#include <string>

QT_USE_NAMESPACE

EchoClient::EchoClient(const QUrl &url, bool debug, QObject *parent) :
    QObject(parent),
    m_url(url),
    m_debug(debug)
{
    if (m_debug)
        qDebug() << "WebSocket server:" << url;
    connect(&m_webSocket, &QWebSocket::connected, this, &EchoClient::onConnected);
    //connect(&m_webSocket, &QWebSocket::disconnected, this, &EchoClient::closed);
    m_webSocket.open(QUrl(url));
}

void EchoClient::onConnected()
{
    if (m_debug)
        qDebug() << "WebSocket connected";
    connect(&m_webSocket, &QWebSocket::textMessageReceived,
            this, &EchoClient::onTextMessageReceived);
    m_webSocket.sendTextMessage(QStringLiteral("Hello, world!"));
}

void EchoClient::onTextMessageReceived(QString message)
{
    traitement(message);
}

int EchoClient::decodeMTHO2(QString mtho2)
{
//-------------------------------variables
  string globalstring = mtho2.toStdString();
  stringstream sstmsb = globalstring.substr(0, 8);
  stringstream sstlsb = globalstring.substr(8, 16);
  string hexatmsb;
  string hexatlsb;
  string hexastr;
  int temp;

//---------------------------------conversion bin-string ==> hex-sstream
  bitset<8> setM(tmsb);
  sstmsb << hex << setM.to_ulong() << endl;

  bitset<8> setL(tlsb);
  sstlsb << hex << setL.to_ulong() << endl;


//----------------------------------verification taille chaines(sachant que cette chaine est plus grande de 1 charactere)
  if (sstmsb.str().length() == 2)
  {
    hexatmsb = '0' + sstmsb.str();
  }
  else
  {
    hexatmsb = sstmsb.str();
  }

  if (sstlsb.str().length() == 2)
  {
    hexatlsb = '0' + sstlsb.str();
  }
  else
  {
    hexatlsb = sstlsb.str();
  }

//----------------------------------concatenation (et suppression des characteres mysteres)
hexatmsb = hexatmsb.substr(0, hexatmsb.length()-1);
hexatlsb = hexatlsb.substr(0, hexatlsb.length()-1);
hexastr = hexatmsb + hexatlsb;
cout << hexastr << ' ' << hexastr.size() << endl;

//-----------------------------------conversion hexa-str ==> decimal-int

temp = strtol(hexastr.c_str(), 0, 16);
//cout << temp << endl;
//-----------------------------------------------------------------
  return temp;
}

void EchoClient::traitement(QString message){
	//qDebug()<<message;
	QJsonDocument jsonResponse = QJsonDocument::fromJson(message.toUtf8());
	QJsonObject jsonObject = jsonResponse.object();
	
	QJsonValue chute1 = jsonObject.value(QString("FALL"));
	QJsonValue CO21=jsonObject.value(QString("CO2"));
	QJsonValue humidite1=jsonObject.value(QString("MTH02"));
	QJsonValue four1=jsonObject.value(QString("OVEN"));
	QJsonValue time1=jsonObject.value(QString("TIME"));
	QJsonValue tv1=jsonObject.value(QString("TV"));
	QJsonValue user1=jsonObject.value(QString("USER"));
	//qDebug()<<CO21<<chute1<<humidite1<<four1<<time1<<tv1<<user1;
	
	int chute=chute1.toInt();
	double CO2=CO21.toDouble();
	QString humidite=humidite1.toString();
	bool four=four1.toBool();
	double time=time1.toDouble();
	QString tv=tv1.toString();
	QString user=user1.toString();

	qDebug()<<"chute : "<<chute<<" CO2 : "<<CO2<<" humidite : "<<humidite<<" four : "<<four<<" time : "<<time<<" tv : "<<tv<<" Utilisateur : "<<user;
	int MTHO2=decodeMTHO2(humidite);
	qDebug()<<MTHO2;
}
