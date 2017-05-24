#include "database.h"

DataBase::DataBase(QString time, int temperature, int humidite, QString user, QString four, double CO2, int chute, QString tv)
{
  try
    {
  	sql::Driver *driver;
  	sql::Connection *con;
  	sql::ResultSet *res;
  	sql::PreparedStatement *pstmt;

    int id_sensor;
    int num;
    QString car;
    QString dtime;



  	  driver = get_driver_instance();
  	  con = driver->connect("tcp://127.0.0.1:3306", "equipe", "toor");
    	con->setSchema("isen_lab");

    	//stmt = con->createStatement();
      //res = stmt->executeQuery("INSERT INTO `VALUE` (`DTIME`, `CAR`, `NUM`, `ID_VALUE`, `TYPE`) VALUES (\'dtime\', \'car\', \'num\', NULL, \'id_sensor\');");



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

        id_sensor = i;
        QString qsid_sensor = QString::number(id_sensor);
        QString qsnum = QString::number(num);

        //on execute la requete
        QString qrequest = "INSERT INTO `VALUE` (`DTIME`, `CAR`, `NUM`, `ID_VALUE`, `TYPE`) VALUES (\'"+dtime+"\', \'"+car+"\', \'"+qsnum+"\', NULL, \'"+qsid_sensor+"\');";

        qDebug() << qrequest << endl;
        //DECOMMENTER CES LIGNES POUR FAIRE REFONCTIONNER+++++++++++++++++++++++++++++++++++
        pstmt = con->prepareStatement(qrequest.toStdString());
        res = pstmt->executeQuery();
        //delete pstmt;
      }




    delete con;
    //ET CES LIGNES AUSSI+++++++++++++++++++++++++++++++++++
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

//aller dans la bdd pour récuperer les seuils
//les comparer avec nos valeurs
//si depassement ==> createAlert()
void DataBase::Comparaison(QString time, int temperature, int humidite, QString user, QString four, double CO2, int chute, QString tv)
{
  try
    {
  	sql::Driver *driver;
  	sql::Connection *con;
  	sql::Statement *stmt;
  	sql::ResultSet *res;

    driver = get_driver_instance();
    con = driver->connect("tcp://127.0.0.1:3306", "equipe", "toor");
    con->setSchema("isen_lab");

    QString numberi;
    int value;
    stmt = con->createStatement();

    for (int i = 1; i < 8; i++)
    {

      numberi = QString::number(i);

      string request_high = "SELECT `THRESHOLD_HIGH` FROM `SENSOR` WHERE `TYPE` = " + numberi.toStdString() + " AND `ID_ROOM` = 1;";
      string request_low = "SELECT `THRESHOLD_LOW` FROM `SENSOR` WHERE `TYPE` = " + numberi.toStdString() + " AND `ID_ROOM` = 1;";


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
        case 5://ON ENVOIE 0, POUR L'INSTANT PAS D'ALERTES QUAND CHUCK EST LA
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
        createAlert(user, time, i, sensDepassement, value);
      }
      else
      {
        //depassement du seuil bas
        res = stmt->executeQuery(request_low);
        res->next();
        //cout << stoi(res->getString("THRESHOLD_LOW")) << endl;
        if (value <= stoi(res->getString("THRESHOLD_LOW")))
        {
          int sensDepassement = -1;
          createAlert(user, time, i, sensDepassement, value);
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

  qDebug() << "Fin de fonction Comparaison()" << endl;
}



void DataBase::createAlert(QString user, QString time, int type, int sensDepassement, int value)
{
  qDebug() << "Une alerte" << endl;

  string id_room = QString::number(1).toStdString();
  string mail_user = user.toStdString();
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
          detail += "Le taux d'humidité est dangereusement haut ! (" + to_string(value) + "%)";
        }
        else
        {
          detail += "Le taux d'humidité est dangereusement bas ! (" + to_string(value) + "%)";
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
        detail += "L'utilisateur possedant l'addresse " + mail_user + "est présent.";//inutilisé pour l'instant
        break;
      case 6:
        detail += "La télévision est allumée !";
        break;
      case 7:
        detail += "Le four est en marche !";
        break;
    }

  //Dans la BDD, table ALERT : id_alert(), id_room, mail_user, dtime, detail
  try
    {
  	sql::Driver *driver;
  	sql::Connection *con;
  	sql::Statement *stmt;

    driver = get_driver_instance();
    con = driver->connect("tcp://127.0.0.1:3306", "equipe", "toor");
    con->setSchema("isen_lab");

    stmt = con->createStatement();

    string request_insert = "INSERT INTO `ALERT`(`ID_ROOM`, `MAIL_USER`, `DTIME`, `DETAIL`) VALUES(\'"+id_room+"\', \'"+mail_user+"\', \'"+dtime+"\', \'"+detail+"\');";
    stmt->execute(request_insert);
    cout << request_insert << endl;

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
  //  #C'estLaFauteDeLaBibliothèque

  /*SmtpClient smtp("smtp.isen-ouest.fr", 465, SmtpClient::SslConnection);

  smtp.setUser("elevecir2");
  smtp.setPassword("Cir2Project");

  MimeMessage message;
  message.setSender(new EmailAddress("steve.corre@isen-ouest.yncrea.fr", "Equipe Isen Lab"));
  message.addRecipient(new EmailAddress("steve.correlemeur@gmail.com", "Drendaf"));//ajouter les bonnes addresses
  message.setSubject("Alerte - Service Isen Lab");

  MimeText text;
  text.setText(QString::fromStdString(detail));
  message.addPart(&text);

  smtp.connectToHost();
  smtp.login();
  smtp.sendMail(message);
  smtp.quit();*/
}
