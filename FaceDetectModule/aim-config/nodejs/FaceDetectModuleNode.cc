#define BUILDING_NODE_EXTENSION
#include <node.h>
#include <FaceDetectModuleExt.h>

using namespace v8;
using namespace rur;

void RegisterModule(Handle<Object> exports) {
  FaceDetectModuleExt::NodeRegister(exports);
}

NODE_MODULE(FaceDetectModule, RegisterModule)
