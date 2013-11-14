/**
 * @file PictureTransformModuleExt.h
 * @brief PictureTransformModule extension
 *
 * This file is created at "Springfield Power Company". It is open-source software and part of "Project Bootstrap". 
 * This software is published under the State license (TR8OR).
 *
 * Copyright © 2013 Homer J. Simpson <chunkylover53@aol.com>
 *
 * @author                   Homer J. Simpson
 * @date                     nov  5, 2013
 * @organisation             Springfield Power Company
 * @project                  Project Bootstrap
 */

#include <PictureTransformModule.h>

namespace rur {

/**
 * Your Description of this module.
 */
class PictureTransformModuleExt: public PictureTransformModule {
public:
	//! The constructor
	PictureTransformModuleExt();

	//! The destructor
	virtual ~PictureTransformModuleExt();

	//! The tick function is the "heart" of your module, it reads and writes to the ports
	void Tick();

	//! As soon as Stop() returns "true", the PictureTransformModuleMain will stop the module
	bool Stop();

private:
	bool mStop;
};

}

