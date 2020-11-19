#include "RDUI.h"

RDPoint RDUIMousePosition = {.x = 0, .y = 0};

static struct {
	struct RDUINode** array;
	size_t count;
} Nodes;

void RDUIInit() {
	Nodes.array = malloc(0);
	Nodes.count = 0;
}

struct RDUINode* RDUINewNode(
	void* data,
	RDUIEventReceiverFunction event_receiver_function
) {
	struct RDUINode* node = malloc(sizeof(struct RDUINode));

	node->data = data;
	node->event_receiver_function = event_receiver_function;

	return node;
}

void RDUIPushNode(struct RDUINode* node) {
	Nodes.count++;

	Nodes.array = realloc(Nodes.array, Nodes.count * sizeof(struct RDUINode*));
	if(Nodes.array != NULL) {
    Nodes.array[Nodes.count - 1] = node;
  } else {
    Nodes.count--;
  }
}

#define ForEachNode() for(size_t i = 0; i < Nodes.count; i++)

void RDUIDispatchEvent(enum RDUIEvent event, void* data) {
	ForEachNode() Nodes.array[i]->event_receiver_function(Nodes.array[i], event, data);
}

void RDUINoOpEventReceiver(struct RDUINode* node, enum RDUIEvent event, void* data) {}

void RDUIHandleKeyImpl(int keycode, int bDown) {
	struct RDUIEventData_key event = {.keycode = keycode, .bDown = bDown};
	RDUIDispatchEvent(RDUIEvent_key, &event);
}

void RDUIHandleButtonImpl(int x, int y, int button, int bDown) {
	struct RDUIEventData_button event = {.position = {.x = x, .y = y}, .button = button, .bDown = bDown};
	RDUIDispatchEvent(RDUIEvent_button, &event);
}

void RDUIHandleMotionImpl(int x, int y, int mask) {
	struct RDUIEventData_motion event = {.position = {.x = x, .y = y}, .mask = mask};

	RDUIMousePosition.x = x;
	RDUIMousePosition.y = y;

	RDUIDispatchEvent(RDUIEvent_motion, &event);
}
