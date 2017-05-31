/**
* \file echoclient.h
* \brief Le fichier d'en-tête de la classe \b DataBase.
* \author Steve Corre et Benjamin Binard
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
* \brief La classe \b DataBase sert à intéragir avec la base de données.
* \details Ses fonctionnalités consistent à envoyer les valeurs reçues et traitées dans \b EchoClient dans la base de données,
* mais aussi à récupérer les données de seuils pour les comparer avec les valeurs de la trame actuelle.
* Si un dépassement est constaté, une alerte sera créée et envoyée dans l'entité correspondante de la base de données.
* \bug L'envoi de mails aurait pû faire l'objet d'une méthode de cette classe mais la bibliotheque choisie n'a pas permis cela.
*/
class DataBase
{
public:
  DataBase(string path, string login, string pass, QString time, int temperature, int humidite, QString user, QString four, double CO2, int chute, QString tv, QString pas, string id_room);
  /**
  * \fn DataBase
  * \brief Le constructeur de la classe \b DataBase.
  * \details En fonction des données qui lui sont transmises, il remplira l'entité \b VALUE de la base de données.
  * Si les quatres valeurs numériques de la trame des chambres sont à 0, il va gérer les données utilisateur (nombre de pas...)
  * Dans le cas contraire, ce sont les données des chambres qui lui sont envoyées.
  * Dans les deux cas, une requête INSERT INTO VALUE(...) VALUES(...) sera envoyée à la base de données.
  * \param path L'adresse de la base de données.
  * \param login L'identifiant pour se connecter à al base de données.
  * \param pass Le mot de passe pour se connecter à la base de données.
  * \param time La date et l'heure
  * \param temperature La valeur de la temperature.
  * \param humidite La valeur de l'humidité en %
  * \param user L'adresse mail de l'utilisateur de la pièce.
  * \param four La valeur associée à la mise en marche du four.
  * \param CO2 La valeur du taux de CO2 en ppm.
  * \param chute La valeur associée à l'éventuelle chute de l'utilisateur
  * \param tv La valeur associée à l'allumage de la télévision
  * \param pas Le nombre de pas effectués par l'utilisateur pendant la dernière heure.
  * \param id_room Le numero de la pièce.
  */
  ~DataBase();
  void Comparaison(string path, string login, string pass, QString time, int temperature, int humidite, QString user, QString four, double CO2, int chute, QString tv, string id_room);
  /**
  * \fn Comparaison
  * \details Cette méthode consiste à envoyer une requête SELECT ... FROM ... WHERE pour récupérer les seuils propres à chaque capteur.
  * Les valeurs passées en paramètres sont ensuite comparées avec leurs seuils respectifs, dès qu'un dépassement est constaté, un appel est lancé à la méthode createAlert().
  * \param path L'adresse de la base de données.
  * \param login L'identifiant pour se connecter à al base de données.
  * \param pass Le mot de passe pour se connecter à la base de données.
  * \param time La date et l'heure.
  * \param temperature La valeur de la temperature.
  * \param humidite La valeur de l'humidité en %.
  * \param user L'adresse mail de l'utilisateur de la pièce.
  * \param four La valeur associée à la mise en marche du four.
  * \param CO2 La valeur du taux de CO2 en ppm.
  * \param chute La valeur associée à l'éventuelle chute de l'utilisateur.
  * \param tv La valeur associée à l'allumage de la télévision.
  */
  void createAlert(string path, string login, string pass, QString user, QString time, int type, int sensDepassement, int value, string id_room);
  /**
  * \fn createAlert
  * \brief Cette méthode peut être appelée par \b Comparaison() dans la cas de dépassement d'un seuil.
  * \details En fonction du type d'alerte (ou plutôt du capteur qui la déclenche), la description de l'alerte changera.
  * une requête INSERT INTO est ensuite envoyée pour ajouter les données liées à l'alerte dans l'entité ALERTE de la base de données.
  * \param path L'adresse de la base de données.
  * \param login L'identifiant pour se connecter à al base de données.
  * \param pass Le mot de passe pour se connecter à la base de données.
  * \param time La date et l'heure.
  * \param type Le type de capteur concerné.
  * \param sensDepassement Une variable qui permet de savoir si c'est le seuil haut ou bas qui a été dépassé.
  * \param value La valeur envoyée par le capteur.
  * \param id_room Le numero de la pièce.
  */
private:
};


#endif
