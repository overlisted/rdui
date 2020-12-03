#pragma once

#include <stdlib.h>

#include "rawdraw/CNFG.h"

struct RDUINode;

enum RDUIEvent {
	RDUIEvent_render,
	RDUIEvent_key,
	RDUIEvent_button,
	RDUIEvent_motion
};

struct RDUIEventData_key {
	int keycode;
	int bDown;
};

struct RDUIEventData_button {
	RDPoint position;
	int button;
	int bDown;
};

struct RDUIEventData_motion {
	RDPoint position;
	int mask;
};

extern RDPoint RDUIMousePosition;

typedef void (*RDUIEventReceiverFunction)(struct RDUINode* node, enum RDUIEvent event, void* data);

struct RDUINode {
	void* data;

  RDUIEventReceiverFunction event_receiver_function;
};

#define RDUIIfEventIs(if_name) \
  struct RDUIEventData_##if_name* if_name##_event = data; \
  if(event == RDUIEvent_##if_name)

void RDUIInit();

struct RDUINode* RDUINewNode(
	void* data,
	RDUIEventReceiverFunction event_receiver_function
);

void RDUINoOpEventReceiver(struct RDUINode* node, enum RDUIEvent event, void* data);

// Handlers for rawdraw events

struct RDUIMenu {
	size_t size;
	struct RDUINode** nodes;
};

void RDUIDispatchEvent(struct RDUIMenu* menu, enum RDUIEvent event, void* data);

void RDUIHandleKeyImpl(struct RDUIMenu* menu, int keycode, int bDown);
void RDUIHandleButtonImpl(struct RDUIMenu* menu, int x, int y, int button, int bDown);
void RDUIHandleMotionImpl(struct RDUIMenu* menu, int x, int y, int mask);

struct RDUIMenu* RDUINewMenu(size_t size, ...);
void RDUIMenuPush(struct RDUIMenu* menu, struct RDUINode* node);

extern struct RDUIMenu* RDUIProcessedMenu;

#ifdef RDUI_IMPLEMENTATION
#include "rdui.c"
#endif
