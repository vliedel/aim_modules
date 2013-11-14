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

#include "PictureTransformModule.h"

namespace rur {

PictureTransformModule::PictureTransformModule():
  cliParam(0)
{
  const char* const channel[2] = {"readInImage", "writeOutImage"};
  cliParam = new Param();
}

PictureTransformModule::~PictureTransformModule() {
  delete cliParam;
}

void PictureTransformModule::Init(std::string & name) {
  cliParam->module_id = name;
  
}

long_seq* PictureTransformModule::readInImage(bool blocking) {
  if (mPortInImageReadBuf.empty())
    return NULL;
  mPortInImageReadBuf.front().swap(mPortInImageValue);
  mPortInImageReadBuf.pop_front();
  return &mPortInImageValue;
}

void PictureTransformModule::androidWriteInImage(long_seq in) {
  mPortInImageReadBuf.push_back(in);
}

bool PictureTransformModule::writeOutImage(const long_seq &output) {
  mPortOutImageWriteBuf.push_back(long_seq(output));
}

AndroidOutImageRead_t PictureTransformModule::androidReadOutImage() {
  AndroidOutImageRead_t ret;
  if (mPortOutImageWriteBuf.empty()) {
    ret.success = false;
    return ret;
  }
  mPortOutImageWriteBuf.front().swap(ret.val);
  ret.success = true;
  mPortOutImageWriteBuf.pop_front();
  return ret;
}

} // namespace
