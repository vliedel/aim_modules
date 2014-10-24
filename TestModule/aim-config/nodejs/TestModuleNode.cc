#define BUILDING_NODE_EXTENSION
#include <node.h>
#include <TestModuleExt.h>

using namespace v8;
using namespace rur;

void RegisterModule(Handle<Object> exports) {
  TestModuleExt::NodeRegister(exports);
}

NODE_MODULE(TestModule, RegisterModule)
