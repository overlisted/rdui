#pragma once

#include <stddef.h>
#include "rawdraw/CNFG.h"

char* UtilStringInsert(char* original, size_t index, char* to_insert);
char* UtilStringRemove(char* original, size_t from, size_t to);
char* UtilStringAppend(char* original, char* to_append);

void UtilDrawRectangleBorders(int width, int x1, int y1, int x2, int y2) {
	CNFGSetLineWidth(width);
	CNFGTackSegment(x1, y1, x2, y1);
	CNFGTackSegment(x2, y1, x2, y2);
	CNFGTackSegment(x2, y2, x1, y2);
	CNFGTackSegment(x1, y2, x1, y1);
}

int UtilMax(int a, int b) {
	return a > b ? a : b;
}
