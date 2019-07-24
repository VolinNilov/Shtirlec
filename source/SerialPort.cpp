#include "SerialPort.hpp"

#include <unistd.h>     // Функции UNIX-стандарта.
#include <fcntl.h>      // Управления файлами.

extern "C"
{
    #include "init_serial_port.h"
}

namespace serial
{
    /////////////////  serial_port  ////////////////
    // PUBLIC:
    // Конструкторы.
    serial_port::serial_port()
    {
        _device = 0;
    }
    serial_port::serial_port(const std::string &path)
    {
        _device = 0;
        open(path);
    }

    // Деструктор.
    serial_port::~serial_port()
    {
        close();
    }

    // Открыть устройство по указанному пути.
    void serial_port::open(const std::string &path)
    {
        if (_device) { close(); }

        int new_device = ::open(path.c_str(), O_RDWR| O_NOCTTY);
        if (new_device < 0) { throw(exception::open); }
        if (init_serial_port(new_device)) { throw(exception::init); }

        _device = new_device;
    }
    // Закрыть устройство.
    void serial_port::close()
    {
        ::close(_device);
        _device = 0;
    }

    // Чтение из устройства.
    size_t serial_port::read(char* buffer, size_t size)
    {
        int read_chars = ::read(_device, buffer, size);
        if (read_chars >= 0) { return (size_t)read_chars; }
        else { throw(exception::read); }
    }

    //Передача данных через serial-порт на устройство
    size_t serial_port::write(std::string buffer)
    {
        int write_chars = ::write(_device, buffer.c_str(), buffer.length());
        if(write_chars >= 0)  { return(size_t) write_chars; }
        else { throw(exception::write); }
    }

    // PROTECTED:

    // PRIVATE:
}
