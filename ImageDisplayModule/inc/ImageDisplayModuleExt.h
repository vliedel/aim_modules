/**
 * @file ImageDisplayModuleExt.h
 * @brief ImageDisplayModule extension
 *
 * This file is created at "Springfield Power Company". It is open-source software and part of "Project Bootstrap". 
 * This software is published under the State license (TR8OR).
 *
 * Copyright © 2013 Homer J. Simpson <chunkylover53@aol.com>
 *
 * @author                   Homer J. Simpson
 * @date                     nov 14, 2013
 * @organisation             Springfield Power Company
 * @project                  Project Bootstrap
 */

#include <ImageDisplayModule.h>
#include <CImg.h>

namespace rur {

/**
 * Your Description of this module.
 */
class ImageDisplayModuleExt: public ImageDisplayModule {
public:
	//! The constructor
	ImageDisplayModuleExt();

	//! The destructor
	virtual ~ImageDisplayModuleExt();

	//! The tick function is the "heart" of your module, it reads and writes to the ports
	void Tick();

	//! As soon as Stop() returns "true", the ImageDisplayModuleMain will stop the module
	bool Stop();

private:
	cimg_library::CImgDisplay* mDisp;
};

}

