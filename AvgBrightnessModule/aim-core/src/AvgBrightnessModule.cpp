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
 * @date                 dec 17, 2013
 * @license              GNU General Lesser Public Licence
 */

#include "AvgBrightnessModule.h"

namespace rur {

AvgBrightnessModule::AvgBrightnessModule():
  cliParam(0)
{
  const char* const channel[3] = {"readImage", "writeBrightness", "writeCommandOut"};
  cliParam = new Param();
}

AvgBrightnessModule::~AvgBrightnessModule() {
  delete cliParam;
}

void AvgBrightnessModule::Init(std::string & name) {
  cliParam->module_id = name;
  
}

std::string* AvgBrightnessModule::readImage(bool blocking) {
  return &dummyImage;
}

bool AvgBrightnessModule::writeBrightness(const float output) {
  return true;
}

bool AvgBrightnessModule::writeCommandOut(const std::string output) {
  return true;
}

} // namespace
