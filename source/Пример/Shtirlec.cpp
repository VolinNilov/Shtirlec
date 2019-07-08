
// Ввод-вывод.
#include<iostream>
#include<string>

// Потоки.
#include<thread>

// Время.
#include<chrono>

#include<cinttypes> // Типы данных фиксированной длины.
#include<string.h>  // memset()
#include<unistd.h>  // open(), read(), write(), ...
#include<fcntl.h>   // Флаги для чтения файлов.

//thread cinttypes string.h unistd.h fcntl.h chrono

// Класс кодов ошибок.
enum class ListenerException
{
    OK,   // Всё нормально.
    OPEN, // Ошибка открытия.
    READ  // Ошибка чтения.
};

// Здесь using namespace НУЖНО для удобной записи литералов времени (100ms, например).
using namespace std::chrono_literals;
//
////
///////
///////////////
///////////////////////////////////
//////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////
#include <pqxx/pqxx>

struct info
{
    int id;
    float val;
};
typedef union union4byte_t
{
uint8_t Bytes[4];
int32_t Int;
float Float;
};
union4byte_t BU;
info transformation(char* buffer)
{
    info result;
    BU.Bytes[0] = buffer[0];
    BU.Bytes[1] = buffer[1];
    BU.Bytes[2] = buffer[2];
    BU.Bytes[3] = buffer[3];
    result.id = BU.Int;
    BU.Bytes[0] = buffer[4];
    BU.Bytes[1] = buffer[5];
    BU.Bytes[2] = buffer[6];
    BU.Bytes[3] = buffer[7];
    result.val = BU.Float;
    return result;
}
int databaze()
{
    info data;
    return 0;
}
///////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////
///////////////////////////////////
///////////////
///////
////
//
int main(int argc, char **argv) // Аргументы main() - число аргументов коммандной строки и массив строк, их содержащих.
{
    // Константы.
    const size_t BufferSize = 64; // Размер буффера.

    // Буффер для временного хранения передаваемых данных.
    char Buffer[BufferSize];

    // Чтение имени устройства.
    std::string DevicePath;
    std::cout << "Введите имя устройства: "; // Обычно /dev/ttyACM$ или /dev/ttyUSB$, где $ - число.
    std::cin >> DevicePath;

    try // Участок кода, способный вызвать исключение.
    {
        // Открытие устройства в режиме чтения.
        int Device = open(DevicePath.c_str(), O_RDWR | O_NOCTTY);
        if (Device == -1)
        {
            // Вызов исключения при неудачном открытии устройства.
            throw(ListenerException::OPEN);
        }

        // В бесконечном цикле слушаем порт.
        while (true)
        {
            // Чтение.
            memset(Buffer, 0, BufferSize);
            int ResultCode = read(Device, Buffer, BufferSize - 1);
            if (ResultCode) // Ввод есть.
            {
                if (ResultCode == -1)
                {
                    // Вызов исключения при неудачном чтении устройства.
                    throw(ListenerException::READ);
                }
//
////
////////
//////////////////
/////////////////////////////////////
/////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////
            info returned = transformation(Buffer);
////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////
/////////////////////////////////////
//////////////////
////////
////
//
                // Вывод на экран.
                std::cout << 'Буфер:' << Buffer << ' id: ' << returned.id << ' val: ' << returned.val << std::endl; 
            }
            else // Ввода нет.
            {
                // Для сохранения ресурсов компьютера обновление только через некоторое время.
                std::this_thread::sleep_for(100ms);
            }
        }
    }
    // Обработка исключений
    catch (ListenerException Exception)
    {
        switch (Exception)
        {
            case ListenerException::OK: { break; }
            case ListenerException::OPEN:
            {
                std::cerr << "Ошибка открытия устройства" << std::endl;
                break;
            }
            case ListenerException::READ:
            {
                std::cerr << "Ошибка чтения устройства" << std::endl;
                break;
            }
        }
    }

    // Завершение программы.
    return(0);
}
