#include "init_serial_port.h"
#include <stdlib.h>  // Библиотека стандартных функций.
#include <unistd.h>  // Функции UNIX-стандарта.
#include <fcntl.h>   // Управления файлами.
#include <errno.h>   // Коды ошибок.
#include <termios.h> // Управление терминалами POSIX.
#include <string.h>  // Строковые функции.

// Инициализация порта.
int init_serial_port(int device)
{
    struct termios sp_attributes;
    struct termios sp_attributes_old;
    memset(&sp_attributes, 0, sizeof sp_attributes);

    // Получение текущих аттрибутов.
    if (tcgetattr(device, &sp_attributes))
    {
        return SP_INIT_GETATTR;
    }

    // Сохранение старых аттрибутов (зачем?).
    sp_attributes_old = sp_attributes;

    // Установка БОД.
    cfsetospeed(&sp_attributes, (speed_t)B9600);
    cfsetispeed(&sp_attributes, (speed_t)B9600);

    // Остальные настройки.
    sp_attributes.c_cflag     &= ~PARENB;            // [ ] Генерация бита чётности.
    sp_attributes.c_cflag     &= ~CSTOPB;            // [ ] Два стоповых бита вместо одного.
    sp_attributes.c_cflag     &= ~CSIZE;             //     Маска размеров символов.
    sp_attributes.c_cflag     |= CS8;                //     [8 бит] Размер символа.

    sp_attributes.c_cflag     &= ~CRTSCTS;           // [ ] Контроль потока.
    sp_attributes.c_cc[VMIN]   = 1;                  // [ ] Чтение вызывает блокировку.
    sp_attributes.c_cc[VTIME]  = 10;                 // [1.0 секунд] Таймаут на чтение.
    sp_attributes.c_cflag     |= CREAD;              // [x] Перейти в режим чтения.
    sp_attributes.c_cflag     |= CLOCAL;             // [x] Игнорировать управление строками.

    // Перевести в режим "raw".
    cfmakeraw(&sp_attributes);

    // Flush порта, затем применение аттрибутов.
    tcflush(device, TCIFLUSH);
    if (tcsetattr(device, TCSANOW, &sp_attributes))
    {
        return SP_INIT_SETATTR;
    }

    return SP_INIT_OK;
}
