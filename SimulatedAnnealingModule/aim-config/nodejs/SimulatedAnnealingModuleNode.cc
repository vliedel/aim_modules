#define BUILDING_NODE_EXTENSION
#include <node.h>
#include <SimulatedAnnealingModuleExt.h>

using namespace v8;
using namespace rur;

void RegisterModule(Handle<Object> exports) {
  SimulatedAnnealingModuleExt::NodeRegister(exports);
}

NODE_MODULE(SimulatedAnnealingModule, RegisterModule)
