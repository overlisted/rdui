#include "RDUI.h"

struct RDUINode* RDUIRootNode = NULL;

void RDUIInit() {
}

struct RDUINode* RDUINewNode(
	struct RDUINode* parent,
	void* data,
	RDUIEventReceiverFunction event_receiver_function
) {
	struct RDUINode* node = malloc(sizeof(struct RDUINode));

	node->parent = parent;
	node->data = data;
	node->event_receiver_function = event_receiver_function;

	node->children = malloc(0);
	node->children_count = 0;

	return node;
}

void RDUIPushChild(struct RDUINode* node, struct RDUINode* child) {
	node->children_count++;

	node->children = realloc(node->children, node->children_count * sizeof(void*));
	if(node->children != NULL) {
    node->children[node->children_count - 1] = child;
  } else {
    node->children_count--;
  }
}

#define ForEachChild(node) for(size_t i = 0; i < node->children_count; i++)

static void RDUIDispatchHierarchyEvent(struct RDUINode* node, enum RDUIEvent event, void* data) {
	node->event_receiver_function(node, event, data);
	ForEachChild(node) RDUIDispatchHierarchyEvent(node->children[i], event, data);
}

void RDUIDispatchEvent(enum RDUIEvent event, void* data) {
	RDUIDispatchHierarchyEvent(RDUIRootNode, event, data);
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
	RDUIDispatchEvent(RDUIEvent_motion, &event);
}
