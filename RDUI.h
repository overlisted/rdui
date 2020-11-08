#include <stdlib.h>

#include "rawdraw/CNFG.h"

struct RDUINode;

enum RDUIEvent {
	RDUIEvent_key,
	RDUIEvent_button,
	RDUIEvent_motion
};

struct RDUIEventData_key {
	int keycode;
	int bDown;
};

struct RDUIEventData_button {
	int x;
	int y;
	int button;
	int bDown;
};

struct RDUIEventData_motion {
	int x;
	int y;
	int mask;
};

typedef struct RDPoint (*RDUIRendererFunction)(struct RDUINode* node, struct RDPoint position);
typedef void (*RDUIEventReceiverFunction)(struct RDUINode* node, enum RDUIEvent event, void* data);

struct RDUINode {
	struct RDUINode* parent;

	struct RDUINode** children;
	size_t children_count;

	void* data;

	RDUIRendererFunction renderer_function;
  RDUIEventReceiverFunction event_receiver_function;
};

extern struct RDUINode* RDUIRootNode;

#define RDUIIfEventIs(if_name) \
  struct RDUIEventData_##if_name* if_name##_event = data; \
  if(RDUIEvent_##if_name)

struct RDUINode* RDUINewNode(
	struct RDUINode* parent,
	void* data,
	RDUIRendererFunction renderer_function,
	RDUIEventReceiverFunction event_receiver_function
);

void RDUIPushChild(struct RDUINode* node, struct RDUINode* child);

void RDUIRenderRoot();
void RDUIDispatchEvent(enum RDUIEvent, void* data);


// Handlers for rawdraw events

void RDUIHandleKeyImpl(int keycode, int bDown) {
	struct RDUIEventData_key event = {.keycode = keycode, .bDown = bDown};
	RDUIDispatchEvent(RDUIEvent_key, &event);
}

void RDUIHandleButtonImpl(int x, int y, int button, int bDown) {
	struct RDUIEventData_button event = {.x = x, .y = y, .button = button, .bDown = bDown};
	RDUIDispatchEvent(RDUIEvent_key, &event);
}

void RDUIHandleMotionImpl(int x, int y, int mask) {
	struct RDUIEventData_motion event = {.x = x, .y = y, .mask = mask};
	RDUIDispatchEvent(RDUIEvent_key, &event);
}
