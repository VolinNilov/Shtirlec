#pragma once

// Инициализация порта.
enum init_error_codes
{
    SP_INIT_OK,      // Ок.
    SP_INIT_GETATTR, // Ошибка при получении аттрибутов.
    SP_INIT_SETATTR  // Ошибка при установке аттрибутов.
};
int init_serial_port(int device);
