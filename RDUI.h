#pragma once

#include <stdlib.h>
#include <stdarg.h>

#include "rawdraw/CNFG.h"

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

struct RDUINode;

typedef void (*RDUIEventReceiverFunction)(struct RDUINode* node, enum RDUIEvent event, void* data);

struct RDUINode {
    void* data;

    RDUIEventReceiverFunction event_receiver_function;
};

RDPoint RDUIMousePosition = {.x = 0, .y = 0};

#define RDUIIfEventIs(if_name) \
  struct RDUIEventData_##if_name* if_name##_event = data; \
  if(event == RDUIEvent_##if_name)

struct RDUIMenu* RDUIProcessedMenu = NULL;

void RDUIInit() {
    // may be removed in future
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


void RDUINoOpEventReceiver(struct RDUINode* node, enum RDUIEvent event, void* data) {}


struct RDUIMenu {
	size_t size;
	struct RDUINode** nodes;
};


#define RDUIForEachNode() for(size_t i = 0; i < menu->size; i++)

void RDUIDispatchEvent(struct RDUIMenu* menu, enum RDUIEvent event, void* data) {
    RDUIProcessedMenu = menu;

    RDUIForEachNode() menu->nodes[i]->event_receiver_function(menu->nodes[i], event, data);

    RDUIProcessedMenu = NULL;
}

// Handlers for rawdraw events
void RDUIHandleKeyImpl(struct RDUIMenu* menu, int keycode, int bDown) {
    struct RDUIEventData_key event = {.keycode = keycode, .bDown = bDown};
    RDUIDispatchEvent(menu, RDUIEvent_key, &event);
}

void RDUIHandleButtonImpl(struct RDUIMenu* menu, int x, int y, int button, int bDown) {
    struct RDUIEventData_button event = {.position = {.x = x, .y = y}, .button = button, .bDown = bDown};
    RDUIDispatchEvent(menu, RDUIEvent_button, &event);
}

void RDUIHandleMotionImpl(struct RDUIMenu* menu, int x, int y, int mask) {
    struct RDUIEventData_motion event = {.position = {.x = x, .y = y}, .mask = mask};

    RDUIMousePosition.x = x;
    RDUIMousePosition.y = y;

    RDUIDispatchEvent(menu, RDUIEvent_motion, &event);
}

struct RDUIMenu* RDUINewMenu(size_t size, ...) {
    va_list list;

    struct RDUIMenu* menu = malloc(sizeof(struct RDUIMenu));
    menu->size = size;
    menu->nodes = malloc(sizeof(struct RDUINode*) * size);

    va_start(list, size);
    for(int i = 0; i < size; i++) menu->nodes[i] = va_arg(list, struct RDUINode*);
    va_end(list);

    return menu;
}

void RDUIMenuPush(struct RDUIMenu* menu, struct RDUINode* node) {
    menu->size++;

    menu->nodes = malloc(menu->size * sizeof(struct RDUINode*));
    if(menu->nodes != NULL) {
        menu->nodes[menu->size - 1] = node;
    } else {
        menu->size--;
    }
}

extern struct RDUIMenu* RDUIProcessedMenu;
