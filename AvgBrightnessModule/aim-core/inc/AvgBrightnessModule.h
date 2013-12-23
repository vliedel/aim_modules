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
#include <vector>
#include <deque>
#include <string>


namespace rur {

struct Param {
  std::string module_id;
};

typedef std::vector<int> long_seq;

struct AndroidBrightnessRead_t {
  bool success;
  float val;
};

struct AndroidCommandOutRead_t {
  bool success;
  std::string val;
};

class AvgBrightnessModule {
private:
  Param *cliParam;
  
  std::string mPortImageValue;
  std::deque<std::string> mPortImageReadBuf;
  
  std::deque<float> mPortBrightnessWriteBuf;
  
  std::deque<std::string> mPortCommandOutWriteBuf;
  
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
  
  void androidWriteImage(std::string base_64);
  
  // Write to this function and assume it ends up at some receiving module
  bool writeBrightness(const float output);
  
  AndroidBrightnessRead_t androidReadBrightness();
  
  // Write to this function and assume it ends up at some receiving module
  bool writeCommandOut(const std::string output);
  
  AndroidCommandOutRead_t androidReadCommandOut();
  
};
} // End of namespace

#endif // AVGBRIGHTNESSMODULE_H_
