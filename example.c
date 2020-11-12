#define CNFG_IMPLEMENTATION
#define RDUI_IMPLEMENTATION

#include "RDUI.h"
#include "default-elements.h"

#include "util.h"

#include <stdio.h>
#include <string.h>

void HandleKey(int keycode, int bDown) {
	RDUIHandleKeyImpl(keycode, bDown);
}

void HandleButton(int x, int y, int button, int bDown) {
	RDUIHandleButtonImpl(x, y, button, bDown);
}

void HandleMotion(int x, int y, int mask) {
	RDUIHandleMotionImpl(x, y, mask);
}

void HandleDestroy() {}

void ButtonSecondClickedHandle(struct RDUIButtonData* data) {
	exit(0);
}

void ButtonClickedHandler(struct RDUIButtonData* data) {
	data->text = "Not again, please!";
	data->clicked_handler = ButtonSecondClickedHandle;
}

void CheckboxActivateHandler(struct RDUICheckboxData* data) {

}

void CheckboxDeactivateHandler(struct RDUICheckboxData* data) {

}

void FieldTypeHandler(struct RDUIFieldData* data) {
	if(!strcmp(data->value, "demo")) UtilStringChange(data->value, "example");
}

int main(int argv, char* argc[]) {
	RDUIInit();

	int i;
	CNFGBGColor = 0xffffff;

	CNFGSetup("RDUI Example", 800, 600);

	struct RDUIButtonData button_data = {
		.text = "Click on me!",
		.text_size = 6,
		.padding = 5,
		.color = 0x555555,
		.text_color = 0xffffff,
		.position = {
			.x = 70,
			.y = 10
		},

		.clicked_handler = ButtonClickedHandler
	};

	struct RDUICheckboxData checkbox_data = {
		.active = 0,
		.size = 50,
		.padding = 5,
		.color = 0x232323,
		.checkmark_color = 0xffffff,
		.position = {
			.x = 10,
			.y = 10
		},

		.activate_handler = CheckboxActivateHandler,
		.deactivate_handler = CheckboxDeactivateHandler
	};

	struct RDUIFieldData field_data = {
		.padding = 5,
		.font_size = 5,
		.min_width = 300,
		.max_width = 1000,
		.position = {
			.x = 10,
			.y = 100
		},
		.border_color = 0x000000,

		.type_handler = FieldTypeHandler
	};

	struct RDUINode* button = RDUINewButton(&button_data);
	struct RDUINode* checkbox = RDUINewCheckbox(&checkbox_data);
	struct RDUINode* field = RDUINewField(&field_data);

	RDUIPushChild(RDUIRootNode, button);
	RDUIPushChild(RDUIRootNode, checkbox);
	RDUIPushChild(RDUIRootNode, field);

	while(1) {
		CNFGHandleInput();
		CNFGClearFrame();

		CNFGColor(0x444444);

		RDUIDispatchEvent(RDUIEvent_render, NULL);

		CNFGSwapBuffers();
	}

	return 0;
}
