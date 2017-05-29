/**
* \file database.cpp
*/

#include "database.h"

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
DataBase::DataBase(string path, string login, string pass, QString time, int temperature, int humidite, QString user, QString four, double CO2, int chute, QString tv, QString pas, string id_room)
{

  /*ici on gere les données du traitment utilisateur*/
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
        //DECOMMENTER CES LIGNES POUR FAIRE REFONCTIONNER+++++++++++++++++++++++++++++++++++
        pstmt = con->prepareStatement(qrequest.toStdString());
        res = pstmt->executeQuery();
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
/**
* \fn Comparaison
* Cette méthode consiste à envoyer une requête SELECT ... FROM ... WHERE pour récupérer les seuils propres à chaque capteur.
* Les valeurs passées en paramètres sont ensuite comparées avec leurs seuils respectifs, dès qu'un dépassement est constaté, un appel est lancé à la méthode createAlert().
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
*/
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
        createAlert(path, login, pass, user, time, i, sensDepassement, value, id_room);
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

  qDebug() << "Fin de fonction Comparaison()" << endl;
}



/**
* \fn createAlert
* Cette méthode peut être appelée par \b Comparaison() dans la cas de dépassement d'un seuil.
* En fonction du type d'alerte (ou plutôt du capteur qui la déclenche), la description de l'alerte changera.
* une requête INSERT INTO est ensuite envoyée pour ajouter les données liées à l'alerte dans l'entité ALERTE de la base de données.
*/
void DataBase::createAlert(string path, string login, string pass, QString user, QString time, int type, int sensDepassement, int value, string id_room)
{
  qDebug() << "Une alerte" << endl;

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
    con = driver->connect(path, login, pass);
    con->setSchema("isen_lab");

    stmt = con->createStatement();


    string request_insert = "INSERT INTO `ALERT`(`ID_ROOM`, `MAIL_USER`, `DTIME`, `DETAIL`) VALUES(\'"+id_room+"\', \'"+mail_user+"\', \'"+dtime+"\', \'"+detail+"\');";
    //cout << request_insert << endl;
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


/**
* \fn getIdRoom
* Cette méthode sert à récupérer l'identifiant de la chambre d'un utilisateur
* \bug Cette fonction est inefficace si, dans la dernière trame, l'utilisateur n'est pas présent dans la pièce...
*/
/*int DataBase::getIdRoom(string path, string login, string pass, QString user)
{
  try
    {
  	sql::Driver *driver;
  	sql::Connection *con;
    sql::Statement *stmt;
    sql::ResultSet *res;

    int idroom;

    //erreur sql si l'utilisateur n'est pas dans la piece et qu'il y a une alerte
    //par defaut la piece est 1
    if (user.toStdString().compare("") == 0)
    {
      idroom = 1;
      return idroom;
    }

    driver = get_driver_instance();
    con = driver->connect(path, login, pass);
    con->setSchema("isen_lab");

    stmt = con->createStatement();

    string request_select = "SELECT `ID_ROOM` FROM `USER_ROOM` WHERE `MAIL_USER` = \'"+user.toStdString()+"\';";
    res = stmt->executeQuery(request_select);
    res->next();

    idroom = stoi(res->getString("ID_ROOM"));


    delete con;
    delete stmt;
    delete res;

    qDebug() << "Room : " << idroom << endl;
    return idroom;

    }

    catch (sql::SQLException &e)
    {
       	 cout << "# ERR: SQLException in " << __FILE__;
      	 cout << __LINE__ << endl;
       	 cout << "# ERR: " << e.what();
       	 cout << " (MySQL error code: " << e.getErrorCode();
       	 cout << ", SQLState: " << e.getSQLState() << " )" << endl;
    }

}*/
