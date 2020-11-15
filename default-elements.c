#include "default-elements.h"
#include "util.h"

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

static void RDUIButtonEventReceiver(struct RDUINode* node, enum RDUIEvent event, void* data) {
	struct RDUIButtonData* button_data = node->data;

	RDUIIfEventIs(render) {
		int button_width, button_height;
		CNFGGetTextExtents(button_data->text, &button_width, &button_height, button_data->text_size);

		button_width += button_data->padding * 2;
		button_height += button_data->padding * 2;

		int drawn_color;
		if(button_data->is_held) {
			drawn_color = button_data->color - 0x030303;
		} else {
			drawn_color = button_data->color + 0x030303;
		}

		CNFGColor(drawn_color);
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

		if(ProcessClick(button_event, button_data->position, button_width, button_height, &button_data->is_held)) {
			button_data->clicked_handler(button_data);
		}
	}
}

struct RDUINode* RDUINewButton(struct RDUIButtonData* data) {
	data->is_held = 0;

	return RDUINewNode(data, RDUIButtonEventReceiver);
}

static void RDUICheckboxEventReceiver(struct RDUINode* node, enum RDUIEvent event, void* data) {
	struct RDUICheckboxData* checkbox_data = node->data;

	RDUIIfEventIs(render) {
		int drawn_color;
		if(checkbox_data->is_held) {
			drawn_color = checkbox_data->color - 0x030303;
		} else {
			drawn_color = checkbox_data->color + 0x030303;
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

struct RDUINode* RDUINewCheckbox(struct RDUICheckboxData* data) {
	data->is_held = 0;

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

	RDPoint pos2 = {
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
			if(cursor_drawing_x < 0) cursor_drawing_x = 0;

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

			field_data->type_handler(field_data);
		}
	}
}

struct RDUINode* RDUINewField(struct RDUIFieldData* data) {
	data->cursor = 0;
	data->value = calloc(1, 1);

	return RDUINewNode(data, RDUIFieldEventReceiver);
}

static void RenderOptions(struct RDUIOptionsBoxData* data) {

}

#include <stdio.h>

static void RDUIOptionsBoxEventReceiver(struct RDUINode* node, enum RDUIEvent event, void* data) {
	struct RDUIOptionsBoxData* options_box_data = node->data;

	int width, height;
	CNFGGetTextExtents(
		options_box_data->options[options_box_data->selected_index],
		&width,
		&height,
		options_box_data->font_size
	);

	width += options_box_data->padding * 2;
	height += options_box_data->padding * 2;

	RDUIIfEventIs(render) {
		if(options_box_data->is_open) RenderOptions(options_box_data);

		int drawn_color;
		if(options_box_data->is_held) {
			drawn_color = options_box_data->color - 0x030303;
		} else {
			drawn_color = options_box_data->color + 0x030303;
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

		}

		if(ProcessClick(button_event, options_box_data->position, width, height, &options_box_data->is_held)) {
			if(options_box_data->is_open) {
				options_box_data->is_open = 0;
			} else {
				options_box_data->is_open = 1;
			}
		}
	}

	RDUIIfEventIs(motion) {

	}

	RDUIIfEventIs(key) {
		if(options_box_data->is_open) {

		}
	}
}

struct RDUINode* RDUINewOptionsBox(struct RDUIOptionsBoxData* data) {
	data->selected_index = 0;
	data->is_held = 0;
	data->is_open = 0;

	return RDUINewNode(data, RDUIOptionsBoxEventReceiver);
}
