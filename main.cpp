/**
\file main.cpp
*/

#include <QtCore/QCoreApplication>
#include <QtCore/QCommandLineParser>
#include <QtCore/QCommandLineOption>
#include "echoclient.h"

/**
* \fn fonction main
* \details Le premier test verifie que l'addresse IP et le Port sur lesquels écouter son entrés au lancement du programme.
* Il s'agit ensuite de préparer l'url totale (avec l'IP et le Port) et de créer une instance de notre EchoClient.
*/
int main(int argc, char *argv[])
{
	if (argv[1] == NULL || argv[2] == NULL/* || argv[3] == NULL*/)
	{
		cout << "Pour lancer correctement le programme tapez './NomDuProgramme AddresseIpTrame PortChambre ou PortUtilisateur'" << endl;
		cout << "(Par exemple : ./echoclient 192.168.199.30 1000)" << endl;
		return 1;
	}

	QString * retour;
    QCoreApplication a(argc, argv);

    QCommandLineParser parser;
    parser.setApplicationDescription("QtWebSockets example: echoclient");
    parser.addHelpOption();

    QCommandLineOption dbgOption(QStringList() << "d" << "debug",
            QCoreApplication::translate("main", "Debug output [default: off]."));
    parser.addOption(dbgOption);
    parser.process(a);
    bool debug = parser.isSet(dbgOption);

		QString ip = argv[1];
		QString port = argv[2];
		/*QString port2 = argv[3];*/
		QString url = "ws://" + ip + ":" + port;
		/*QString url2 = "ws://" + ip + ":" + port2;*/
		qDebug() << url << endl;
		/*qDebug() << url2 << endl;*/

    EchoClient client(QUrl(url), debug);
		//EchoClient client(QUrl(QStringLiteral()), debug);
		//"ws://192.168.199.30:1000"
		//bdd : 127.0.0.1 3306 equipe toor


		//qDebug()<<client;
    //QObject::connect(&client, &EchoClient::closed, &a, &QCoreApplication::quit);

    return a.exec();
}
