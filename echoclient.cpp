/**
* \file echoclient.cpp
*/


#include "echoclient.h"
#include "database.h"
#include <sstream>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>
#include <QJsonArray>
#include <QJsonObject>
#include <string>
#include <iostream>

QT_USE_NAMESPACE

EchoClient::EchoClient(const QUrl &url, bool debug, QObject *parent) :
    QObject(parent),
    m_url(url),
    m_debug(debug)
{
    this->bdd_complete_path = askBddIp();
    this->bdd_login = askBddLogin();
    this->bdd_pass = askBddPassword();
    this->id_room = askIdRoom();
    if (m_debug)
        qDebug() << "WebSocket server:" << url;
    connect(&m_webSocket, &QWebSocket::connected, this, &EchoClient::onConnected);

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
  //qDebug() << "TAILLE DE LA TRAME " << message.length() << endl;
  if (message.length() > 100)
  {
    qDebug() << "TRAITEMENT CHAMBRE" << endl;
    traitementChambre(message);
  }
  else
  {
    qDebug() << "TRAITEMENT UTILISATEUR" << endl;
    traitementUtilisateur(message);
  }
}

int EchoClient::decodeMTHO2(QString mtho2)
{
//-------------------------------variables
  string globalstring = mtho2.toStdString();
  string tmsb=globalstring.substr(0, 8);
  string tlsb=globalstring.substr(8, 8);
  stringstream sstmsb;
  stringstream sstlsb;
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

//-----------------------------------conversion hexa-str ==> decimal-int

temp = strtol(hexastr.c_str(), 0, 16);
temp = temp - 400;
temp = temp / 10;

//-----------------------------------------------------------------
  return temp;
}


int EchoClient::decodeHumidity(QString qstr_humidity)
{
	string str_humidity = qstr_humidity.toStdString();
	str_humidity = str_humidity.substr(16,8);
	int humidity = stoi(str_humidity, 0, 2);
	return humidity;
}


QString EchoClient::decodeTime(double dtime)
{
  long long int ssitime = static_cast<long long int>(dtime);
  string stime = to_string(ssitime);
  stime = stime.substr(0, stime.length()-3);
  int itime = stoi(stime);
  QDateTime timestamp;
  timestamp.setTime_t(itime);
  //qDebug() << timestamp << endl;
  QString time = timestamp.addSecs(7200).toString(Qt::ISODate);
  //qDebug() << time << endl;
  return time;
}
//if not yet defined ...
string EchoClient::askBddIp()
{
  string bdd_ip;
  string bdd_port;

  cout << "Quelle est l'addresse utilisée pour se connecter à la base de données ? (ex:127.0.0.1)" << endl;
  cin >> bdd_ip;
  //cout << bdd_ip << endl;

  cout << "le port utilisé ? (ex:3306)" << endl;
  cin >> bdd_port;
  //cout << bdd_port << endl;

  string bdd_complete_path = "tcp://" + bdd_ip + ":" + bdd_port;
  return bdd_complete_path;
}

string EchoClient::askBddLogin()
{
  string bdd_login;
  cout << "Identifiant : (ex:equipe)" << endl;
  cin >> bdd_login;
  //cout << bdd_login << endl;
  return bdd_login;
}

string EchoClient::askBddPassword()
{
  string bdd_pass;
  cout << "Mot de passe : (ex:toor)" << endl;
  cin >> bdd_pass;
  //cout << bdd_pass << endl;
  return bdd_pass;
}

string EchoClient::askIdRoom()
{
  string id_room;
  cout << "Quel est le numero  du labo ? (1 ou 2)" << endl;
  cin >> id_room;
  return id_room;
}


void EchoClient::traitementChambre(QString message)
{
	QJsonDocument jsonResponse = QJsonDocument::fromJson(message.toUtf8());
	QJsonObject jsonObject = jsonResponse.object();

	QJsonValue chute1 = jsonObject.value(QString("FALL"));
	QJsonValue CO21=jsonObject.value(QString("CO2"));
	QJsonValue MTHO21=jsonObject.value(QString("MTH02"));
	QJsonValue four1=jsonObject.value(QString("OVEN"));
	QJsonValue time1=jsonObject.value(QString("TIME"));
	QJsonValue tv1=jsonObject.value(QString("TV"));
	QJsonValue user1=jsonObject.value(QString("USER"));

  double dtime = time1.toDouble();
  QString time = decodeTime(dtime);

	int chute = chute1.toInt();
	double CO2 = CO21.toDouble();
  QString tv=tv1.toString();
	QString user=user1.toString();
	QString MTHO2 = MTHO21.toString();
	bool four2 = four1.toBool();
  QString four;

  if(four2)
  {
    four="TRUE";
  }
  else
  {
    four="FALSE";
  }

	int temperature = decodeMTHO2(MTHO2);
	int humidite = decodeHumidity(MTHO2);

	qDebug() << "Chute : " << chute << endl << "CO2 : " << CO2 << endl;
	qDebug() << "Capteur MTHO2 : " << MTHO2 << endl << "Four : " << four << endl;
	qDebug() << "Tv : " << tv << endl << "Utilisateur : " << user << endl;
	qDebug() << "Temperature : " << temperature << endl << "Humidite : " << humidite << endl;
	qDebug() << "Date et Heure : " << time << endl;


//PARTIE MYSQL

  isenlab_db = new DataBase(getBddIp(), getBddLogin(), getBddPassword(), time, temperature, humidite, user, four, CO2, chute, tv, "", getIdRoom());
  isenlab_db->Comparaison(getBddIp(), getBddLogin(), getBddPassword(), time, temperature, humidite, user, four, CO2, chute, tv, getIdRoom());

}

void EchoClient::traitementUtilisateur(QString message)
{
  QJsonDocument jsonResponse = QJsonDocument::fromJson(message.toUtf8());
	QJsonObject jsonObject = jsonResponse.object();

  QJsonValue time1=jsonObject.value(QString("TIME"));
	QJsonValue pas1=jsonObject.value(QString("STEP"));
	QJsonValue user1=jsonObject.value(QString("USER"));

  double dtime = time1.toDouble();
  QString time = decodeTime(dtime);

  QString user = user1.toString();

  int pas2 = pas1.toInt();
  QString pas = QString::number(pas2);

  qDebug() << "Date et Heure : " << time << endl;
  qDebug() << "Utilisateur : " << user << endl;
  qDebug() << "Nombre de pas dans l'heure : " << pas << endl;

  isenlab_db = new DataBase(getBddIp(), getBddLogin(), getBddPassword(), time, 0, 0, user, "", 0, 0, "", pas, getIdRoom());

}


string EchoClient::getBddIp()
{
  return bdd_complete_path;
}

string EchoClient::getBddLogin()
{
  return bdd_login;
}

string EchoClient::getBddPassword()
{
  return bdd_pass;
}

string EchoClient::getIdRoom()
{
  return id_room;
}
