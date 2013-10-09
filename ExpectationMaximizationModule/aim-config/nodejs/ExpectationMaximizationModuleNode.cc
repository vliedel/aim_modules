#define BUILDING_NODE_EXTENSION
#include <node.h>
#include <ExpectationMaximizationModuleExt.h>

using namespace v8;
using namespace rur;

void RegisterModule(Handle<Object> exports) {
  ExpectationMaximizationModuleExt::NodeRegister(exports);
}

NODE_MODULE(ExpectationMaximizationModule, RegisterModule)
