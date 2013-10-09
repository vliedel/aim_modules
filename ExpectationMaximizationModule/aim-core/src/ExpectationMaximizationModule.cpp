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

#include "ExpectationMaximizationModule.h"

namespace rur {
using namespace yarp::os;

ExpectationMaximizationModule::ExpectationMaximizationModule():
  cliParam(0)
{
  const char* const channel[7] = {"readSample", "readTrain", "writeModel", "readClassify", "writeClassified", "writeGetParameters", "readSetParameters"};
  cliParam = new Param();
  portSample = new BufferedPort<Bottle>();
  portTrain = new BufferedPort<Bottle>();
  portModel = new BufferedPort<Bottle>();
  portClassify = new BufferedPort<Bottle>();
  portClassified = new BufferedPort<Bottle>();
  portGetParameters = new BufferedPort<Bottle>();
  portSetParameters = new BufferedPort<Bottle>();
}

ExpectationMaximizationModule::~ExpectationMaximizationModule() {
  delete cliParam;
  delete portSample;
  delete portTrain;
  delete portModel;
  delete portClassify;
  delete portClassified;
  delete portGetParameters;
  delete portSetParameters;
}

void ExpectationMaximizationModule::Init(std::string & name) {
  cliParam->module_id = name;
  
  std::stringstream yarpPortName;
  yarpPortName.str(""); yarpPortName.clear();
  yarpPortName << "/expectationmaximizationmodule" << name << "/sample";
  portSample->open(yarpPortName.str().c_str());
  
  yarpPortName.str(""); yarpPortName.clear();
  yarpPortName << "/expectationmaximizationmodule" << name << "/train";
  portTrain->open(yarpPortName.str().c_str());
  
  yarpPortName.str(""); yarpPortName.clear();
  yarpPortName << "/expectationmaximizationmodule" << name << "/model";
  portModel->open(yarpPortName.str().c_str());
  
  yarpPortName.str(""); yarpPortName.clear();
  yarpPortName << "/expectationmaximizationmodule" << name << "/classify";
  portClassify->open(yarpPortName.str().c_str());
  
  yarpPortName.str(""); yarpPortName.clear();
  yarpPortName << "/expectationmaximizationmodule" << name << "/classified";
  portClassified->open(yarpPortName.str().c_str());
  
  yarpPortName.str(""); yarpPortName.clear();
  yarpPortName << "/expectationmaximizationmodule" << name << "/getparameters";
  portGetParameters->open(yarpPortName.str().c_str());
  
  yarpPortName.str(""); yarpPortName.clear();
  yarpPortName << "/expectationmaximizationmodule" << name << "/setparameters";
  portSetParameters->open(yarpPortName.str().c_str());
  
}

float_seq* ExpectationMaximizationModule::readSample(bool blocking) {
  Bottle *b = portSample->read(blocking);
  if (b != NULL) {
    for (int i = 0; i < b->size(); ++i) {
      portSampleBuf.push_back(b->get(i).asDouble());
    }
  }
  return &portSampleBuf;
}

int* ExpectationMaximizationModule::readTrain(bool blocking) {
  Bottle *b = portTrain->read(blocking);
  if (b != NULL) { 
    portTrainBuf = b->get(0).asInt();
    return &portTrainBuf;
  }
  return NULL;
}

bool ExpectationMaximizationModule::writeModel(const float_seq &output) {
  Bottle &outputPrepare = portModel->prepare();
  outputPrepare.clear();
  for (int i = 0; i < output.size(); ++i) {
    outputPrepare.addDouble(output[i]);
  }
  bool forceStrict = true; // wait till previous sends are complete
  portModel->write(forceStrict);
  return true;
}

float_seq* ExpectationMaximizationModule::readClassify(bool blocking) {
  Bottle *b = portClassify->read(blocking);
  if (b != NULL) {
    for (int i = 0; i < b->size(); ++i) {
      portClassifyBuf.push_back(b->get(i).asDouble());
    }
  }
  return &portClassifyBuf;
}

bool ExpectationMaximizationModule::writeClassified(const float_seq &output) {
  Bottle &outputPrepare = portClassified->prepare();
  outputPrepare.clear();
  for (int i = 0; i < output.size(); ++i) {
    outputPrepare.addDouble(output[i]);
  }
  bool forceStrict = true; // wait till previous sends are complete
  portClassified->write(forceStrict);
  return true;
}

bool ExpectationMaximizationModule::writeGetParameters(const float_seq &output) {
  Bottle &outputPrepare = portGetParameters->prepare();
  outputPrepare.clear();
  for (int i = 0; i < output.size(); ++i) {
    outputPrepare.addDouble(output[i]);
  }
  bool forceStrict = true; // wait till previous sends are complete
  portGetParameters->write(forceStrict);
  return true;
}

float_seq* ExpectationMaximizationModule::readSetParameters(bool blocking) {
  Bottle *b = portSetParameters->read(blocking);
  if (b != NULL) {
    for (int i = 0; i < b->size(); ++i) {
      portSetParametersBuf.push_back(b->get(i).asDouble());
    }
  }
  return &portSetParametersBuf;
}

} // namespace
