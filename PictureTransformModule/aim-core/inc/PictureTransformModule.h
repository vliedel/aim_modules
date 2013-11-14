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
 * @date                 nov  5, 2013
 * @license              State
 */

#ifndef PICTURETRANSFORMMODULE_H_
#define PICTURETRANSFORMMODULE_H_

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

struct AndroidOutImageRead_t {
  bool success;
  long_seq val;
};

class PictureTransformModule {
private:
  Param *cliParam;
  
  long_seq mPortInImageValue;
  std::deque<long_seq> mPortInImageReadBuf;
  
  std::deque<long_seq> mPortOutImageWriteBuf;
  
protected:
  static const int channel_count = 2;
  const char* channel[2];
public:
  // Default constructor
  PictureTransformModule();
  
  // Default destructor
  virtual ~PictureTransformModule();
  
  // Extend this with your own code, first call PictureTransformModule::Init(name);
  void Init(std::string& name);
  
  // Function to get Param struct (to subsequently set CLI parameters)
  inline Param *GetParam() { return cliParam; }
  
  // Overwrite this function with your own code
  virtual void Tick() = 0;
  
  // Overwrite this function with your own code
  bool Stop() { return false; }
  
  // Read from this function and assume it means something
  // Remark: caller is responsible for evoking vector->clear()
  long_seq *readInImage(bool blocking=false);
  
  void androidWriteInImage(long_seq input);
  
  // Write to this function and assume it ends up at some receiving module
  bool writeOutImage(const long_seq &output);
  
  AndroidOutImageRead_t androidReadOutImage();
  
};
} // End of namespace

#endif // PICTURETRANSFORMMODULE_H_
