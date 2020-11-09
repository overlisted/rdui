#pragma once

#include <stddef.h>

char* UtilStringInsert(char* original, size_t index, char* to_insert);
char* UtilStringRemove(char* original, size_t from, size_t to);
char* UtilStringAppend(char* original, char* to_append);
