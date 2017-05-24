#include "database.h"

DataBase::DataBase(QString time, int temperature, int humidite, QString user, QString four, double CO2, int chute, QString tv)
{
  try
    {
  	sql::Driver *driver;
  	sql::Connection *con;
  	//sql::ResultSet *res;
  	//sql::PreparedStatement *pstmt;

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
        QString test = "INSERT INTO `VALUE` (`DTIME`, `CAR`, `NUM`, `ID_VALUE`, `TYPE`) VALUES (\'"+dtime+"\', \'"+car+"\', \'"+qsnum+"\', NULL, \'"+qsid_sensor+"\');";
        //string test2 = test.toStdString();
        qDebug() << test << endl;
        //pstmt = con->prepareStatement(test.toStdString());
        //res = pstmt->executeQuery();
        //delete pstmt;
      }




    //delete res;
    delete con;
    //delete pstmt;

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
/*void DataBase::Comparaison(QString time, int temperature, int humidite, QString user, QString four, double CO2, int chute, QString tv)
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

    int value;
    for (int i = 1; i < 5; i++)
    {
      QString request_high = "SELECT `THRESHOLD_HIGH` FROM `SENSOR` WHERE `TYPE` = \'"+i+"\' AND `ROOM` = 1;";
      QString request_low = "SELECT `THRESHOLD_LOW` FROM `SENSOR` WHERE `TYPE` = \'"+i+"\' AND `ROOM` = 1;";
      switch (i)
      {
        case 1:
          value = chute;
          break;
        case 2;
          value = CO2;
          break;
        case 3:
          value = humidite;
          break;
        case 4:
          value = temperature;
          break;
      }

      res = stmt->executeQuery(request_high);
      if (value >= res)
      {
        createAlert();
      }
      res = stmt->executeQuery(request_low);
      if (value <= res)
      {
        createAlert();
      }
    }


  }

  delete res;
  delete con;
  delete stmt;

    catch (sql::SQLException &e)
    {
     	 cout << "# ERR: SQLException in " << __FILE__;
    	 cout << __LINE__ << endl;
     	 cout << "# ERR: " << e.what();
     	 cout << " (MySQL error code: " << e.getErrorCode();
     	 cout << ", SQLState: " << e.getSQLState() << " )" << endl;
    }

}



void DataBase::createAlert()
{
  /*envoyer le mail : https://curl.haxx.se/libcurl/c/smtp-mail.html*/
//}
