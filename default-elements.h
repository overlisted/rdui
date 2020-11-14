#pragma once

#include "RDUI.h"

struct RDUIButtonData {
	char* text;
	int text_size;
	int padding;
	int color;
	int text_color;
	RDPoint position;
	char is_held;

	void (*clicked_handler)(struct RDUIButtonData* data);
};

struct RDUINode* RDUINewButton(struct RDUIButtonData* data);

struct RDUICheckboxData {
	char active;
	int size;
	int padding;
	int color;
	int checkmark_color;
	RDPoint position;
	char is_held;

	void (*activate_handler)(struct RDUICheckboxData* data);
	void (*deactivate_handler)(struct RDUICheckboxData* data);
};

struct RDUINode* RDUINewCheckbox(struct RDUICheckboxData* data);

struct RDUIFieldData {
	int padding;
	int font_size;
	int min_width;
	int max_width;
	RDPoint position;
	int border_color;
	char* value;
	size_t cursor;

	void (*type_handler)(struct RDUIFieldData* data);
};

struct RDUINode* RDUINewField(struct RDUIFieldData* data);

struct RDUIOptionsBoxData {
	RDPoint position;
	char** options;
};

struct RDUINode* RDUINewOptionsBox(struct RDUIOptionsBoxData* data);

#ifdef RDUI_IMPLEMENTATION
#include "default-elements.c"
#endif
