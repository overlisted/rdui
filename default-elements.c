#include "default-elements.h"
#include "util.h"

#include <string.h>

static void RDUIButtonEventReceiver(struct RDUINode* node, enum RDUIEvent event, void* data) {
	struct RDUIButtonData* button_data = node->data;

	RDUIIfEventIs(render) {
		int button_width, button_height;
		CNFGGetTextExtents(button_data->text, &button_width, &button_height, button_data->text_size);

		button_width += button_data->padding * 2;
		button_height += button_data->padding * 2;

		CNFGColor(button_data->color);
		CNFGTackRectangle(
			button_data->position.x,
			button_data->position.y,
			button_data->position.x + button_width,
			button_data->position.y + button_height
		);

		CNFGColor(button_data->text_color);
		CNFGPenX = button_data->position.x + button_data->padding;
		CNFGPenY = button_data->position.y + button_data->padding;
		CNFGDrawText(button_data->text, button_data->text_size);
	}

	RDUIIfEventIs(button) {
		int button_width, button_height;
		CNFGGetTextExtents(button_data->text, &button_width, &button_height, button_data->text_size);

		button_width += button_data->padding * 2;
		button_height += button_data->padding * 2;


		if(button_event->button == 1) {
			if(
				 button_event->position.x > button_data->position.x
				 && button_event->position.y > button_data->position.y
				 && button_event->position.x < button_data->position.x + button_width
				 && button_event->position.y < button_data->position.y + button_height
			) {
				if(button_event->bDown == 1) button_data->color -= 0x101010;
				if(button_event->bDown == 0) {
					button_data->color += 0x101010;
					button_data->clicked_handler(button_data);
				}
			}
		}
	}
}

struct RDUINode* RDUINewButton(struct RDUIButtonData* data) {
	return RDUINewNode(data, RDUIButtonEventReceiver);
}

static void RDUICheckboxEventReceiver(struct RDUINode* node, enum RDUIEvent event, void* data) {
	struct RDUICheckboxData* checkbox_data = node->data;

	RDUIIfEventIs(render) {
		CNFGColor(checkbox_data->color);
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
		if(button_event->button == 1) {
			if(
				 button_event->position.x > checkbox_data->position.x
				 && button_event->position.y > checkbox_data->position.y
				 && button_event->position.x < checkbox_data->position.x + checkbox_data->size
				 && button_event->position.y < checkbox_data->position.y + checkbox_data->size
			) {
				if(button_event->bDown == 1) checkbox_data->color -= 0x101010;
				if(button_event->bDown == 0) {
					checkbox_data->color += 0x101010;
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
	}
}

struct RDUINode* RDUINewCheckbox(struct RDUICheckboxData* data) {
	return RDUINewNode(data, RDUICheckboxEventReceiver);
}

static struct RDUIFieldData* focused_field;
static char shift_down = 0;

static void PutCursorInValidState(struct RDUIFieldData* field) {
	size_t length = strlen(field->value);
	if(field->cursor > length) field->cursor = length;
}

#define KEY_SHIFT 65505
#define KEY_BACKSPACE 65288
#define KEY_DELETE 65535
#define KEY_LEFT_ARROW 65361
#define KEY_RIGHT_ARROW 65363
#define KEY_ESCAPE 65307

static void RDUIFieldEventReceiver(struct RDUINode* node, enum RDUIEvent event, void* data) {
	struct RDUIFieldData* field_data = node->data;

	int text_width, text_height;
	CNFGGetTextExtents(field_data->value, &text_width, &text_height, field_data->font_size);

	struct RDUIPosition pos2 = {
		.x = field_data->position.x + UtilMax(text_width + field_data->padding * 2, field_data->min_width),
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

		CNFGPenX = field_data->position.x + field_data->padding;
		CNFGPenY = field_data->position.y + field_data->padding;
		CNFGDrawText(field_data->value, field_data->font_size);

		if(focused_field == field_data) {
			int cursor_drawing_x, whatever;
			char* buf = UtilCopyString(field_data->value);

			CNFGGetTextExtents(UtilStringCutAfter(buf, field_data->cursor), &cursor_drawing_x, &whatever, field_data->font_size);

      CNFGColor(0);
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
		  focused_field = field_data;
		}
	}

	RDUIIfEventIs(key) {
		if(key_event->keycode == KEY_SHIFT) shift_down = key_event->bDown;

		if(key_event->bDown == 1 && focused_field == field_data) {
			if(key_event->keycode >= 32 && key_event->keycode < 127) {
				if(key_event->keycode > 96 && key_event->keycode < 123 && shift_down) key_event->keycode -= 32;
				field_data->value = UtilStringInsertOne(field_data->value, field_data->cursor, key_event->keycode);
				field_data->cursor++;
			} else {
				if(key_event->keycode == KEY_BACKSPACE) {
					field_data->value = UtilStringRemoveOne(field_data->value, field_data->cursor);
					field_data->cursor--;
				}

				if(key_event->keycode == KEY_DELETE) {
					field_data->value = UtilStringRemoveOne(field_data->value, field_data->cursor + 1);
					PutCursorInValidState(field_data);
				}

				if(key_event->keycode == KEY_LEFT_ARROW) if(field_data->cursor > 0) field_data->cursor--;
				if(key_event->keycode == KEY_RIGHT_ARROW) if(field_data->cursor < strlen(field_data->value)) field_data->cursor++;
				if(key_event->keycode == KEY_ESCAPE) focused_field = NULL;
			}
		}
	}
}

struct RDUINode* RDUINewField(struct RDUIFieldData* data) {
	data->cursor = 0;
	data->value = calloc(1, 1);

	return RDUINewNode(data, RDUIFieldEventReceiver);
}
