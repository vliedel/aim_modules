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
 * @author You
 * @copyright Your Company
 * @date 21 Jun. 2013
 * @license LGPLv3
 */

#include "MonotoneSweepModule.h"

namespace rur {

MonotoneSweepModule::MonotoneSweepModule():
  portAudioBuf(0),
  portAudioVal(0),
  portInfraredBuf(0),
  portInfraredVal(0),
  cliParam(0)
{
  const char* const channel[4] = {"readAudio", "readInfrared", "writeLeftWheel", "writeRightWheels"};
  cliParam = new Param();
  pthread_mutex_init(&portAudioMutex, NULL);
  pthread_mutex_init(&portInfraredMutex, NULL);
}

MonotoneSweepModule::~MonotoneSweepModule() {
  delete cliParam;
}

void MonotoneSweepModule::Init(std::string & name) {
  cliParam->module_id = name;
  
  std::string nodeName = "monotonesweepmodule" + cliParam->module_id;
  int argc(0);
  char** argv(NULL);
  ros::init(argc, argv, nodeName, ros::init_options::NoSigintHandler);
  ros::NodeHandle rosHandle;
  ros::NodeHandle rosPrivHandle("~");
  portAudioSub = rosHandle.subscribe< std_msgs::Int32MultiArray>("portaudio", 1000, boost::bind(&MonotoneSweepModule::portAudioCB, this, _1));
  portInfraredSub = rosHandle.subscribe< std_msgs::Int32>("portinfrared", 1000, boost::bind(&MonotoneSweepModule::portInfraredCB, this, _1));
  portLeftWheelPub = rosPrivHandle.advertise< std_msgs::Int32>("portleftwheel", 1000);
  portRightWheelsPub = rosPrivHandle.advertise< std_msgs::Int32MultiArray>("portrightwheels", 1000);
}

long_seq* MonotoneSweepModule::readAudio(bool blocking) {
  ros::spinOnce();
  pthread_mutex_lock(&portAudioMutex);
  if (!portAudioBuf.empty()) {
    portAudioVal.swap(portAudioBuf.front());
    portAudioBuf.pop_front();
  }
  pthread_mutex_unlock(&portAudioMutex);
  return &portAudioVal;
}

void MonotoneSweepModule::portAudioCB(const std_msgs::Int32MultiArray::ConstPtr& msg) {
  pthread_mutex_lock(&portAudioMutex);
  long_seq read;
  portAudioBuf.push_back(read);
  std::vector< int>::const_iterator it;
  for (it=msg->data.begin(); it!=msg->data.end(); ++it) {
    portAudioBuf.back().push_back(*it);
    }
  pthread_mutex_unlock(&portAudioMutex);
}

int* MonotoneSweepModule::readInfrared(bool blocking) {
  ros::spinOnce();
  pthread_mutex_lock(&portInfraredMutex);
  if (portInfraredBuf.empty()) {
    pthread_mutex_unlock(&portInfraredMutex); // Don't forget to unlock!
    return NULL;
  }
  portInfraredVal = portInfraredBuf.front();
  portInfraredBuf.pop_front();
  pthread_mutex_unlock(&portInfraredMutex);
  return &portInfraredVal;
}

void MonotoneSweepModule::portInfraredCB(const std_msgs::Int32::ConstPtr& msg) {
  pthread_mutex_lock(&portInfraredMutex);
  portInfraredBuf.push_back(msg->data);
  pthread_mutex_unlock(&portInfraredMutex);
}

bool MonotoneSweepModule::writeLeftWheel(const int output) {
  std_msgs::Int32 msg;
  msg.data = output;
  portLeftWheelPub.publish(msg);
  return true;
}

bool MonotoneSweepModule::writeRightWheels(const long_seq &output) {
  std_msgs::Int32MultiArray msg;
  long_seq::const_iterator it;
  for (it=output.begin(); it!=output.end(); ++it) {
    msg.data.push_back(*it);
  }
  portRightWheelsPub.publish(msg);
  return true;
}

} // namespace
