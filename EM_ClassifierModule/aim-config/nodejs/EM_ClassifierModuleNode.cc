#define BUILDING_NODE_EXTENSION
#include <node.h>
#include <EM_ClassifierModuleExt.h>

using namespace v8;
using namespace rur;

void RegisterModule(Handle<Object> exports) {
  EM_ClassifierModuleExt::NodeRegister(exports);
}

NODE_MODULE(EM_ClassifierModule, RegisterModule)
