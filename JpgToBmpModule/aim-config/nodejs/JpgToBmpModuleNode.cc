#define BUILDING_NODE_EXTENSION
#include <node.h>
#include <JpgToBmpModuleExt.h>

using namespace v8;
using namespace rur;

void RegisterModule(Handle<Object> exports) {
  JpgToBmpModuleExt::NodeRegister(exports);
}

NODE_MODULE(JpgToBmpModule, RegisterModule)
