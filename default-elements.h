#pragma once

#include "RDUI.h"

struct RDUIButtonData {
	char* text;
	int text_size;
	int padding;
	int color;
	int text_color;
	struct RDUIPosition position;

	void (*clicked_handler)(struct RDUIButtonData* data);
};

struct RDUINode* RDUINewButton(struct RDUINode* parent, struct RDUIButtonData* data);
