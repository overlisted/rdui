#define CNFG_IMPLEMENTATION

#include <RDUI.h>
#include <default-elements.h>
#include <onlyinput/onlyinput.h>
#include <stdio.h>
#include <string.h>

#include "util.h"

static struct RDUIMenu *menu;

void HandleKey(int keycode, int bDown) {
  OIHandleKey(keycode, bDown);
  RDUIHandleKeyImpl(menu, keycode, bDown);

  printf("key %d\n", keycode);
}

void HandleButton(int x, int y, int button, int bDown) {
  RDUIHandleButtonImpl(menu, x, y, button, bDown);
}

void HandleMotion(int x, int y, int mask) {
  RDUIHandleMotionImpl(menu, x, y, mask);
}

void HandleDestroy() {}

void ButtonSecondClickedHandle(struct RDUIButtonData *data) { exit(0); }

void ButtonClickedHandler(struct RDUIButtonData *data) {
  data->text = "Not again, please!";
  data->clicked_handler = ButtonSecondClickedHandle;
}

void CheckboxActivateHandler(struct RDUICheckboxData *data) {}

void CheckboxDeactivateHandler(struct RDUICheckboxData *data) {}

void OptionsBoxSelectHandler(struct RDUIOptionsBoxData *data) {}

void FieldTypeHandler(struct RDUIFieldData *data) {
  if (!strcmp(data->value, "example")) {
    UtilStringChange(data->value, "demo");
    data->cursor = 4;
  }
}

int main(int argv, char *argc[]) {
  RDUIInit();

  CNFGBGColor = 0xffffffff;

  CNFGSetup("RDUI Example", 800, 600);

  struct RDUIButtonData button_data = {.text = "Click on me!",
                                       .font_size = 6,
                                       .padding = 5,
                                       .color = 0x555555ff,
                                       .font_color = 0xffffffff,
                                       .position = {.x = 70, .y = 10},

                                       .clicked_handler = ButtonClickedHandler};

  struct RDUICheckboxData checkbox_data = {
      .active = 0,
      .size = 50,
      .padding = 5,
      .color = 0x232323ff,
      .checkmark_color = 0xffffffff,
      .position = {.x = 10, .y = 10},

      .activate_handler = CheckboxActivateHandler,
      .deactivate_handler = CheckboxDeactivateHandler};

  struct RDUIFieldData field_data = {.padding = 5,
                                     .font_size = 5,
                                     .min_width = 300,
                                     .max_width = 1000,
                                     .position = {.x = 10, .y = 100},
                                     .font_color = 0x000000ff,
                                     .border_color = 0x000000ff,
                                     .value = "A text field",
                                     .allowed_symbols = NULL,

                                     .type_handler = FieldTypeHandler};

  char *options[] = {"Pepsi", "Coca Cola",    "Fanta",
                     "Tea",   "Orange juice", NULL};

  struct RDUIOptionsBoxData options_box_data = {.position = {.x = 10, .y = 150},
                                                .color = 0x555555ff,
                                                .font_color = 0xffffffff,
                                                .font_size = 5,
                                                .padding = 5,
                                                .options = options,

                                                .select_handler =
                                                    OptionsBoxSelectHandler};

  struct RDUINode *button = RDUINewButton(&button_data);
  struct RDUINode *checkbox = RDUINewCheckbox(&checkbox_data);
  struct RDUINode *field = RDUINewField(&field_data);
  struct RDUINode *options_box = RDUINewOptionsBox(&options_box_data);

  menu = RDUINewMenu(4, button, checkbox, field, options_box);

  RDUIFocusedField = &field_data;

  while (1) {
    CNFGHandleInput();
    CNFGClearFrame();

    CNFGColor(0x444444ff);

    RDUIDispatchEvent(menu, RDUIEvent_render, NULL);
    if (options_box_data.selected_index == 2) {
      CNFGColor(0);
      CNFGPenX = 200;
      CNFGPenY = 150;

      CNFGDrawText("why", 5);
    }

    CNFGSwapBuffers();
  }

  return 0;
}
