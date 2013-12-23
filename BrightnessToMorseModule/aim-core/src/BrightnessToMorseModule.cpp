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
 * @date                 dec 19, 2013
 * @license              GNU General Lesser Public Licence
 */

#include "BrightnessToMorseModule.h"

namespace rur {

BrightnessToMorseModule::BrightnessToMorseModule():
  cliParam(0)
{
  const char* const channel[2] = {"readBrightness", "writeMorse"};
  cliParam = new Param();
}

BrightnessToMorseModule::~BrightnessToMorseModule() {
  delete cliParam;
}

void BrightnessToMorseModule::Init(std::string & name) {
  cliParam->module_id = name;
  
}

float* BrightnessToMorseModule::readBrightness(bool blocking) {
  if (mPortBrightnessReadBuf.empty())
    return NULL;
  mPortBrightnessValue = mPortBrightnessReadBuf.front();
  mPortBrightnessReadBuf.pop_front();
  return &mPortBrightnessValue;
}

void BrightnessToMorseModule::androidWriteBrightness(float in) {
  mPortBrightnessReadBuf.push_back(in);
}

bool BrightnessToMorseModule::writeMorse(const std::string output) {
  mPortMorseWriteBuf.push_back(output);
}

AndroidMorseRead_t BrightnessToMorseModule::androidReadMorse() {
  AndroidMorseRead_t ret;
  if (mPortMorseWriteBuf.empty()) {
    ret.success = false;
    return ret;
  }
  ret.val = mPortMorseWriteBuf.front();
  ret.success = true;
  mPortMorseWriteBuf.pop_front();
  return ret;
}

} // namespace
