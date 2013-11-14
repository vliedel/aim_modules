#define BUILDING_NODE_EXTENSION
#include <node.h>
#include <PictureTransformModuleExt.h>

using namespace v8;
using namespace rur;

void RegisterModule(Handle<Object> exports) {
  PictureTransformModuleExt::NodeRegister(exports);
}

NODE_MODULE(PictureTransformModule, RegisterModule)
