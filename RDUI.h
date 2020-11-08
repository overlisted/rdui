#include <stdlib.h>

#include "rawdraw/CNFG.h"

struct RDUINode;

enum RDUIEvent {
	RDUIEvent_click
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
  struct RDUIEvent_##if_name##_Data* if_name##_event = data; \
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

