#ifndef ECHOCLIENT_H
#define ECHOCLIENT_H

//recuperation
#include <QtCore/QObject>
#include <QtWebSockets/QWebSocket>
#include <iostream>
//#include <Qtcore/QDebug>
//traitement
#include <string>
#include <sstream>
#include <bitset>
#include <cstdlib>
using namespace std;
//envoi mysql
#include "mysql_connection.h"
#include <cppconn/connection.h>
#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>

class EchoClient : public QObject
{
    Q_OBJECT
public:
    explicit EchoClient(const QUrl &url, bool debug = false, QObject *parent = Q_NULLPTR);
	void traitement(QString message);
	int decodeMTHO2(QString mtho2);
	int decodeHumidity(QString qstr_humidity);
Q_SIGNALS:
    void closed();

private Q_SLOTS:
    void onConnected();
    void onTextMessageReceived(QString message);

private:
    QWebSocket m_webSocket;
    QUrl m_url;
    bool m_debug;
};

#endif // ECHOCLIENT_H
