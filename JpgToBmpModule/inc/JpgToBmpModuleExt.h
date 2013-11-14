/**
 * @file JpgToBmpModuleExt.h
 * @brief JpgToBmpModule extension
 *
 * This file is created at "Springfield Power Company". It is open-source software and part of "Project Bootstrap". 
 * This software is published under the State license (TR8OR).
 *
 * Copyright © 2013 Homer J. Simpson <chunkylover53@aol.com>
 *
 * @author                   Homer J. Simpson
 * @date                     nov 12, 2013
 * @organisation             Springfield Power Company
 * @project                  Project Bootstrap
 */

#include <JpgToBmpModule.h>

namespace rur {

/**
 * Your Description of this module.
 */
class JpgToBmpModuleExt: public JpgToBmpModule {
public:
	//! The constructor
	JpgToBmpModuleExt();

	//! The destructor
	virtual ~JpgToBmpModuleExt();

	//! The tick function is the "heart" of your module, it reads and writes to the ports
	void Tick();

	//! As soon as Stop() returns "true", the JpgToBmpModuleMain will stop the module
	bool Stop();
};

}

