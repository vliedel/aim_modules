#define BUILDING_NODE_EXTENSION
#include <node.h>
#include <FaceRecogModuleExt.h>

using namespace v8;
using namespace rur;

void RegisterModule(Handle<Object> exports) {
  FaceRecogModuleExt::NodeRegister(exports);
}

NODE_MODULE(FaceRecogModule, RegisterModule)
