#include "RDUI.h"

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

int main(int argv, char* argc[]) {
	RDUIInit();

	return 0;
}
