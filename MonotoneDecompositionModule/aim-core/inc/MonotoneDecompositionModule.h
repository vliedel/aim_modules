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
 */

#ifndef MONOTONEDECOMPOSITIONMODULE_H_
#define MONOTONEDECOMPOSITIONMODULE_H_

#include <string>
#include <vector>
#include <deque>
#include <pthread.h>
// ros specific headers
#include <ros/ros.h>
#include <std_msgs/Float64MultiArray.h>
#include <std_msgs/Int32MultiArray.h>
#include <std_msgs/Int16.h>
#include <std_msgs/Int16MultiArray.h>
#include <std_msgs/Float32MultiArray.h>
#include <std_msgs/Int32.h>
#include <std_msgs/Float64.h>
#include <std_msgs/Float32.h>

namespace rur {

struct Param {
  std::string module_id;
  int parameter;
};

typedef std::vector<int> long_seq;

class MonotoneDecompositionModule {
private:
  Param *cliParam;
  
  ros::Subscriber portAudioSub;
  std::deque<long_seq> portAudioBuf;
  long_seq portAudioVal;
  pthread_mutex_t portAudioMutex;
  
  ros::Subscriber portInfraredSub;
  std::deque<int> portInfraredBuf;
  int portInfraredVal;
  pthread_mutex_t portInfraredMutex;
  
  ros::Publisher portLeftWheelPub;
  
  void portAudioCB(const std_msgs::Int32MultiArray::ConstPtr& msg);
  void portInfraredCB(const std_msgs::Int32::ConstPtr& msg);
protected:
  static const int channel_count = 3;
  const char* channel[3];
  // Read from this function and assume it means something
  // Remark: caller is responsible for evoking vector->clear()
  long_seq *readAudio(bool blocking=false);
  
  // Read from this function and assume it means something
  // Remark: check if result is not NULL
  int *readInfrared(bool blocking=false);
  
  // Write to this function and assume it ends up at some receiving module
  bool writeLeftWheel(const int output);
  
public:
  MonotoneDecompositionModule();
  
  ~MonotoneDecompositionModule();
  
  // Extend this with your own code, first call MonotoneDecompositionModule::Init(name);
  void Init(std::string& name);
  
  // Function to get Param struct (to subsequently set CLI parameters)
  inline Param *GetParam() { return cliParam; }
  
  // Overwrite this function with your own code
  virtual void Tick() {}
  
  // Overwrite this function with your own code
  bool Stop() { return false; }
  
};
} // End of namespace

#endif // MONOTONEDECOMPOSITIONMODULE_H_
