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

#ifndef EXPECTATIONMAXIMIZATIONMODULE_H_
#define EXPECTATIONMAXIMIZATIONMODULE_H_

#include <string>
#include <vector>
#include <string>
#include <vector>
#include <sstream>
#include <yarp/os/BufferedPort.h>
#include <yarp/os/Network.h>
#include <yarp/os/Bottle.h>

namespace rur {

struct Param {
  std::string module_id;
};

typedef std::vector<int> long_seq;

typedef std::vector<float> float_seq;

typedef std::vector<char> char_seq;

class ExpectationMaximizationModule {
private:
  Param *cliParam;
  
  yarp::os::Network yarp;
  float_seq portSampleBuf;
  yarp::os::BufferedPort<yarp::os::Bottle> *portSample;
  int portTrainBuf;
  yarp::os::BufferedPort<yarp::os::Bottle> *portTrain;
  yarp::os::BufferedPort<yarp::os::Bottle> *portModel;
  float_seq portClassifyBuf;
  yarp::os::BufferedPort<yarp::os::Bottle> *portClassify;
  yarp::os::BufferedPort<yarp::os::Bottle> *portClassified;
  yarp::os::BufferedPort<yarp::os::Bottle> *portGetParameters;
  float_seq portSetParametersBuf;
  yarp::os::BufferedPort<yarp::os::Bottle> *portSetParameters;
protected:
  static const int channel_count = 7;
  const char* channel[7];
  // Read from this function and assume it means something
  // Remark: caller is responsible for evoking vector->clear()
  float_seq *readSample(bool blocking=false);
  
  // Read from this function and assume it means something
  // Remark: check if result is not NULL
  int *readTrain(bool blocking=false);
  
  // Write to this function and assume it ends up at some receiving module
  bool writeModel(const float_seq &output);
  
  // Read from this function and assume it means something
  // Remark: caller is responsible for evoking vector->clear()
  float_seq *readClassify(bool blocking=false);
  
  // Write to this function and assume it ends up at some receiving module
  bool writeClassified(const float_seq &output);
  
  // Write to this function and assume it ends up at some receiving module
  bool writeGetParameters(const char_seq &output);
  
  // Read from this function and assume it means something
  // Remark: caller is responsible for evoking vector->clear()
  float_seq *readSetParameters(bool blocking=false);
  
public:
  // Default constructor
  ExpectationMaximizationModule();
  
  // Default destructor
  virtual ~ExpectationMaximizationModule();
  
  // Extend this with your own code, first call ExpectationMaximizationModule::Init(name);
  void Init(std::string& name);
  
  // Function to get Param struct (to subsequently set CLI parameters)
  inline Param *GetParam() { return cliParam; }
  
  // Overwrite this function with your own code
  virtual void Tick() = 0;
  
  // Overwrite this function with your own code
  bool Stop() { return false; }
  
};
} // End of namespace

#endif // EXPECTATIONMAXIMIZATIONMODULE_H_
