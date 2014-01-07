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

#ifndef JPGTOBMPMODULE_H_
#define JPGTOBMPMODULE_H_

#include <string>
#include <vector>
#include <string>
#include <vector>

namespace rur {

struct Param {
  std::string module_id;
};

typedef std::vector<int> long_seq;

class JpgToBmpModule {
private:
  Param *cliParam;
  
  std::string dummyJpg;
protected:
  static const int channel_count = 2;
  const char* channel[2];
public:
  // Default constructor
  JpgToBmpModule();
  
  // Default destructor
  virtual ~JpgToBmpModule();
  
  // Extend this with your own code, first call JpgToBmpModule::Init(name);
  void Init(std::string& name);
  
  // Function to get Param struct (to subsequently set CLI parameters)
  inline Param *GetParam() { return cliParam; }
  
  // Overwrite this function with your own code
  virtual void Tick() = 0;
  
  // Overwrite this function with your own code
  bool Stop() { return false; }
  
  // Read from this function and assume it means something
  // Remark: check if result is not NULL
  std::string *readJpg(bool blocking=false);
  
  // Write to this function and assume it ends up at some receiving module
  bool writeBmp(const long_seq &output);
  
};
} // End of namespace

#endif // JPGTOBMPMODULE_H_
