#include <iostream>
#include <pqxx/pqxx>

//таблица measures
//id_sensor, receiver_timestamp, measured_value

/// Query employees from database, print results.
int main(int, char *argv[])
{
  try
  {
    int id = 12;
    float val = 36.6;
    pqxx::connection c{"host=localhost dbname=sensors_data user=inserter password=inserter_p"}; //данные по которым осуществляется транзакция
    pqxx::work txn{c}; //открытие транзакции

    std::string Query = 
    "INSERT INTO measures (id_sensor, receival_timestamp, measured_value) "
    "VALUES("
    "(SELECT id_sensor FROM sensor_instances WHERE pid_sensor = " + std::to_string(id) + "), " +
    "now(), " +
    std::to_string(val) +
    ")";

    pqxx::result r = txn.exec(Query); //запрос
    txn.commit();
  }
  catch (const pqxx::sql_error &e)
  {
    std::cerr << "SQL error: " << e.what() << std::endl;
    std::cerr << "Query was: " << e.query() << std::endl;
    return 2;
  }
  catch (const std::exception &e)
  {
    std::cerr << "Error: " << e.what() << std::endl;
    return 1;
  }
}