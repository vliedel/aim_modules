#define BUILDING_NODE_EXTENSION
#include <node.h>
#include <SwiftenModuleExt.h>

using namespace v8;
using namespace rur;

void RegisterModule(Handle<Object> exports) {
  SwiftenModuleExt::NodeRegister(exports);
}

NODE_MODULE(SwiftenModule, RegisterModule)
