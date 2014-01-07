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
 * @date                 nov 12, 2013
 * @license              State
 */

#include "JpgToBmpModule.h"

namespace rur {

JpgToBmpModule::JpgToBmpModule():
  cliParam(0)
{
  const char* const channel[2] = {"readJpg", "writeBmp"};
  cliParam = new Param();
}

JpgToBmpModule::~JpgToBmpModule() {
  delete cliParam;
}

void JpgToBmpModule::Init(std::string & name) {
  cliParam->module_id = name;
  
}

std::string* JpgToBmpModule::readJpg(bool blocking) {
  return &dummyJpg;
}

bool JpgToBmpModule::writeBmp(const long_seq &output) {
  return true;
}

} // namespace
