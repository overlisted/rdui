#include "util.h"

#include <stdlib.h>
#include <string.h>

char* UtilStringInsert(char* original, size_t index, char* to_insert) {
	size_t original_length = strlen(original);
	size_t to_insert_length = strlen(to_insert);

	original = realloc(original, original_length + to_insert_length + 1);

	memmove(original + (index + to_insert_length), original + index, to_insert_length);
	memcpy(original + index, to_insert, to_insert_length);

	return original;
}

char* UtilStringRemove(char* original, size_t from, size_t to) {
	size_t original_length = strlen(original);
	size_t remove_length = to - from;

	memmove(original + from, original + to, remove_length);
	original = realloc(original, original_length - remove_length + 1);

	return original;
}

char* UtilStringAppend(char* original, char* to_append) {
	size_t original_length = strlen(original);
	size_t to_append_length = strlen(to_append);

	original = realloc(original, original_length + to_append_length + 1);
	memcpy(original + original_length, to_append, to_append_length);

	return original;
}
