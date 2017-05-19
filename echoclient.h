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

class DataBase;

class EchoClient : public QObject
{
    Q_OBJECT
public:
  explicit EchoClient(const QUrl &url, bool debug = false, QObject *parent = Q_NULLPTR);
	void traitement(QString message);
	int decodeMTHO2(QString mtho2);
	int decodeHumidity(QString qstr_humidity);
	QString decodeTime(double dtime);
Q_SIGNALS:
    void closed();

private Q_SLOTS:
    void onConnected();
    void onTextMessageReceived(QString message);

private:
    QWebSocket m_webSocket;
    QUrl m_url;
    bool m_debug;
    DataBase * isenlab_db;
};

#endif // ECHOCLIENT_H
