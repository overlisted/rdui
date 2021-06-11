#pragma once
#pragma GCC diagnostic ignored "-Wunused-variable"

#include "RDUI.h"
#include "util.h"
#include <onlyinput/onlyinput.h>
#include <string.h>

static char ProcessClick(
	struct RDUIEventData_button* event,
	RDPoint	element_position,
	int element_width,
	int element_height,
	char* element_is_held
) {
	char result = 0;
	if(event->button == 1) {
		if(
			 event->position.x > element_position.x
			 && event->position.y > element_position.y
			 && event->position.x < element_position.x + element_width
			 && event->position.y < element_position.y + element_height
		) {
			if(*element_is_held && event->bDown == 0) result = 1;
			if(event->bDown) *element_is_held = 1;
		}

		if(!event->bDown) *element_is_held = 0;
	}

	return result;
}

struct RDUIButtonData {
	char* text;
	int font_size;
	int padding;
	int color;
	int font_color;
	RDPoint position;
	char is_held;

	void (*clicked_handler)(struct RDUIButtonData* data);
};

static void RDUIButtonEventReceiver(struct RDUIElement* element, enum RDUIEvent event, void* data) {
	struct RDUIButtonData* button_data = element->data;

	RDUIIfEventIs(render) {
		int button_width, button_height;
		CNFGGetTextExtents(button_data->text, &button_width, &button_height, button_data->font_size);

		button_width += button_data->padding * 2;
		button_height += button_data->padding * 2;

		int drawn_color = button_data->color;
		if(button_data->is_held) {
			drawn_color -= 0x030303;
		}

		CNFGColor(drawn_color);
		CNFGTackRectangle(
			button_data->position.x,
			button_data->position.y,
			button_data->position.x + button_width,
			button_data->position.y + button_height
		);

		CNFGColor(button_data->font_color);
		CNFGPenX = button_data->position.x + button_data->padding;
		CNFGPenY = button_data->position.y + button_data->padding;
		CNFGDrawText(button_data->text, button_data->font_size);
	}

	RDUIIfEventIs(button) {
		int button_width, button_height;
		CNFGGetTextExtents(button_data->text, &button_width, &button_height, button_data->font_size);

		button_width += button_data->padding * 2;
		button_height += button_data->padding * 2;

		if(ProcessClick(button_event, button_data->position, button_width, button_height, &button_data->is_held)) {
			button_data->clicked_handler(button_data);
		}
	}
}

struct RDUIElement* RDUINewButton(struct RDUIButtonData* data) {
	data->is_held = 0;

	return RDUINewElement(data, RDUIButtonEventReceiver);
}

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

static void RDUICheckboxEventReceiver(struct RDUIElement* element, enum RDUIEvent event, void* data) {
	struct RDUICheckboxData* checkbox_data = element->data;

	RDUIIfEventIs(render) {
		int drawn_color = checkbox_data->color;
		if(checkbox_data->is_held) {
			drawn_color -= 0x030303;
		}

		CNFGColor(drawn_color);
		CNFGTackRectangle(
			checkbox_data->position.x,
			checkbox_data->position.y,
			checkbox_data->position.x + checkbox_data->size,
			checkbox_data->position.y + checkbox_data->size
		);

		if(checkbox_data->active) {
			CNFGColor(checkbox_data->checkmark_color);
			CNFGTackSegment(
				checkbox_data->position.x + checkbox_data->padding,
				checkbox_data->position.y + checkbox_data->size / 2,
				checkbox_data->position.x + checkbox_data->size / 2,
				checkbox_data->position.y - checkbox_data->padding + checkbox_data->size
			);

			CNFGTackSegment(
				checkbox_data->position.x - checkbox_data->padding + checkbox_data->size,
				checkbox_data->position.y + checkbox_data->padding,
				checkbox_data->position.x + checkbox_data->size / 2,
				checkbox_data->position.y - checkbox_data->padding + checkbox_data->size
			);
		}
	}

	RDUIIfEventIs(button) {
		if(ProcessClick(button_event, checkbox_data->position, checkbox_data->size, checkbox_data->size, &checkbox_data->is_held)) {
			if(checkbox_data->active) {
				checkbox_data->active = 0;
				checkbox_data->deactivate_handler(checkbox_data);
			} else {
				checkbox_data->active = 1;
				checkbox_data->activate_handler(checkbox_data);
			}
		}
	}
}

struct RDUIElement* RDUINewCheckbox(struct RDUICheckboxData* data) {
	data->is_held = 0;

	return RDUINewElement(data, RDUICheckboxEventReceiver);
}

extern struct RDUIFieldData* RDUIFocusedField;
struct RDUIFieldData {
	int padding;
	int font_size;
	int font_color;
	int min_width;
	int max_width;
	RDPoint position;
	int border_color;
	char* value;
	char* allowed_symbols; // NOTE: can be left NULL
	size_t cursor;

	void (*type_handler)(struct RDUIFieldData* data);
};

struct RDUIFieldData* RDUIFocusedField = NULL;

static void PutCursorInValidState(struct RDUIFieldData* field) {
	size_t length = strlen(field->value);
	if(field->cursor > length) field->cursor = length;
}

static void RDUIFieldEventReceiver(struct RDUIElement* element, enum RDUIEvent event, void* data) {
	struct RDUIFieldData* field_data = element->data;

	int text_width, text_height;
	CNFGGetTextExtents(field_data->value, &text_width, &text_height, field_data->font_size);

	RDPoint pos2 = {
		.x = field_data->position.x + UtilMin(UtilMax(text_width + field_data->padding * 2, field_data->min_width), field_data->max_width),
		.y = field_data->position.y + text_height + field_data->padding * 2
	};

	RDUIIfEventIs(render) {
		CNFGColor(field_data->border_color);
		UtilDrawRectangleBorders(
			2,
			field_data->position.x,
			field_data->position.y,
			pos2.x,
			pos2.y
		);

		CNFGColor(field_data->font_color);
		CNFGPenX = field_data->position.x + field_data->padding;
		CNFGPenY = field_data->position.y + field_data->padding;
		CNFGDrawText(field_data->value, field_data->font_size);

		if(RDUIFocusedField == field_data) {
			int cursor_drawing_x, whatever;
			char* buf = UtilCopyString(field_data->value);

			CNFGGetTextExtents(UtilStringCutAfter(buf, field_data->cursor), &cursor_drawing_x, &whatever, field_data->font_size);
			if(cursor_drawing_x < 0) cursor_drawing_x = 0;

      CNFGColor(field_data->font_color);
			CNFGTackSegment(
				field_data->position.x + field_data->padding + cursor_drawing_x,
				field_data->position.y + field_data->padding,
				field_data->position.x + field_data->padding + cursor_drawing_x,
				field_data->position.y + field_data->padding + text_height
			);
		}
	}

	RDUIIfEventIs(button) {
		if(
			button_event->button == 1
			 && button_event->bDown == 1
			 && button_event->position.x > field_data->position.x
			 && button_event->position.y > field_data->position.y
			 && button_event->position.x < pos2.x
			 && button_event->position.y < pos2.y
	 	) {
		  RDUIFocusedField = field_data;
		}
	}

	RDUIIfEventIs(key) {
		if(key_event->bDown == 1 && RDUIFocusedField == field_data) {
			char character = OIReadAscii();
			if(character != 0) {
				if(field_data->allowed_symbols && !UtilStringContains(field_data->allowed_symbols, character)) return;
				field_data->value = UtilStringInsertOne(field_data->value, field_data->cursor, character);
				field_data->cursor++;
			} else {
				if(key_event->keycode == CNFG_KEY_BACKSPACE && field_data->cursor > 0) {
					field_data->value = UtilStringRemoveOne(field_data->value, field_data->cursor);
					field_data->cursor--;
				}

				if(key_event->keycode == CNFG_KEY_DELETE) {
					field_data->value = UtilStringRemoveOne(field_data->value, field_data->cursor + 1);
					PutCursorInValidState(field_data);
				}

				if(key_event->keycode == CNFG_KEY_LEFT_ARROW) if(field_data->cursor > 0) field_data->cursor--;
				if(key_event->keycode == CNFG_KEY_RIGHT_ARROW) if(field_data->cursor < strlen(field_data->value)) field_data->cursor++;
				if(key_event->keycode == CNFG_KEY_ESCAPE) RDUIFocusedField = NULL;
			}

			field_data->type_handler(field_data);
		}
	}
}

struct RDUIElement* RDUINewField(struct RDUIFieldData* data) {
	if(!data->value || *data->value == '\0') {
		data->value = calloc(1, 1);
		data->cursor = 0;
	} else {
		data->value = UtilCopyString(data->value);
		data->cursor = strlen(data->value);
	}

	return RDUINewElement(data, RDUIFieldEventReceiver);
}

struct RDUIOptionsBoxData {
	RDPoint position;
	int color;
	int font_color;
	int font_size;
	int padding;
	char** options; // NOTE: they have to be null terminated
	char* held_options;
	size_t selected_index;
	char is_held;
	char is_open;

	void (*select_handler)(struct RDUIOptionsBoxData* data);
};

static void RenderOptions(struct RDUIOptionsBoxData* data, int option_width, int option_height) {
	for(size_t i = 0; data->options[i] != NULL; i++) {
		int x = data->position.x;
		int y = data->position.y + ((i - data->selected_index) * option_height);

		int drawn_color = data->color - 0x050505;
		if(data->held_options[i]) {
			drawn_color -= 0x030303;
		}

		if(
			RDUIMousePosition.x > x
			 && RDUIMousePosition.y > y
			 && RDUIMousePosition.x < x + option_width
			 && RDUIMousePosition.y < y + option_height
		) {
			drawn_color -= 0x020202;
		}

		CNFGColor(drawn_color);
		CNFGTackRectangle(x, y, x + option_width, y + option_height);

		CNFGColor(data->font_color);
		CNFGPenX = x + data->padding;
		CNFGPenY = y + data->padding;
		CNFGDrawText(data->options[i], data->font_size);
	}
}

static void RDUIOptionsBoxEventReceiver(struct RDUIElement* element, enum RDUIEvent event, void* data) {
	struct RDUIOptionsBoxData* options_box_data = element->data;

	int width = 0, height = 0;
	for(size_t i = 0; options_box_data->options[i] != NULL; i++) {
		int _width, _height;

		CNFGGetTextExtents(
			options_box_data->options[i],
			&_width,
			&_height,
			options_box_data->font_size
		);

		width = UtilMax(width, _width);
		height = UtilMax(height, _height);
	}

	width += options_box_data->padding * 2;
	height += options_box_data->padding * 2;

	RDUIIfEventIs(render) {
		if(options_box_data->is_open) RenderOptions(options_box_data, width, height);

		int drawn_color = options_box_data->color;
		if(options_box_data->is_held) {
			drawn_color -= 0x030303;
		}

		CNFGColor(drawn_color);
		CNFGTackRectangle(
			options_box_data->position.x,
			options_box_data->position.y,
			options_box_data->position.x + width,
			options_box_data->position.y + height
		);

		CNFGColor(options_box_data->font_color);
		CNFGPenX = options_box_data->position.x + options_box_data->padding;
		CNFGPenY = options_box_data->position.y + options_box_data->padding;
		CNFGDrawText(options_box_data->options[options_box_data->selected_index], options_box_data->font_size);
	}

	RDUIIfEventIs(button) {
		if(options_box_data->is_open) {
			for(size_t i = 0; options_box_data->options[i] != NULL; i++) {
				RDPoint position = {
					.x = options_box_data->position.x,
					.y = options_box_data->position.y + ((i - options_box_data->selected_index) * height)
				};

				if(ProcessClick(button_event, position, width, height, &options_box_data->held_options[i])) {
					options_box_data->is_open = 0;
					options_box_data->selected_index = i;
					options_box_data->select_handler(options_box_data);
					return;
				}
			}
		}

		if(ProcessClick(button_event, options_box_data->position, width, height, &options_box_data->is_held)) {
			if(options_box_data->is_open) {
				options_box_data->is_open = 0;
			} else {
				options_box_data->is_open = 1;
			}
		}
	}

	RDUIIfEventIs(key) {
		if(options_box_data->is_open) {

		}
	}
}

struct RDUIElement* RDUINewOptionsBox(struct RDUIOptionsBoxData* data) {
	size_t options_count = 0;
	while(data->options[options_count] != NULL) options_count++;

	data->held_options = calloc(options_count, 1);
	data->selected_index = 0;
	data->is_held = 0;
	data->is_open = 0;

	return RDUINewElement(data, RDUIOptionsBoxEventReceiver);
}
