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

		if(
			button_event->button == 1
			 && button_event->bDown == 1
			 && button_event->position.x > button_data->position.x
			 && button_event->position.y > button_data->position.y
			 && button_event->position.x < button_data->position.x + button_width
			 && button_event->position.y < button_data->position.y + button_height
		) button_data->clicked_handler(button_data);
	}
}

struct RDUINode* RDUINewButton(struct RDUINode* parent, struct RDUIButtonData* data) {
	return RDUINewNode(parent, data, RDUIButtonEventReceiver);
}
