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

#ifndef AVGBRIGHTNESSMODULE_H_
#define AVGBRIGHTNESSMODULE_H_

#include <string>
#include <vector>
#include <string>
#include <vector>

namespace rur {

struct Param {
  std::string module_id;
};

typedef std::vector<int> long_seq;

class AvgBrightnessModule {
private:
  Param *cliParam;
  
  std::string dummyImage;
protected:
  static const int channel_count = 3;
  const char* channel[3];
public:
  // Default constructor
  AvgBrightnessModule();
  
  // Default destructor
  virtual ~AvgBrightnessModule();
  
  // Extend this with your own code, first call AvgBrightnessModule::Init(name);
  void Init(std::string& name);
  
  // Function to get Param struct (to subsequently set CLI parameters)
  inline Param *GetParam() { return cliParam; }
  
  // Overwrite this function with your own code
  virtual void Tick() = 0;
  
  // Overwrite this function with your own code
  bool Stop() { return false; }
  
  // Read from this function and assume it means something
  // Remark: check if result is not NULL
  std::string *readImage(bool blocking=false);
  
  // Write to this function and assume it ends up at some receiving module
  bool writeBrightness(const float output);
  
  // Write to this function and assume it ends up at some receiving module
  bool writeCommandOut(const std::string output);
  
};
} // End of namespace

#endif // AVGBRIGHTNESSMODULE_H_
