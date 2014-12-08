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
 * @author               Homer J. Simpson
 * @copyright            Springfield Power Company
 * @date                 nov  5, 2014
 * @license              State
 */

#include "SimulatedAnnealingModule.h"
#include <SimulatedAnnealingModuleExt.h>

namespace rur {
using namespace v8;

SimulatedAnnealingModule::SimulatedAnnealingModule():
  cliParam(0)
{
  const char* const channel[4] = {"readSearchspace", "readCommand", "readCost", "writeCandidate"};
  cliParam = new Param();
  DestroyFlag = false;
  readBufSearchspace = std::deque<std::string>(0);
  readBufCommand = std::deque<std::string>(0);
  readBufCost = std::deque<std::string>(0);
  writeBufCandidate = std::deque<std::string>(0);
}

SimulatedAnnealingModule::~SimulatedAnnealingModule() {
  delete cliParam;
}

void SimulatedAnnealingModule::Init(std::string & name) {
  cliParam->module_id = name;
  
}

static void* RunModule(void* object) {
  static_cast<SimulatedAnnealingModuleExt*>(object)->Run();
  return 0;
}

void SimulatedAnnealingModule::Run() {
  while (true) {
    Tick();
  }
}

v8::Handle<v8::Value> SimulatedAnnealingModule::NodeNew(const v8::Arguments& args) {
  v8::HandleScope scope;
  if ((args.Length() < 1) || (!args[0]->IsString())) {
    return v8::ThrowException(v8::String::New("Invalid argument, must provide a string."));
  }
  SimulatedAnnealingModuleExt* obj = new SimulatedAnnealingModuleExt();
  obj->Wrap(args.This());
  
  v8::String::Utf8Value v8str(args[0]->ToString());
  std::string name = std::string(*v8str);
  obj->Init(name);
  
  pthread_mutex_init(&(obj->destroyMutex), NULL);
  
  // Init ports
  pthread_mutex_init(&(obj->readMutexSearchspace), NULL);
  
  pthread_mutex_init(&(obj->readMutexCommand), NULL);
  
  pthread_mutex_init(&(obj->readMutexCost), NULL);
  
  pthread_mutex_init(&(obj->writeMutexCandidate), NULL);
  uv_async_init(uv_default_loop() , &(obj->asyncCandidate), &(obj->CallBackCandidate));
  
  // Start the module loop
  pthread_create(&(obj->moduleThread), 0, RunModule, obj);
  
  // Make this object persistent
  obj->Ref();
  
  return args.This();
}

v8::Handle<v8::Value> SimulatedAnnealingModule::NodeDestroy(const v8::Arguments& args) {
  v8::HandleScope scope;
  SimulatedAnnealingModuleExt* obj = ObjectWrap::Unwrap<SimulatedAnnealingModuleExt>(args.This());
  return scope.Close(v8::Boolean::New(obj->Destroy()));
}

bool SimulatedAnnealingModule::Destroy() {
  bool canDestroy = true;
  if (canDestroy) {
    pthread_mutex_lock(&writeMutexCandidate);
    if (!writeBufCandidate.empty())
      canDestroy = false;
    pthread_mutex_unlock(&writeMutexCandidate);
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

void SimulatedAnnealingModule::NodeRegister(v8::Handle<v8::Object> exports) {
  v8::Local<v8::FunctionTemplate> tpl = v8::FunctionTemplate::New(NodeNew);
  tpl->SetClassName(v8::String::NewSymbol("SimulatedAnnealingModule"));
  tpl->InstanceTemplate()->SetInternalFieldCount(1);
  
  // Prototypes
  tpl->PrototypeTemplate()->Set(v8::String::NewSymbol("Destroy"), v8::FunctionTemplate::New(NodeDestroy)->GetFunction());
  tpl->PrototypeTemplate()->Set(v8::String::NewSymbol("WriteSearchspace"), v8::FunctionTemplate::New(NodeWriteSearchspace)->GetFunction());
  tpl->PrototypeTemplate()->Set(v8::String::NewSymbol("WriteCommand"), v8::FunctionTemplate::New(NodeWriteCommand)->GetFunction());
  tpl->PrototypeTemplate()->Set(v8::String::NewSymbol("WriteCost"), v8::FunctionTemplate::New(NodeWriteCost)->GetFunction());
  tpl->PrototypeTemplate()->Set(v8::String::NewSymbol("RegReadCandidate"), v8::FunctionTemplate::New(NodeRegReadCandidate)->GetFunction());
  
  v8::Persistent<v8::Function> constructor = v8::Persistent<v8::Function>::New(tpl->GetFunction());
  exports->Set(v8::String::NewSymbol("SimulatedAnnealingModule"), constructor);
}

v8::Handle<v8::Value> SimulatedAnnealingModule::NodeWriteSearchspace(const v8::Arguments& args) {
  v8::HandleScope scope;
  SimulatedAnnealingModuleExt* obj = ObjectWrap::Unwrap<SimulatedAnnealingModuleExt>(args.This());
  if (args.Length() < 1)
    return scope.Close(v8::Boolean::New(false)); // Could also throw an exception
  pthread_mutex_lock(&(obj->readMutexSearchspace));
  v8::String::Utf8Value v8str(args[0]->ToString());
  obj->readBufSearchspace.push_back(std::string(*v8str));
  pthread_mutex_unlock(&(obj->readMutexSearchspace));
  return scope.Close(v8::Boolean::New(true));
}

std::string* SimulatedAnnealingModule::readSearchspace(bool blocking) {
  pthread_mutex_lock(&destroyMutex);
  bool destroy = DestroyFlag;
  pthread_mutex_unlock(&destroyMutex);
  if (destroy)
    return NULL;
  pthread_mutex_lock(&readMutexSearchspace);
  if (readBufSearchspace.empty()) {
    pthread_mutex_unlock(&readMutexSearchspace); // Don't forget to unlock!
    return NULL;
  }
  readValSearchspace = readBufSearchspace.front();
  readBufSearchspace.pop_front();
  pthread_mutex_unlock(&readMutexSearchspace);
  return &readValSearchspace;
}

v8::Handle<v8::Value> SimulatedAnnealingModule::NodeWriteCommand(const v8::Arguments& args) {
  v8::HandleScope scope;
  SimulatedAnnealingModuleExt* obj = ObjectWrap::Unwrap<SimulatedAnnealingModuleExt>(args.This());
  if (args.Length() < 1)
    return scope.Close(v8::Boolean::New(false)); // Could also throw an exception
  pthread_mutex_lock(&(obj->readMutexCommand));
  v8::String::Utf8Value v8str(args[0]->ToString());
  obj->readBufCommand.push_back(std::string(*v8str));
  pthread_mutex_unlock(&(obj->readMutexCommand));
  return scope.Close(v8::Boolean::New(true));
}

std::string* SimulatedAnnealingModule::readCommand(bool blocking) {
  pthread_mutex_lock(&destroyMutex);
  bool destroy = DestroyFlag;
  pthread_mutex_unlock(&destroyMutex);
  if (destroy)
    return NULL;
  pthread_mutex_lock(&readMutexCommand);
  if (readBufCommand.empty()) {
    pthread_mutex_unlock(&readMutexCommand); // Don't forget to unlock!
    return NULL;
  }
  readValCommand = readBufCommand.front();
  readBufCommand.pop_front();
  pthread_mutex_unlock(&readMutexCommand);
  return &readValCommand;
}

v8::Handle<v8::Value> SimulatedAnnealingModule::NodeWriteCost(const v8::Arguments& args) {
  v8::HandleScope scope;
  SimulatedAnnealingModuleExt* obj = ObjectWrap::Unwrap<SimulatedAnnealingModuleExt>(args.This());
  if (args.Length() < 1)
    return scope.Close(v8::Boolean::New(false)); // Could also throw an exception
  pthread_mutex_lock(&(obj->readMutexCost));
  v8::String::Utf8Value v8str(args[0]->ToString());
  obj->readBufCost.push_back(std::string(*v8str));
  pthread_mutex_unlock(&(obj->readMutexCost));
  return scope.Close(v8::Boolean::New(true));
}

std::string* SimulatedAnnealingModule::readCost(bool blocking) {
  pthread_mutex_lock(&destroyMutex);
  bool destroy = DestroyFlag;
  pthread_mutex_unlock(&destroyMutex);
  if (destroy)
    return NULL;
  pthread_mutex_lock(&readMutexCost);
  if (readBufCost.empty()) {
    pthread_mutex_unlock(&readMutexCost); // Don't forget to unlock!
    return NULL;
  }
  readValCost = readBufCost.front();
  readBufCost.pop_front();
  pthread_mutex_unlock(&readMutexCost);
  return &readValCost;
}

v8::Handle<v8::Value> SimulatedAnnealingModule::NodeRegReadCandidate(const v8::Arguments& args) {
  v8::HandleScope scope;
  SimulatedAnnealingModuleExt* obj = ObjectWrap::Unwrap<SimulatedAnnealingModuleExt>(args.This());
  if (args.Length() < 1 || !args[0]->IsFunction())
    return scope.Close(v8::Boolean::New(false)); // Could also throw an exception
  v8::Local<v8::Function> callback = v8::Local<v8::Function>::Cast(args[0]);
  obj->nodeCallBackCandidate = v8::Persistent<v8::Function>::New(callback);
  return scope.Close(v8::Boolean::New(true));
}

void SimulatedAnnealingModule::CallBackCandidate(uv_async_t *handle, int status) {
  v8::HandleScope scope;
  SimulatedAnnealingModuleExt* obj = (SimulatedAnnealingModuleExt*)(handle->data);
  const unsigned argc = 1;
  while (true) {
    pthread_mutex_lock(&(obj->writeMutexCandidate));
    if (obj->writeBufCandidate.empty()) {
      pthread_mutex_unlock(&(obj->writeMutexCandidate)); // Don't forget to unlock!
      break;
    }
    v8::Local<v8::Value> argv[argc] = { v8::Local<v8::Value>::New(v8::String::New(obj->writeBufCandidate.front().c_str())) };
    obj->writeBufCandidate.pop_front();
    pthread_mutex_unlock(&(obj->writeMutexCandidate));
    if (!obj->nodeCallBackCandidate.IsEmpty())
      obj->nodeCallBackCandidate->Call(v8::Context::GetCurrent()->Global(), argc, argv);
  }
  pthread_mutex_lock(&(obj->destroyMutex));
  bool destroy = obj->DestroyFlag;
  pthread_mutex_unlock(&(obj->destroyMutex));
  if (destroy)
    obj->Destroy();
}

bool SimulatedAnnealingModule::writeCandidate(const std::string json) {
  pthread_mutex_lock(&destroyMutex);
  bool destroy = DestroyFlag;
  pthread_mutex_unlock(&destroyMutex);
  if (destroy)
    return false;
  pthread_mutex_lock(&writeMutexCandidate);
  writeBufCandidate.push_back(json);
  pthread_mutex_unlock(&writeMutexCandidate);
  asyncCandidate.data = (void*) this;
  uv_async_send(&asyncCandidate);
  return true;
}

} // namespace
