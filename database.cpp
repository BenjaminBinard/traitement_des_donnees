#include "database.h"

DataBase::DataBase(QString time, int temperature, int humidite, QString user, QString four, double CO2, int chute, QString tv)
{
  try
    {
  	sql::Driver *driver;
  	sql::Connection *con;
  	sql::Statement *stmt;
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
        QString test = "INSERT INTO `VALUE` (`DTIME`, `CAR`, `NUM`, `ID_VALUE`, `TYPE`) VALUES (\'"+dtime+"\', \'"+car+"\', \'"+qsnum+"\', NULL, \'"+qsid_sensor+"\');";
        //string test2 = test.toStdString();
        qDebug() << test << endl;
        pstmt = con->prepareStatement(test.toStdString());
        res = pstmt->executeQuery();
        delete pstmt;
      }



      /*
      on veut remplir VALUE
      on a 8 capteurs
      attention, plusieurs types de données
      variables car et num
      changeantes selon le type de capteur
      toute une phase de tests avant d'envoyer
      id_sensor
      definir les variables propres à chaque capteur
      chute co2 four tv utilisateur temperature humidite time
      i     i   s    s  s            i          i         s
      if (chute co2 temperature humidite)
      {
        num =
        car = NULL
      }
      else if (four tv utilisateur)
      {
        num = NULL
        car =
      }

      dtime = time


      for (int i = 0; i < 8; i++)
      {
        insert into
      }
      */

  	  //stmt->execute("INSERT INTO VALUE() VALUES()");
   	// res = stmt->executeQuery("SELECT 'Hello World!' AS _message");
   	 /*while (res->next())
  	 {
     		 cout << "\t... MySQL replies: ";
     		 // Access column data by alias or column name
     		 cout << res->getString("_message") << endl;
     		 cout << "\t... MySQL says it again: ";
     	 	// Access column data by numeric offset, 1 is the first column
     	 	cout << res->getString(1) << endl;
    	}*/
  	//pstmt = con->preparedStatement("INSERT INTO SENSOR
    delete res;
    delete stmt;
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

//getters
/*QString DataBase::getAddress()
{
  return address;
}

QString DataBase::getIdentity()
{
  return identity;
}

QString DataBase::getPassword()
{
  return password;
}*/











/*try
  {
	sql::Driver *driver;
	sql::Connection *con;
	sql::Statement *stmt;
	sql::ResultSet *res;
	sql::PreparedStatement *pstmt;

	driver = get_driver_instance();
	con = driver->connect("tcp://127.0.0.1:3306", "equipe", "toor");
  	con->setSchema("isen_lab");

  	stmt = con->createStatement();
	stmt->execute("INSERT INTO VALUE() VALUES()");

  //---------------------------------------------------------------------commenté
 	// res = stmt->executeQuery("SELECT 'Hello World!' AS _message");
 	 while (res->next())
	 {
   		 cout << "\t... MySQL replies: ";
   		 // Access column data by alias or column name
   		 cout << res->getString("_message") << endl;
   		 cout << "\t... MySQL says it again: ";
   	 	// Access column data by numeric offset, 1 is the first column
   	 	cout << res->getString(1) << endl;
  	}
	//pstmt = con->preparedStatement("INSERT INTO SENSOR
  //----------------------------------------------------------------fin commenté
  delete res;
  delete stmt;
  delete con;

}
catch (sql::SQLException &e)
{
 	 cout << "# ERR: SQLException in " << __FILE__;
	 cout << __LINE__ << endl;
 	 cout << "# ERR: " << e.what();
 	 cout << " (MySQL error code: " << e.getErrorCode();
 	 cout << ", SQLState: " << e.getSQLState() << " )" << endl;
}*/
