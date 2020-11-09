#include "RDUI.h"
#include "default-elements.h"

#include "stdio.h"

void HandleKey(int keycode, int bDown) {
	if(keycode == 65307) exit(0);
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

int main(int argv, char* argc[]) {
	RDUIInit();

	int i;
	CNFGBGColor = 0xffffff;

	CNFGSetup("RDUI Example", 800, 600);

	struct RDUIButtonData button_data = {
		.text = "Click on me!",
		.text_size = 10,
		.padding = 5,
		.color = 0x555555,
		.text_color = 0xffffff,
		.position = {
			.x = 200,
			.y = 200
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
			.x = 100,
			.y = 200
		},

		.activate_handler = CheckboxActivateHandler,
		.deactivate_handler = CheckboxDeactivateHandler
	};

	struct RDUINode* button = RDUINewButton(RDUIRootNode, &button_data);
	struct RDUINode* checkbox = RDUINewCheckbox(RDUIRootNode, &checkbox_data);

	RDUIPushChild(RDUIRootNode, button);
	RDUIPushChild(RDUIRootNode, checkbox);

	while(1) {
		CNFGHandleInput();
		CNFGClearFrame();

		CNFGColor(0x444444);
		CNFGTackRectangle(10, 10, 100, 100);

		RDUIDispatchEvent(RDUIEvent_render, NULL);

		CNFGSwapBuffers();
	}

	return 0;
}
