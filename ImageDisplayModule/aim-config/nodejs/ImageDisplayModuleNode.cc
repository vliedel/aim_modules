#define BUILDING_NODE_EXTENSION
#include <node.h>
#include <ImageDisplayModuleExt.h>

using namespace v8;
using namespace rur;

void RegisterModule(Handle<Object> exports) {
  ImageDisplayModuleExt::NodeRegister(exports);
}

NODE_MODULE(ImageDisplayModule, RegisterModule)
