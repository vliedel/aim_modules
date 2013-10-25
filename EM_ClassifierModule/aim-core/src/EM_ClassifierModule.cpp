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
 * @copyright            Almende B.V.
 * @date                 okt 25, 2013
 * @license              LGPLv3
 */

#include "EM_ClassifierModule.h"

namespace rur {

EM_ClassifierModule::EM_ClassifierModule():
  cliParam(0)
{
  const char* const channel[7] = {"readSample", "readTrain", "writeModel", "readClassify", "writeClassified", "writeGetParameters", "readSetParameters"};
  cliParam = new Param();
  dummySample = float_seq(0);
  dummyTrain = int(0);
  dummyClassify = float_seq(0);
  dummySetParameters = float_seq(0);
}

EM_ClassifierModule::~EM_ClassifierModule() {
  delete cliParam;
}

void EM_ClassifierModule::Init(std::string & name) {
  cliParam->module_id = name;
  
}

float_seq* EM_ClassifierModule::readSample(bool blocking) {
  return &dummySample;
}

int* EM_ClassifierModule::readTrain(bool blocking) {
  return &dummyTrain;
}

bool EM_ClassifierModule::writeModel(const float_seq &output) {
  return true;
}

float_seq* EM_ClassifierModule::readClassify(bool blocking) {
  return &dummyClassify;
}

bool EM_ClassifierModule::writeClassified(const float_seq &output) {
  return true;
}

bool EM_ClassifierModule::writeGetParameters(const float_seq &output) {
  return true;
}

float_seq* EM_ClassifierModule::readSetParameters(bool blocking) {
  return &dummySetParameters;
}

} // namespace
