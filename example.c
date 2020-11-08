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

	struct RDUINode* button = RDUINewButton(RDUIRootNode, &button_data);

	RDUIPushChild(RDUIRootNode, button);

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
