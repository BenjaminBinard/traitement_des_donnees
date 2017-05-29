/**
* \file database.h
* Le fichier d'en-tête de la classe DataBase.
*/

#ifndef DATABASE_H
#define DATABASE_H

#include "mysql_connection.h"
#include <cppconn/connection.h>
#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>
#include <cppconn/prepared_statement.h>

#include <QtCore>
//#include <QtGui/QApplication>
#include "echoclient.h"
//#include "SmtpClient/src/SmtpMime"


/**
* \class DataBase database.h
* La classe \b DataBase sert à intéragir avec la base de données.
* Ses fonctionnalités consistent à envoyer les valeurs reçues et traitées dans \b EchoClient dans la base de données,
* mais aussi à récupérer les données de seuils pour les comparer avec les valeurs de la trame actuelle.
* Si un dépassement est constaté, une alerte sera créée et envoyée dans l'entité correspondante de la base de données.
* \bug L'envoi de mails aurait pû faire l'objet d'une méthode de cette classe mais la bibliotheque choisie n'a pas permis cela.
*/
class DataBase
{
public:
  DataBase(string path, string login, string pass, QString time, int temperature, int humidite, QString user, QString four, double CO2, int chute, QString tv, QString pas, string id_room);
  ~DataBase();
  void Comparaison(string path, string login, string pass, QString time, int temperature, int humidite, QString user, QString four, double CO2, int chute, QString tv, string id_room);
  void createAlert(string path, string login, string pass, QString user, QString time, int type, int sensDepassement, int value, string id_room);
  //int getIdRoom(string path, string login, string pass, QString user, string id_room);
private:
};


#endif
