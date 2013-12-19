#define BUILDING_NODE_EXTENSION
#include <node.h>
#include <BrightnessToMorseModuleExt.h>

using namespace v8;
using namespace rur;

void RegisterModule(Handle<Object> exports) {
  BrightnessToMorseModuleExt::NodeRegister(exports);
}

NODE_MODULE(BrightnessToMorseModule, RegisterModule)
