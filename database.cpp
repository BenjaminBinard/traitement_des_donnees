/**
* \file database.cpp
*/

#include "database.h"


DataBase::DataBase(string path, string login, string pass, QString time, int temperature, int humidite, QString user, QString four, double CO2, int chute, QString tv, QString pas, string id_room)
{

  /*ici on gere les données du traitment utilisateur*/
  /*Que ce soit pour la chambre ou l'utilisateur, on utilise la même classe*/
  /*Pour l'utilisateur, cependant, en envoie toujours toutes les valeurs numériques à 0*/
  if (temperature == 0 && humidite == 0 && CO2 == 0 && chute == 0)
  {
    try
      {
    	sql::Driver *driver;
    	sql::Connection *con;
    	sql::Statement *stmt;

      driver = get_driver_instance();
  	  con = driver->connect(path, login, pass);
    	con->setSchema("isen_lab");

      stmt = con->createStatement();

      string request_insert = "INSERT INTO `VALUE` (`DTIME`, `CAR`, `NUM`, `ID_VALUE`, `TYPE`, `ID_ROOM`) VALUES (\'"+time.toStdString()+"\', \'"+user.toStdString()+"\', \'"+pas.toStdString()+"\', NULL, 8, \'"+id_room+"\');";
      stmt->execute(request_insert);

      //cout << request_insert << endl;


      delete con;
      delete stmt;
      }

      catch (sql::SQLException &e)
      {
       	 cout << "# ERR: SQLException in " << __FILE__;
      	 cout << __LINE__ << endl;
       	 cout << "# ERR: " << e.what();
       	 cout << " (MySQL error code: " << e.getErrorCode();
       	 cout << ", SQLState: " << e.getSQLState() << " )" << endl;
      }

      return;
  }
  /*sinon c'est qu'il s'agit des données du traitement chambre*/


  try
    {
  	sql::Driver *driver;
  	sql::Connection *con;
  	sql::ResultSet *res;
  	sql::PreparedStatement *pstmt;

    int itype;
    int num;
    QString car;
    QString dtime;



  	  driver = get_driver_instance();
  	  con = driver->connect(path, login, pass);
    	con->setSchema("isen_lab");


      for (int i = 1; i < 8; i++)
      {
        //Selection progressive des valeurs
        //remplissage des car ou num nulls
        if (i < 5)
        {
          car = "NULL";
        }
        else
        {
          num = 0;
        }
        //remplissage des valeurs transcrites
        switch(i)
        {
          case 1:
            num = chute;
            break;
          case 2:
            num = CO2;
            break;
          case 3:
            num = humidite;
            break;
          case 4:
            num = temperature;
            break;
          case 5:
            car = user;
            break;
          case 6:
            car = tv;
            break;
          case 7:
            car = four;
            break;
        }
        //le temps est le même dans tous les cas
        dtime = time;

        itype = i;
        QString qstype = QString::number(i);
        QString qsnum = QString::number(num);

        QString qsid_room = QString::fromStdString(id_room);

        //on execute la requete
        QString qrequest = "INSERT INTO `VALUE` (`DTIME`, `CAR`, `NUM`, `ID_VALUE`, `TYPE`, `ID_ROOM`) VALUES (\'"+dtime+"\', \'"+car+"\', \'"+qsnum+"\', NULL, \'"+qstype+"\', \'"+qsid_room+"\');";

        //qDebug() << qrequest << endl;
        pstmt = con->prepareStatement(qrequest.toStdString());
        res = pstmt->executeQuery();
      }




    delete con;
    delete pstmt;
    delete res;

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

void DataBase::Comparaison(string path, string login, string pass, QString time, int temperature, int humidite, QString user, QString four, double CO2, int chute, QString tv, string id_room)
{
  try
    {
  	sql::Driver *driver;
  	sql::Connection *con;
  	sql::Statement *stmt;
  	sql::ResultSet *res;

    driver = get_driver_instance();
    con = driver->connect(path, login, pass);
    con->setSchema("isen_lab");

    QString numberi;
    int value;
    stmt = con->createStatement();

    for (int i = 1; i < 8; i++)
    {

      numberi = QString::number(i);

      string request_high = "SELECT `THRESHOLD_HIGH` FROM `SENSOR` WHERE `TYPE` = " + numberi.toStdString() + " AND `ID_ROOM` = "+ id_room +";";
      string request_low = "SELECT `THRESHOLD_LOW` FROM `SENSOR` WHERE `TYPE` = " + numberi.toStdString() + " AND `ID_ROOM` = "+ id_room +";";


      switch (i)
      {
        case 1:
          value = chute;
          break;
        case 2:
          value = CO2;
          break;
        case 3:
          value = humidite;
          break;
        case 4:
          value = temperature;
          break;
        case 5://On renvoie toujours 0, pas d'alerte quand un utilisateur rentre simplement dans la pièce
          value = 0;
          break;
        case 6:
          if (tv.toStdString().compare("off") != 0)
          {
            value = 1;
          }
          else
          {
            value = 0;
          }
          break;
        case 7:
          if (four.toStdString().compare("FALSE") != 0)
          {
            value = 1;
          }
          else
          {
            value = 0;
          }
          break;
      }


      //depassement du seuil haut
      res = stmt->executeQuery(request_high);
      res->next();
      //cout << stoi(res->getString("THRESHOLD_HIGH")) << endl;
      if (value >= stoi(res->getString("THRESHOLD_HIGH")))
      {
        //on transmet si ça depasse en haut ou en bas(int)
        //on transmet la value
        int sensDepassement = 1;
        createAlert(path, login, pass, user, time, i, sensDepassement, value, id_room);
      }
      else
      {
        //depassement du seuil bas
        res = stmt->executeQuery(request_low);
        res->next();
        if (value <= stoi(res->getString("THRESHOLD_LOW")))
        {
          int sensDepassement = -1;
          createAlert(path, login, pass, user, time, i, sensDepassement, value, id_room);
        }
      }

    }

    delete con;
    delete stmt;
    delete res;
  }


  catch (sql::SQLException &e)
  {
     	 cout << "# ERR: SQLException in " << __FILE__;
    	 cout << __LINE__ << endl;
     	 cout << "# ERR: " << e.what();
     	 cout << " (MySQL error code: " << e.getErrorCode();
     	 cout << ", SQLState: " << e.getSQLState() << " )" << endl;
  }

  //qDebug() << "Fin de fonction Comparaison()" << endl;
}




void DataBase::createAlert(string path, string login, string pass, QString user, QString time, int type, int sensDepassement, int value, string id_room)
{
  cout << "Alerte !" << endl;

  string mail_user = user.toStdString();
  cout << mail_user << endl;
  string dtime = time.toStdString();
  string detail = " ";

    switch (type)//on construit la chaine 'detail' en fonction du type d'alerte (chute, haute temperature, etc...)
    {
      case 1:
        detail += "L'utilisateur possedant l'addresse " + mail_user + " a chu.";
        break;
      case 2:
        detail += "Le taux de CO2 est dangereusement haut ! (" + to_string(value) + "ppm)";
        break;
      case 3:
        if (sensDepassement == 1)
        {
          detail += "Humidité trop importante ! (" + to_string(value) + ")";
        }
        else
        {
          detail += "Humidité critiquement basse ! (" + to_string(value) + ")";
        }
        break;
      case 4:
        if (sensDepassement == 1)
        {
          detail += "La temperature est trop élevée ! (" + to_string(value) + "°C)";
        }
        else
        {
          detail += "La temperature est trop basse ! (" + to_string(value) + "°C)";
        }
        break;
      case 5:
        detail += "L'utilisateur possedant l'addresse " + mail_user + "est présent.";//inutilisé
        break;
      case 6:
        detail += "La télévision est allumée !";
        break;
      case 7:
        detail += "Le four est en marche !";
        break;
    }
    cout << detail << endl;

  //Dans la BDD, table ALERT : id_alert(), id_room, mail_user, dtime, detail
  try
    {
  	sql::Driver *driver;
  	sql::Connection *con;
  	sql::Statement *stmt;

    driver = get_driver_instance();
    con = driver->connect(path, login, pass);
    con->setSchema("isen_lab");

    stmt = con->createStatement();


    string request_insert = "INSERT INTO `ALERT`(`ID_ROOM`, `MAIL_USER`, `DTIME`, `DETAIL`) VALUES(\'"+id_room+"\', \'"+mail_user+"\', \'"+dtime+"\', \'"+detail+"\');";
    //cout << request_insert << endl;
    stmt->execute(request_insert);

    delete con;
    delete stmt;

    }


  catch (sql::SQLException &e)
  {
     	 cout << "# ERR: SQLException in " << __FILE__;
    	 cout << __LINE__ << endl;
     	 cout << "# ERR: " << e.what();
     	 cout << " (MySQL error code: " << e.getErrorCode();
     	 cout << ", SQLState: " << e.getSQLState() << " )" << endl;
  }

  //Pour les mails, utilisation de la bibliotheque smtpclient créée par bluetiger9
  /*
  https://github.com/bluetiger9/SmtpClient-for-Qt/wiki/Examples#example1
  https://github.com/bluetiger9/SmtpClient-for-Qt
  */

  //des erreurs lors de la compilation avec cette partie

  /*SmtpClient smtp("smtp.isen-ouest.fr", 465, SmtpClient::SslConnection);

  smtp.setUser("elevecir2");
  smtp.setPassword("Cir2Project");

  MimeMessage message;
  message.setSender(new EmailAddress("steve.corre@isen-ouest.yncrea.fr", "Equipe Isen Lab"));
  message.addRecipient(new EmailAddress("steve.correlemeur@gmail.com", "Steve"));
  message.setSubject("Alerte - Service Isen Lab");

  MimeText text;
  text.setText(QString::fromStdString(detail));
  message.addPart(&text);

  smtp.connectToHost();
  smtp.login();
  smtp.sendMail(message);
  smtp.quit();*/
}
