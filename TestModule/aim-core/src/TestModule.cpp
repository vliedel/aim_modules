/**
 * This file is created at Almende B.V. It is open-source software and part of the Common 
 * Hybrid Agent Platform (CHAP). A toolbox with a lot of open-source tools, ranging from 
 * thread pools and TCP/IP components to control architectures and learning algorithms. 
 * This software is published under the GNU Lesser General Public license (LGPL).
 *
 * It is not possible to add usage restrictions to an open-source license. Nevertheless,
 * we personally strongly object against this software being used by the military, in the
 * bio-industry, for animal experimentation, or anything that violates the Universal
 * Declaration of Human Rights.
 *
 * @author               Bart van Vliet
 * @copyright            DoBots
 * @date                 okt 21, 2014
 * @license              GNU General Lesser Public Licence
 */

#include "TestModule.h"
#include <TestModuleExt.h>

namespace rur {
using namespace v8;

TestModule::TestModule():
  cliParam(0)
{
  const char* const channel[3] = {"readParamDesc", "readError", "writeParam"};
  cliParam = new Param();
  DestroyFlag = false;
  readBufParamDesc = std::deque<std::string>(0);
  readBufError = std::deque<float>(0);
  readValError = float(0);
  writeBufParam = std::deque<std::string>(0);
}

TestModule::~TestModule() {
  delete cliParam;
}

void TestModule::Init(std::string & name) {
  cliParam->module_id = name;
  
}

static void* RunModule(void* object) {
  static_cast<TestModuleExt*>(object)->Run();
}

void TestModule::Run() {
  while (true) {
    Tick();
  }
}

v8::Handle<v8::Value> TestModule::NodeNew(const v8::Arguments& args) {
  v8::HandleScope scope;
  if ((args.Length() < 1) || (!args[0]->IsString())) {
    return v8::ThrowException(v8::String::New("Invalid argument, must provide a string."));
  }
  TestModuleExt* obj = new TestModuleExt();
  obj->Wrap(args.This());
  
  v8::String::Utf8Value v8str(args[0]->ToString());
  std::string name = std::string(*v8str);
  obj->Init(name);
  
  pthread_mutex_init(&(obj->destroyMutex), NULL);
  
  // Init ports
  pthread_mutex_init(&(obj->readMutexParamDesc), NULL);
  
  pthread_mutex_init(&(obj->readMutexError), NULL);
  
  pthread_mutex_init(&(obj->writeMutexParam), NULL);
  uv_async_init(uv_default_loop() , &(obj->asyncParam), &(obj->CallBackParam));
  
  // Start the module loop
  pthread_create(&(obj->moduleThread), 0, RunModule, obj);
  
  // Make this object persistent
  obj->Ref();
  
  return args.This();
}

v8::Handle<v8::Value> TestModule::NodeDestroy(const v8::Arguments& args) {
  v8::HandleScope scope;
  TestModuleExt* obj = ObjectWrap::Unwrap<TestModuleExt>(args.This());
  return scope.Close(v8::Boolean::New(obj->Destroy()));
}

bool TestModule::Destroy() {
  bool canDestroy = true;
  if (canDestroy) {
    pthread_mutex_lock(&writeMutexParam);
    if (!writeBufParam.empty())
      canDestroy = false;
    pthread_mutex_unlock(&writeMutexParam);
  }
  if (canDestroy) {
    pthread_cancel(moduleThread);
    Unref();
    return true;
  }
  else {
    pthread_mutex_lock(&destroyMutex);
    DestroyFlag = true;
    pthread_mutex_unlock(&destroyMutex);
    return true; // return true anyway?
  }
}

void TestModule::NodeRegister(v8::Handle<v8::Object> exports) {
  v8::Local<v8::FunctionTemplate> tpl = v8::FunctionTemplate::New(NodeNew);
  tpl->SetClassName(v8::String::NewSymbol("TestModule"));
  tpl->InstanceTemplate()->SetInternalFieldCount(1);
  
  // Prototypes
  tpl->PrototypeTemplate()->Set(v8::String::NewSymbol("Destroy"), v8::FunctionTemplate::New(NodeDestroy)->GetFunction());
  tpl->PrototypeTemplate()->Set(v8::String::NewSymbol("WriteParamDesc"), v8::FunctionTemplate::New(NodeWriteParamDesc)->GetFunction());
  tpl->PrototypeTemplate()->Set(v8::String::NewSymbol("WriteError"), v8::FunctionTemplate::New(NodeWriteError)->GetFunction());
  tpl->PrototypeTemplate()->Set(v8::String::NewSymbol("RegReadParam"), v8::FunctionTemplate::New(NodeRegReadParam)->GetFunction());
  
  v8::Persistent<v8::Function> constructor = v8::Persistent<v8::Function>::New(tpl->GetFunction());
  exports->Set(v8::String::NewSymbol("TestModule"), constructor);
}

v8::Handle<v8::Value> TestModule::NodeWriteParamDesc(const v8::Arguments& args) {
  v8::HandleScope scope;
  TestModuleExt* obj = ObjectWrap::Unwrap<TestModuleExt>(args.This());
  if (args.Length() < 1)
    return scope.Close(v8::Boolean::New(false)); // Could also throw an exception
  pthread_mutex_lock(&(obj->readMutexParamDesc));
  v8::String::Utf8Value v8str(args[0]->ToString());
  obj->readBufParamDesc.push_back(std::string(*v8str));
  pthread_mutex_unlock(&(obj->readMutexParamDesc));
  return scope.Close(v8::Boolean::New(true));
}

std::string* TestModule::readParamDesc(bool blocking) {
  pthread_mutex_lock(&destroyMutex);
  bool destroy = DestroyFlag;
  pthread_mutex_unlock(&destroyMutex);
  if (destroy)
    return NULL;
  pthread_mutex_lock(&readMutexParamDesc);
  if (readBufParamDesc.empty()) {
    pthread_mutex_unlock(&readMutexParamDesc); // Don't forget to unlock!
    return NULL;
  }
  readValParamDesc = readBufParamDesc.front();
  readBufParamDesc.pop_front();
  pthread_mutex_unlock(&readMutexParamDesc);
  return &readValParamDesc;
}

v8::Handle<v8::Value> TestModule::NodeWriteError(const v8::Arguments& args) {
  v8::HandleScope scope;
  TestModuleExt* obj = ObjectWrap::Unwrap<TestModuleExt>(args.This());
  if (args.Length() < 1)
    return scope.Close(v8::Boolean::New(false)); // Could also throw an exception
  pthread_mutex_lock(&(obj->readMutexError));
  obj->readBufError.push_back(args[0]->NumberValue());
  pthread_mutex_unlock(&(obj->readMutexError));
  return scope.Close(v8::Boolean::New(true));
}

float* TestModule::readError(bool blocking) {
  pthread_mutex_lock(&destroyMutex);
  bool destroy = DestroyFlag;
  pthread_mutex_unlock(&destroyMutex);
  if (destroy)
    return NULL;
  pthread_mutex_lock(&readMutexError);
  if (readBufError.empty()) {
    pthread_mutex_unlock(&readMutexError); // Don't forget to unlock!
    return NULL;
  }
  readValError = readBufError.front();
  readBufError.pop_front();
  pthread_mutex_unlock(&readMutexError);
  return &readValError;
}

v8::Handle<v8::Value> TestModule::NodeRegReadParam(const v8::Arguments& args) {
  v8::HandleScope scope;
  TestModuleExt* obj = ObjectWrap::Unwrap<TestModuleExt>(args.This());
  if (args.Length() < 1 || !args[0]->IsFunction())
    return scope.Close(v8::Boolean::New(false)); // Could also throw an exception
  v8::Local<v8::Function> callback = v8::Local<v8::Function>::Cast(args[0]);
  obj->nodeCallBackParam = v8::Persistent<v8::Function>::New(callback);
  return scope.Close(v8::Boolean::New(true));
}

void TestModule::CallBackParam(uv_async_t *handle, int status) {
  v8::HandleScope scope;
  TestModuleExt* obj = (TestModuleExt*)(handle->data);
  const unsigned argc = 1;
  while (true) {
    pthread_mutex_lock(&(obj->writeMutexParam));
    if (obj->writeBufParam.empty()) {
      pthread_mutex_unlock(&(obj->writeMutexParam)); // Don't forget to unlock!
      break;
    }
    v8::Local<v8::Value> argv[argc] = { v8::Local<v8::Value>::New(v8::String::New(obj->writeBufParam.front().c_str())) };
    obj->writeBufParam.pop_front();
    pthread_mutex_unlock(&(obj->writeMutexParam));
    if (!obj->nodeCallBackParam.IsEmpty())
      obj->nodeCallBackParam->Call(v8::Context::GetCurrent()->Global(), argc, argv);
  }
  pthread_mutex_lock(&(obj->destroyMutex));
  bool destroy = obj->DestroyFlag;
  pthread_mutex_unlock(&(obj->destroyMutex));
  if (destroy)
    obj->Destroy();
}

bool TestModule::writeParam(const std::string output) {
  pthread_mutex_lock(&destroyMutex);
  bool destroy = DestroyFlag;
  pthread_mutex_unlock(&destroyMutex);
  if (destroy)
    return false;
  pthread_mutex_lock(&writeMutexParam);
  writeBufParam.push_back(output);
  pthread_mutex_unlock(&writeMutexParam);
  asyncParam.data = (void*) this;
  uv_async_send(&asyncParam);
  return true;
}

} // namespace
