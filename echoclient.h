/**
* \file echoclient.h
* \brief Le fichier d'en-tête correspondant à la classe \b EchoClient.
* \author Steve Corre et Benjamin Binard
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
* \brief La classe \b EchoClient est l'ensemble des fonctionnalités permettant de récupérer une trame et de la traiter.
*/
class EchoClient : public QObject
{
    Q_OBJECT
public:
  explicit EchoClient(const QUrl &url, bool debug = false, QObject *parent = Q_NULLPTR);
  /**
 * \fn explicit EchoClient(const QUrl &url, bool debug = false, QObject *parent = Q_NULLPTR)
 * \brief Constructeur de la classe EchoClient.
 *
 * \param &url
 * \param debug
 * \param *parent
 */
	void traitementChambre(QString message);
  /**
 * \fn void traitementChambre(QString message)
 * \brief Fonction qui sépare les éléments de la trame de la pièce, les traite et les envoie à la base de données.
 *
 * \param message La trame transmise.
 */
  void traitementUtilisateur(QString message);
  /**
 * \fn void traitementUtilisateur(QString message)
 * \brief Fonction qui sépare les éléments de la trame utilisateur, les traite et les envoie à la base de données.
 *
 * \param message La trame transmise.
 */
	int decodeMTHO2(QString mtho2);
  /**
 * \fn int decodeMTHO2(QString mtho2)
 * \brief Fonction qui extrait la temperature de la chaine envoyée par le capteur MTHO2.
 *
 * \param qstr_humidity Chaine binaire envoyée par le capteur MTHO2.
 * \return La temperature dans la pièce (int).
 */
	int decodeHumidity(QString qstr_humidity);
  /**
 * \fn int decodeHumidity(QString qstr_humidity)
 * \brief Fonction qui extrait le niveau d'humidité de la chaine envoyée par le capteur MTHO2.
 *
 * \param qstr_humidity Chaine binaire envoyée par le capteur MTHO2.
 * \return Le taux d'humidité de la pièce (int).
 */
	QString decodeTime(double dtime);
  /**
 * \fn QString decodeTime(double dtime)
 * \brief Fonction qui décode la timestamp entré et renvoie l'heure en format lisible.
 *
 * \param dtime Timestamp de la date et l'heure actuelle.
 * \return La date et l'heure en une chaine de caractères (QString).
 */

  //parametres bdd setters(askers)
  string askBddIp();
  /**
 * \fn string askBddIp()
 * \brief Fonction setter qui demande à l'utilisateur l'adresse IP et le port de la base de données.
 *
 * \return Adresse de la base de données (std::string).
 */
  string askBddLogin();
  /**
 * \fn string askBddLogin()
 * \brief Fonction setter qui demande à l'utilisateur son identifiant pour se connecter à la base de données.
 *
 * \return L'identifiant permettant de se connecter à la base de données (std::string).
 */
  string askBddPassword();
  /**
 * \fn string askBddPassword()
 * \brief Fonction setter qui demande à l'utilisateur le mot de passe pour se connecter à la base de données.
 *
 * \return Le mot de passe permettant de se connecter à la base de données (std::string).
 */
  string askIdRoom();
  /**
 * \fn string askIdRoom()
 * \brief Fonction setter qui demande à l'utilisateur la pièce à laquelle est associée l'addresse IP rentrée plus tôt.
 *
 * \return Le numero de la pièce surveillée (std::string).
 */

  //getters
  string getBddIp();
  /**
 * \fn string getBddIp()
 * \brief Fonction getter retournant l'addresse de la base de données sous forme de chaine de caractères.
 *
 * \return Adresse de la base de données (std::string).
 */
  string getBddLogin();
  /**
 * \fn string getBddLogin()
 * \brief Fonction getter retournant le login de la base de données sous forme de chaine de caractères.
 *
 * \return L'identifiant permettant de se connecter à la base de données (std::string).
 */
  string getBddPassword();
  /**
 * \fn string getBddPassword()
 * \brief Fonction getter retournant le mot de passe de la base de données sous forme de chaine de caractères.
 *
 * \return Mot de passe de la base de données (std::string).
 */
  string getIdRoom();
  /**
 * \fn string getIdRoom()
 * \brief Fonction getter retournant le numero de la pièce sous forme de chaine de caractères.
 *
 * \return Le numero de la pièce (std::string).
 */

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
