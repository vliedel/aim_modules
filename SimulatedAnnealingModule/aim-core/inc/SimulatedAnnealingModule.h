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

#ifndef SIMULATEDANNEALINGMODULE_H_
#define SIMULATEDANNEALINGMODULE_H_

#include <string>
#include <vector>
#include <string>
#include <vector>

namespace rur {

struct Param {
  std::string module_id;
};

typedef std::vector<int> long_seq;

class SimulatedAnnealingModule {
private:
  Param *cliParam;
  
  std::string dummySearchspace;
  std::string dummyCommand;
  float dummyCost;
protected:
  static const int channel_count = 4;
  const char* channel[4];
public:
  // Default constructor
  SimulatedAnnealingModule();
  
  // Default destructor
  virtual ~SimulatedAnnealingModule();
  
  // Extend this with your own code, first call SimulatedAnnealingModule::Init(name);
  void Init(std::string& name);
  
  // Function to get Param struct (to subsequently set CLI parameters)
  inline Param *GetParam() { return cliParam; }
  
  // Overwrite this function with your own code
  virtual void Tick() = 0;
  
  // Overwrite this function with your own code
  bool Stop() { return false; }
  
  // Read from this function and assume it means something
  // Remark: check if result is not NULL
  std::string *readSearchspace(bool blocking=false);
  
  // Read from this function and assume it means something
  // Remark: check if result is not NULL
  std::string *readCommand(bool blocking=false);
  
  // Read from this function and assume it means something
  // Remark: check if result is not NULL
  float *readCost(bool blocking=false);
  
  // Write to this function and assume it ends up at some receiving module
  bool writeCandidate(const std::string json);
  
};
} // End of namespace

#endif // SIMULATEDANNEALINGMODULE_H_
