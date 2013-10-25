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
 * @copyright            Almende B.V.
 * @date                 okt 25, 2013
 * @license              LGPLv3
 */

#ifndef EM_CLASSIFIERMODULE_H_
#define EM_CLASSIFIERMODULE_H_

#include <string>
#include <vector>
#include <string>
#include <vector>

namespace rur {

struct Param {
  std::string module_id;
};

typedef std::vector<int> long_seq;

typedef std::vector<float> float_seq;

typedef std::vector<char> char_seq;

class EM_ClassifierModule {
private:
  Param *cliParam;
  
  float_seq dummySample;
  int dummyTrain;
  float_seq dummyClassify;
  float_seq dummySetParameters;
protected:
  static const int channel_count = 7;
  const char* channel[7];
public:
  // Default constructor
  EM_ClassifierModule();
  
  // Default destructor
  virtual ~EM_ClassifierModule();
  
  // Extend this with your own code, first call EM_ClassifierModule::Init(name);
  void Init(std::string& name);
  
  // Function to get Param struct (to subsequently set CLI parameters)
  inline Param *GetParam() { return cliParam; }
  
  // Overwrite this function with your own code
  virtual void Tick() = 0;
  
  // Overwrite this function with your own code
  bool Stop() { return false; }
  
  // Read from this function and assume it means something
  // Remark: caller is responsible for evoking vector->clear()
  float_seq *readSample(bool blocking=false);
  
  // Read from this function and assume it means something
  // Remark: check if result is not NULL
  int *readTrain(bool blocking=false);
  
  // Write to this function and assume it ends up at some receiving module
  bool writeModel(const float_seq &output);
  
  // Read from this function and assume it means something
  // Remark: caller is responsible for evoking vector->clear()
  float_seq *readClassify(bool blocking=false);
  
  // Write to this function and assume it ends up at some receiving module
  bool writeClassified(const float_seq &output);
  
  // Write to this function and assume it ends up at some receiving module
  bool writeGetParameters(const float_seq &output);
  
  // Read from this function and assume it means something
  // Remark: caller is responsible for evoking vector->clear()
  float_seq *readSetParameters(bool blocking=false);
  
};
} // End of namespace

#endif // EM_CLASSIFIERMODULE_H_
