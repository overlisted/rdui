#pragma once

#include <stddef.h>
#include "rawdraw/CNFG.h"

char* UtilStringInsert(char* original, size_t index, char* to_insert);
char* UtilStringRemove(char* original, size_t from, size_t to);
char* UtilStringAppend(char* original, char* to_append);

void UtilDrawRectangleBorders(int width, int x1, int y1, int x2, int y2);

int UtilMax(int a, int b);
