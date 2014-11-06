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
 * @date                 nov  5, 2014
 * @license              State
 */

#include "SimulatedAnnealingModule.h"

namespace rur {

SimulatedAnnealingModule::SimulatedAnnealingModule():
  cliParam(0)
{
  const char* const channel[4] = {"readSearchspace", "readCommand", "readCost", "writeCandidate"};
  cliParam = new Param();
  dummyCost = float(0);
}

SimulatedAnnealingModule::~SimulatedAnnealingModule() {
  delete cliParam;
}

void SimulatedAnnealingModule::Init(std::string & name) {
  cliParam->module_id = name;
  
}

std::string* SimulatedAnnealingModule::readSearchspace(bool blocking) {
  return &dummySearchspace;
}

std::string* SimulatedAnnealingModule::readCommand(bool blocking) {
  return &dummyCommand;
}

float* SimulatedAnnealingModule::readCost(bool blocking) {
  return &dummyCost;
}

bool SimulatedAnnealingModule::writeCandidate(const std::string json) {
  return true;
}

} // namespace
