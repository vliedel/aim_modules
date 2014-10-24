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

#ifndef TESTMODULE_H_
#define TESTMODULE_H_

#include <string>
#include <vector>
#include <string>
#include <vector>
#include <deque>
#include <node.h>
#include <pthread.h>

namespace rur {

struct Param {
  std::string module_id;
};

typedef std::vector<int> long_seq;

class TestModule : public node::ObjectWrap {
private:
  Param *cliParam;
  
  pthread_t moduleThread;
  bool DestroyFlag;
  pthread_mutex_t destroyMutex;
  
  std::deque<std::string> readBufParamDesc;
  std::string readValParamDesc;
  pthread_mutex_t readMutexParamDesc;
  
  std::deque<float> readBufError;
  float readValError;
  pthread_mutex_t readMutexError;
  
  std::deque<std::string> writeBufParam;
  v8::Persistent<v8::Function> nodeCallBackParam;
  uv_async_t asyncParam;
  pthread_mutex_t writeMutexParam;
  
  static v8::Handle<v8::Value> NodeNew(const v8::Arguments& args);
  
  static v8::Handle<v8::Value> NodeDestroy(const v8::Arguments& args);
  
  bool Destroy();
  
  // Function to be used in NodeJS, not in your C++ code
  static v8::Handle<v8::Value> NodeWriteParamDesc(const v8::Arguments& args);
  
  // Function to be used in NodeJS, not in your C++ code
  static v8::Handle<v8::Value> NodeWriteError(const v8::Arguments& args);
  
  // Function to be used in NodeJS, not in your C++ code
  static v8::Handle<v8::Value> NodeRegReadParam(const v8::Arguments& args);
  
  // Function to be used internally, not in your C++ code
  static void CallBackParam(uv_async_t *handle, int status);
  
protected:
  static const int channel_count = 3;
  const char* channel[3];
public:
  // Default constructor
  TestModule();
  
  // Default destructor
  virtual ~TestModule();
  
  // Extend this with your own code, first call TestModule::Init(name);
  void Init(std::string& name);
  
  // Function to get Param struct (to subsequently set CLI parameters)
  inline Param *GetParam() { return cliParam; }
  
  // Overwrite this function with your own code
  virtual void Tick() = 0;
  
  // Overwrite this function with your own code
  bool Stop() { return false; }
  
  void Run();
  
  // Function template for NodeJS, do not use in your own code
  static void NodeRegister(v8::Handle<v8::Object> exports);
  
  // Read from this function and assume it means something
  // Remark: check if result is not NULL
  std::string *readParamDesc(bool blocking=false);
  
  // Read from this function and assume it means something
  // Remark: check if result is not NULL
  float *readError(bool blocking=false);
  
  // Write to this function and assume it ends up at some receiving module
  bool writeParam(const std::string output);
  
};
} // End of namespace

#endif // TESTMODULE_H_
