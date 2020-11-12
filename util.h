#pragma once

#include <stddef.h>
#include "rawdraw/CNFG.h"

char* UtilCopyString(char* original);

char* UtilStringInsert(char* original, size_t index, char* to_insert);
char* UtilStringInsertOne(char* original, size_t index, char to_insert);
char* UtilStringRemove(char* original, size_t from, size_t to);
char* UtilStringRemoveOne(char* original, size_t index);
char* UtilStringAppend(char* original, char* to_append);
char* UtilStringAppendOne(char* original, char to_append);
char* UtilStringCutEnd(char* original, size_t count);
char* UtilStringCutAfter(char* original, size_t index);
char* UtilStringCutStart(char* original, size_t count);
char* UtilStringChange(char* original,const char* data);

void UtilDrawRectangleBorders(int width, int x1, int y1, int x2, int y2);

int UtilMax(int a, int b);
