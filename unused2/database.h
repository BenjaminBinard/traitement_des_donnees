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
#include "echoclient.h"


class DataBase
{
public:
  DataBase(QString time, int temperature, int humidite, QString user, QString four, double CO2, int chute, QString tv);
  ~DataBase();
  //void Comparaison(QString time, int temperature, int humidite, QString user, QString four, double CO2, int chute, QString tv);
  //createAlert();generer une chaine qu'on enverra par mail
private:
  /*QString address = "tcp://127.0.0.1:3306";
  QString identity = "equipe";
  QString password = "toor";*/
};


#endif
