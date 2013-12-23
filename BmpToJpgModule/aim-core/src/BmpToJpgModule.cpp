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

#include "BmpToJpgModule.h"

namespace rur {

BmpToJpgModule::BmpToJpgModule():
  cliParam(0)
{
  const char* const channel[2] = {"readBmp", "writeJpg"};
  cliParam = new Param();
  dummyBmp = long_seq(0);
}

BmpToJpgModule::~BmpToJpgModule() {
  delete cliParam;
}

void BmpToJpgModule::Init(std::string & name) {
  cliParam->module_id = name;
  
}

long_seq* BmpToJpgModule::readBmp(bool blocking) {
  return &dummyBmp;
}

bool BmpToJpgModule::writeJpg(const std::string output) {
  return true;
}

} // namespace
