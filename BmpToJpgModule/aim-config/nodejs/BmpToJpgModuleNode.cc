#define BUILDING_NODE_EXTENSION
#include <node.h>
#include <BmpToJpgModuleExt.h>

using namespace v8;
using namespace rur;

void RegisterModule(Handle<Object> exports) {
  BmpToJpgModuleExt::NodeRegister(exports);
}

NODE_MODULE(BmpToJpgModule, RegisterModule)
