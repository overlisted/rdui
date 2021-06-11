#pragma once

#include <stdarg.h>
#include <stdlib.h>

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

struct RDUIElement;

typedef void (*RDUIEventReceiverFunction)(struct RDUIElement *element,
                                          enum RDUIEvent event, void *data);

struct RDUIElement {
  void *data;

  RDUIEventReceiverFunction event_receiver_function;
};

RDPoint RDUIMousePosition = {.x = 0, .y = 0};

#define RDUIIfEventIs(if_name)                                                 \
  struct RDUIEventData_##if_name *if_name##_event = data;                      \
  if (event == RDUIEvent_##if_name)

struct RDUIMenu *RDUIProcessedMenu = NULL;

void RDUIInit() {
  // may be removed in future
}

struct RDUIElement *
RDUINewElement(void *data, RDUIEventReceiverFunction event_receiver_function) {
  struct RDUIElement *element = malloc(sizeof(struct RDUIElement));

  element->data = data;
  element->event_receiver_function = event_receiver_function;

  return element;
}

void RDUINoOpEventReceiver(struct RDUIElement *element, enum RDUIEvent event,
                           void *data) {}

struct RDUIMenu {
  size_t size;
  struct RDUIElement **elements;
};

#define RDUIForEachElement() for (size_t i = 0; i < menu->size; i++)

static void RDUIDispatchEvent(struct RDUIMenu *menu, enum RDUIEvent event,
                       void *data) {
  RDUIProcessedMenu = menu;

  RDUIForEachElement() menu->elements[i]->event_receiver_function(menu->elements[i],
                                                            event, data);

  RDUIProcessedMenu = NULL;
}

// Handlers for rawdraw events
void RDUIRenderFrame(struct RDUIMenu *menu) {
    RDUIDispatchEvent(menu, RDUIEvent_render, NULL);
}

void RDUIHandleKeyImpl(struct RDUIMenu *menu, int keycode, int bDown) {
  struct RDUIEventData_key event = {.keycode = keycode, .bDown = bDown};
  RDUIDispatchEvent(menu, RDUIEvent_key, &event);
}

void RDUIHandleButtonImpl(struct RDUIMenu *menu, int x, int y, int button,
                          int bDown) {
  struct RDUIEventData_button event = {
      .position = {.x = x, .y = y}, .button = button, .bDown = bDown};
  RDUIDispatchEvent(menu, RDUIEvent_button, &event);
}

void RDUIHandleMotionImpl(struct RDUIMenu *menu, int x, int y, int mask) {
  struct RDUIEventData_motion event = {.position = {.x = x, .y = y},
                                       .mask = mask};

  RDUIMousePosition.x = x;
  RDUIMousePosition.y = y;

  RDUIDispatchEvent(menu, RDUIEvent_motion, &event);
}

struct RDUIMenu *RDUINewMenu(size_t size, ...) {
  va_list list;

  struct RDUIMenu *menu = malloc(sizeof(struct RDUIMenu));
  menu->size = size;
  menu->elements = malloc(sizeof(struct RDUIElement *) * size);

  va_start(list, size);
  for (int i = 0; i < size; i++)
    menu->elements[i] = va_arg(list, struct RDUIElement *);
  va_end(list);

  return menu;
}

void RDUIMenuPush(struct RDUIMenu *menu, struct RDUIElement *element) {
  menu->size++;

  menu->elements = malloc(menu->size * sizeof(struct RDUIElement *));
  if (menu->elements != NULL) {
    menu->elements[menu->size - 1] = element;
  } else {
    menu->size--;
  }
}

extern struct RDUIMenu *RDUIProcessedMenu;
