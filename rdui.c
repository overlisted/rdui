#include "RDUI.h"

struct RDUINode* RDUIRootNode = NULL;

void RDUIInit() {
	RDUIRootNode = RDUINewNode(NULL, NULL, RDUINoOpRenderer, RDUINoOpEventReceiver);
}

void RDUIPushChild(struct RDUINode* node, struct RDUINode* child) {
	node->children_count++;

	if(realloc(node->children, node->children_count * sizeof(void*))) {
    node->children[node->children_count - 1] = child;
  } else {
    node->children_count--;
  }
}

#define ForEachChild(node) for(size_t i = 0; i < node->children_count; i++)

static void RDUIRenderHierarchy(struct RDUINode* node, struct RDUIPosition position) {
	position = node->renderer_function(node, position);
	ForEachChild(node) RDUIRenderHierarchy(node->children[i], position);
}

void RDUIRenderRoot() {
	RDUIRenderHierarchy(RDUIRootNode, RDUIPositionOrigin);
}

static void RDUIDispatchHierarchyEvent(struct RDUINode* node, enum RDUIEvent event, void* data) {
	node->event_receiver_function(node, event, data);
	ForEachChild(node) RDUIDispatchHierarchyEvent(node->children[i], event, data);
}

void RDUIDispatchEvent(enum RDUIEvent event, void* data) {
	RDUIDispatchHierarchyEvent(RDUIRootNode, event, data);
}
