#include "util.h"

#include <stdlib.h>
#include <string.h>

char* UtilCopyString(char* original) {
	size_t original_length = strlen(original) + 1;
	char* clone = malloc(original_length);

	memcpy(clone, original, original_length);

	return clone;
}

char* UtilStringInsert(char* original, size_t index, char* to_insert) {
	size_t original_length = strlen(original);
	size_t to_insert_length = strlen(to_insert);

	original = realloc(original, original_length + to_insert_length + 1);

	memmove(original + (index + to_insert_length), original + index, to_insert_length);
	memcpy(original + index, to_insert, to_insert_length);

	return original;
}

char* UtilStringInsertOne(char* original, size_t index, char to_insert) {
	size_t original_length = strlen(original);

	original = realloc(original, original_length + 2);

	memmove(original + index + 1, original + index, original_length - index + 1);
	original[index] = to_insert;

	return original;
}

char* UtilStringRemove(char* original, size_t from, size_t to) {
	size_t original_length = strlen(original);
	size_t remove_length = to - from;

	memmove(original + from, original + to, remove_length);
	original = realloc(original, original_length - remove_length + 1);

	return original;
}

char* UtilStringRemoveOne(char* original, size_t index) {
	size_t original_length = strlen(original);

	memmove(original + index - 1, original + index, original_length - index + 1);
	original = realloc(original, original_length);

	return original;
}

char* UtilStringAppend(char* original, char* to_append) {
	size_t original_length = strlen(original);
	size_t to_append_length = strlen(to_append);

	original = realloc(original, original_length + to_append_length + 1);
	memcpy(original + original_length, to_append, to_append_length);

	return original;
}

char* UtilStringAppendOne(char* original, char to_append) {
	size_t original_length = strlen(original);

	original = realloc(original, original_length + 2);
	original[original_length] = to_append;
	original[original_length + 1] = '\0';

	return original;
}

char* UtilStringCutAfter(char* original, size_t index) {
	char* result = realloc(original, index + 1);
	result[index] = '\0';

	return result;
}

char* UtilStringCutEnd(char* original, size_t count) {
	size_t original_length = strlen(original);
	size_t result_length = original_length - count;

	char* result = malloc(result_length);
	strncpy(result, original, result_length);

	return result;
}

char* UtilStringCutStart(char* original, size_t count) {
	return original + count;
}

char* UtilStringChange(char* original, const char* data) {
	size_t data_length = strlen(data) + 1;

	original = realloc(original, data_length);

	memcpy(original, data, data_length);

	return original;
}

int UtilStringContains(char* string, char c) {
	size_t length = strlen(string);

	for(size_t i = 0; i < length; i++) if(string[i] == c) return 1;

	return 0;
}

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

int UtilMin(int a, int b) {
	return a < b ? a : b;
}
