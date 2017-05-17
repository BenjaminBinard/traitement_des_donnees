#include "echoclient.h"
#include <sstream>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>
#include <QJsonArray>
#include <QJsonObject>
#include <QtCore/QDebug>
#include <string>
#include <iostream>

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
//cout << hexastr << ' ' << hexastr.size() << endl;

//-----------------------------------conversion hexa-str ==> decimal-int

temp = strtol(hexastr.c_str(), 0, 16);
temp = temp - 400;
temp = temp / 10;
//cout << temp << endl;
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


void EchoClient::traitement(QString message){
	//qDebug()<<message;
	QJsonDocument jsonResponse = QJsonDocument::fromJson(message.toUtf8());
	QJsonObject jsonObject = jsonResponse.object();
	
	QJsonValue chute1 = jsonObject.value(QString("FALL"));
	QJsonValue CO21=jsonObject.value(QString("CO2"));
	QJsonValue MTHO21=jsonObject.value(QString("MTH02"));
	QJsonValue four1=jsonObject.value(QString("OVEN"));
	QJsonValue time1=jsonObject.value(QString("TIME"));
	QJsonValue tv1=jsonObject.value(QString("TV"));
	QJsonValue user1=jsonObject.value(QString("USER"));
	//qDebug()<<CO21<<chute1<<humidite1<<four1<<time1<<tv1<<user1;
	
	int chute=chute1.toInt();
	double CO2=CO21.toDouble();
	QString MTHO2=MTHO21.toString();
	bool four=four1.toBool();
	double time=time1.toDouble();
	QString tv=tv1.toString();
	QString user=user1.toString();

	qDebug()<<"chute : "<<chute<<" CO2 : "<<CO2<<" humidite : "<<MTHO2<<" four : "<<four<<" time : "<<time<<" tv : "<<tv<<" Utilisateur : "<<user;
	int temperature=decodeMTHO2(MTHO2);
	int humidite=decodeHumidity(MTHO2);
	qDebug() <<"temperature:"<<temperature<<endl<<"humidite:"<<humidite<<endl;

//si on le fait directement on instancie ce qui concerne mysql
//puis on fait nos statements
//voire meme des prepared statements
try
  {  
	sql::Driver *driver;
	sql::Connection *con;
	sql::Statement *stmt;
	sql::ResultSet *res;
	sql::PreparedStatement *pstmt;

	driver = get_driver_instance();
	con = driver->connect("tcp://127.0.0.1:3306", "equipe", "toor");
  	con->setSchema("isen_lab");

  	 stmt = con->createStatement();
	stmt->execute("INSERT INTO ROOM(ID_ROOM, IP, PORT) VALUES(2, '172.31.1.31', '2000')");
 	// res = stmt->executeQuery("SELECT 'Hello World!' AS _message");
 	 /*while (res->next()) 
	 {
   		 cout << "\t... MySQL replies: ";
   		 // Access column data by alias or column name
   		 cout << res->getString("_message") << endl;
   		 cout << "\t... MySQL says it again: ";
   	 	// Access column data by numeric offset, 1 is the first column
   	 	cout << res->getString(1) << endl;
  	}*/
	//pstmt = con->preparedStatement("INSERT INTO SENSOR
  delete res;
  delete stmt;
  delete con;

}
catch (sql::SQLException &e) 
{
 	 cout << "# ERR: SQLException in " << __FILE__;
	 cout << __LINE__ << endl;
 	 cout << "# ERR: " << e.what();
 	 cout << " (MySQL error code: " << e.getErrorCode();
 	 cout << ", SQLState: " << e.getSQLState() << " )" << endl;
}
}
