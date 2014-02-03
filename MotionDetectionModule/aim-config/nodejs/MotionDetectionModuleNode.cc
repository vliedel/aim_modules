#define BUILDING_NODE_EXTENSION
#include <node.h>
#include <MotionDetectionModuleExt.h>

using namespace v8;
using namespace rur;

void RegisterModule(Handle<Object> exports) {
  MotionDetectionModuleExt::NodeRegister(exports);
}

NODE_MODULE(MotionDetectionModule, RegisterModule)
