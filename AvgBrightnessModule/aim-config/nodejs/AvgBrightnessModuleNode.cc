#define BUILDING_NODE_EXTENSION
#include <node.h>
#include <AvgBrightnessModuleExt.h>

using namespace v8;
using namespace rur;

void RegisterModule(Handle<Object> exports) {
  AvgBrightnessModuleExt::NodeRegister(exports);
}

NODE_MODULE(AvgBrightnessModule, RegisterModule)
