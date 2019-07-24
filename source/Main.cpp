// Ввод-вывод.
#include <iostream>
#include <string>

//База Данных
#include <pqxx/pqxx>

// Потоки.
#include <thread>

// Время.
#include <chrono>

#include <cinttypes> // Типы данных фиксированной длины.
#include <cstring>   // memset()
#include <unistd.h>  // open(), read(), write(), ...
#include <fcntl.h>   // Флаги для чтения файлов.
#include <cerrno>    // Коды ошибок C.

// Serial port.
#include "SerialPort.hpp"

// Здесь using namespace НУЖНО для удобной записи литералов времени (100ms, например).
using namespace std::chrono_literals;

struct info
{
    int id;
    float val;
};

union union4byte_t
{
    uint8_t Bytes[4];
    int32_t Int;
    float Float;
};
union4byte_t BU;

int database(info &result) //Отправка данных в БД (Базу Данных)
{
    try
    {
       pqxx::connection c{"host=localhost dbname=sensors_data user=inserter password=inserter_p"}; //данные по которым осуществляется транзакция
       pqxx::work txn{c}; //открытие транзакции

       std::string Query = 
       "INSERT INTO measures (id_sensor, receival_timestamp, measured_value) "
       "VALUES("
        + std::to_string(result.id) + ", " +
       "now(), " +
       std::to_string(result.val) +
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
    return(0);
}

info transformation(char* buffer)
{
    info result;
    std::memcpy(BU.Bytes, buffer, 4 * sizeof(char)); 
    result.id = BU.Int;
    std::memcpy(BU.Bytes, buffer + 4, 4 * sizeof(char));
    result.val = BU.Float;
    return result;
}

int main(int argc, char **argv) // Аргументы main() - число аргументов коммандной строки и массив строк, их содержащих.
{
    info result;
    // Константы.
    const size_t BufferSize = 16; // Размер буффера.
    const size_t PacketSize = 9;  // Размер пакета.
    const auto SleepInterval = 100ms;

    // Буффер для временного хранения передаваемых данных.
    char Buffer[BufferSize];

    // Чтение имени устройства.
    std::string DevicePath;
    if (argc > 1)
    {
        // Из аргументов коммандной строки.
        DevicePath = argv[1];
    }
    else
    {
        // Из стандартного ввода.
        std::cout << "Введите имя устройства: /dev/tty"; // Обычно /dev/ttyACM$ или /dev/ttyUSB$, где $ - число.
        std::cin >> DevicePath;
        DevicePath = "/dev/tty" + DevicePath;
    }

    // Инициализация порта.
    try
    {
        serial::serial_port SerialPort(DevicePath);
        //std::cerr << strerror(errno) << std::endl;

        // В бесконечном цикле слушаем порт.
        while (true)
        {
            // Чтение.
            std::memset(Buffer, 0, BufferSize * sizeof(char));
            size_t Estimated = PacketSize;
            while(Estimated)
            {
                size_t ResultCode = SerialPort.read(Buffer + (PacketSize - Estimated), Estimated);
                //std::cout << ResultCode << std::endl;
                if (ResultCode) // Ввод есть.
                {
                    Estimated -= ResultCode;
                }
                else // Ввода нет.
                {
                    // Для сохранения ресурсов компьютера обновление только через некоторое время.
                    std::this_thread::sleep_for(SleepInterval);
                }
            }
            
            /*
            for (size_t i = 0; i < PacketSize; ++i)
            {
                std::cout << (int)(unsigned char)(Buffer[i]) << " ";
            }
            std::cout << std::endl;
            for (size_t i = 0; i < PacketSize; ++i)
            {
                std::cout << (Buffer[i]) << " ";
            }            
            std::cout << std::endl;*/
            //std::cout << Buffer[0] << std::endl;
                
            switch(Buffer[0])
            {
                case '#':
                {
                    result = transformation(Buffer + 1);
                    std::cout << "ID: " << result.id << "   " << "Значение: " << result.val << std::endl;
                    database(result);
                    break;
                }
                case '!':
                {
                    std::cout << "Ошибка базовой станции: " << Buffer + 1 << std::endl;
                    break;
                }
            }
        }
    }
    catch (serial::serial_port::exception Exception)
    {
        switch (Exception)
        {
            case serial::serial_port::exception::ok: { break; }
            case serial::serial_port::exception::open:
            {
                std::cerr << "Ошибка при открытии устройства " + DevicePath << std::endl;
                break;
            }
            case serial::serial_port::exception::init:
            {
                std::cerr << "Ошибка при инициализации устройства " + DevicePath << std::endl;
                break;
            }
            case serial::serial_port::exception::read:
            {
                std::cerr << "Ошибка при чтении устройства " + DevicePath << std::endl;
                break;
            }
        }
    }
    // Завершение программы.
    return(0);
}