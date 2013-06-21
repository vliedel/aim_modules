#define BUILDING_NODE_EXTENSION
#include <node.h>
#include <MonotoneSweepModuleExt.h>

using namespace v8;
using namespace rur;

void RegisterModule(Handle<Object> exports) {
  MonotoneSweepModuleExt::NodeRegister(exports);
}

NODE_MODULE(MonotoneSweepModule, RegisterModule)
