#ifndef MENU_UTILS_H
#define MENU_UTILS_H

#include "commons.h"

char* read_string_input(const char* message, char* string_out, size_t max_string_len);

int read_int_input(const char* message);

#endif //MENU_UTILS_H
