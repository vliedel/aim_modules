#define BUILDING_NODE_EXTENSION
#include <node.h>
#include <MonotoneDecompositionModuleExt.h>

using namespace v8;
using namespace rur;

void RegisterModule(Handle<Object> exports) {
  MonotoneDecompositionModuleExt::NodeRegister(exports);
}

NODE_MODULE(MonotoneDecompositionModule, RegisterModule)
