#include "default-elements.h"

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
			if(button_event->bDown == 1) button_data->color -= 0x101010;
			if(button_event->bDown == 0) button_data->color += 0x101010;

			if(
				button_event->bDown == 0
				 && button_event->position.x > button_data->position.x
				 && button_event->position.y > button_data->position.y
				 && button_event->position.x < button_data->position.x + button_width
				 && button_event->position.y < button_data->position.y + button_height
			) button_data->clicked_handler(button_data);
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
			if(button_event->bDown == 1) checkbox_data->color -= 0x101010;
			if(button_event->bDown == 0) checkbox_data->color += 0x101010;

			if(
				button_event->bDown == 0
				 && button_event->position.x > checkbox_data->position.x
				 && button_event->position.y > checkbox_data->position.y
				 && button_event->position.x < checkbox_data->position.x + checkbox_data->size
				 && button_event->position.y < checkbox_data->position.y + checkbox_data->size
			) {
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

struct RDUINode* RDUINewCheckbox(struct RDUICheckboxData* data) {
	return RDUINewNode(data, RDUICheckboxEventReceiver);
}
