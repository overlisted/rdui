#pragma once

#include <stdlib.h>

#include "rawdraw/CNFG.h"

struct RDUINode;

struct RDUIPosition {
	int x;
	int y;
};

static struct RDUIPosition RDUIPositionOrigin = {.x = 0, .y = 0};

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
	struct RDUIPosition position;
	int button;
	int bDown;
};

struct RDUIEventData_motion {
	struct RDUIPosition position;
	int mask;
};

typedef void (*RDUIEventReceiverFunction)(struct RDUINode* node, enum RDUIEvent event, void* data);

struct RDUINode {
	struct RDUINode* parent;

	struct RDUINode** children;
	size_t children_count;

	void* data;

  RDUIEventReceiverFunction event_receiver_function;
};

#define RDUIIfEventIs(if_name) \
  struct RDUIEventData_##if_name* if_name##_event = data; \
  if(event == RDUIEvent_##if_name)

extern struct RDUINode* RDUIRootNode;

void RDUIInit();

struct RDUINode* RDUINewNode(
	void* data,
	RDUIEventReceiverFunction event_receiver_function
);

void RDUIPushChild(struct RDUINode* node, struct RDUINode* child);

void RDUIDispatchEvent(enum RDUIEvent event, void* data);
void RDUINoOpEventReceiver(struct RDUINode* node, enum RDUIEvent event, void* data);

// Handlers for rawdraw events

void RDUIHandleKeyImpl(int keycode, int bDown);
void RDUIHandleButtonImpl(int x, int y, int button, int bDown);
void RDUIHandleMotionImpl(int x, int y, int mask);
