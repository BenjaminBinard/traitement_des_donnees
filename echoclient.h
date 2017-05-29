/**
* \file echoclient.h
* Le fichier d'en-tête correspondant à la classe \b EchoClient.
*/

#ifndef ECHOCLIENT_H
#define ECHOCLIENT_H

//recuperation
#include <QtCore/QObject>
#include <QtWebSockets/QWebSocket>
#include <iostream>
#include <QtCore/QDebug>
//traitement
#include <string>
#include <sstream>
#include <bitset>
#include <cstdlib>
#include <QDateTime>
using namespace std;
//envoi mysql
#include "database.h"
/**
* La classe \b DataBase est incluse dans ce fichier car une instance est un attribut de \b EchoClient (afin d'envoyer les données)
*/
class DataBase;

/**
* \class EchoClient echoclient.h
* La classe \b EchoClient est l'ensemble des fonctionnalités permettant de récupérer une trame et de la traiter.
*/
class EchoClient : public QObject
{
    Q_OBJECT
public:
  explicit EchoClient(const QUrl &url, /*const QUrl &url2,*/ bool debug = false, QObject *parent = Q_NULLPTR);
	void traitementChambre(QString message);
  void traitementUtilisateur(QString message);
	int decodeMTHO2(QString mtho2);
	int decodeHumidity(QString qstr_humidity);
	QString decodeTime(double dtime);

  //parametres bdd setters(askers)
  string askBddIp();
  string askBddLogin();
  string askBddPassword();
  string askIdRoom();

  //getters
  string getBddIp();
  string getBddLogin();
  string getBddPassword();
  string getIdRoom();

Q_SIGNALS:
    void closed();

private Q_SLOTS:
    void onConnected();
    void onTextMessageReceived(QString message);

private:
    QWebSocket m_webSocket;
    /*QWebSocket u_webSocket;*/
    /*QUrl u_url;*/
    QUrl m_url;
    bool m_debug;

    DataBase * isenlab_db;
    string id_room;

    string bdd_complete_path;
    string bdd_login;
    string bdd_pass;
};

#endif // ECHOCLIENT_H
