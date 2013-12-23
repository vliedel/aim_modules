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

#ifndef BRIGHTNESSTOMORSEMODULE_H_
#define BRIGHTNESSTOMORSEMODULE_H_

#include <string>
#include <vector>
#include <vector>
#include <deque>
#include <string>


namespace rur {

struct Param {
  std::string module_id;
};

typedef std::vector<int> long_seq;

typedef std::vector<float> float_seq;

struct AndroidMorseRead_t {
  bool success;
  std::string val;
};

class BrightnessToMorseModule {
private:
  Param *cliParam;
  
  float mPortBrightnessValue;
  std::deque<float> mPortBrightnessReadBuf;
  
  std::deque<std::string> mPortMorseWriteBuf;
  
protected:
  static const int channel_count = 2;
  const char* channel[2];
public:
  // Default constructor
  BrightnessToMorseModule();
  
  // Default destructor
  virtual ~BrightnessToMorseModule();
  
  // Extend this with your own code, first call BrightnessToMorseModule::Init(name);
  void Init(std::string& name);
  
  // Function to get Param struct (to subsequently set CLI parameters)
  inline Param *GetParam() { return cliParam; }
  
  // Overwrite this function with your own code
  virtual void Tick() = 0;
  
  // Overwrite this function with your own code
  bool Stop() { return false; }
  
  // Read from this function and assume it means something
  // Remark: check if result is not NULL
  float *readBrightness(bool blocking=false);
  
  void androidWriteBrightness(float input);
  
  // Write to this function and assume it ends up at some receiving module
  bool writeMorse(const std::string output);
  
  AndroidMorseRead_t androidReadMorse();
  
};
} // End of namespace

#endif // BRIGHTNESSTOMORSEMODULE_H_
