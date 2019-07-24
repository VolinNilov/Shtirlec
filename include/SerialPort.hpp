#pragma once
#include <cinttypes>
#include <string>

namespace serial
{
    class serial_port
    {
    public:
        enum class exception
        {
            ok,   // Ок.
            open, // Исключение, вызванное ошибкой при открытии устройства.
            init, // Исключение, вызванное ошибкой при инициализации устройства.
            read, // Исключение, вызванное ошибкой при чтении из устройства.
            write,// Исключениеб вызванное ошибкой при записи данных на устройство.
        };

        // Конструкторы.
        serial_port();
        serial_port(const std::string &path);

        // Деструктор.
        ~serial_port();

        // Открыть устройство по указанному пути.
        void open(const std::string &path);
        // Закрыть устройство.
        void close();

        // Чтение данных из устройства.
        size_t read(char* buffer, size_t size);
        // ЗАпись данных в устройство
        size_t write(std::string buffer);

    protected:
        int _device; // Идентификатор устройства.

    private:

    };
}
