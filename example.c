#include "RDUI.h"

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

int main(int argv, char* argc[]) {
	RDUIInit();

	int i;
	CNFGBGColor = 0xffffff;

	CNFGSetup("RDUI Example", 800, 600);

	while(1) {
		CNFGHandleInput();
		CNFGClearFrame();
	}

	return 0;
}
