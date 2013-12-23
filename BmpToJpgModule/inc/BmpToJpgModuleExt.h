/**
 * @file BmpToJpgModuleExt.h
 * @brief BmpToJpgModule extension
 *
 * This file is created at "DoBots". It is open-source software and part of "AI". 
 * This software is published under the GNU General Lesser Public Licence license (GPLv3).
 *
 * Copyright © 2013 Bart van Vliet <bart@almende.org>
 *
 * @author                   Bart van Vliet
 * @date                     dec 19, 2013
 * @organisation             DoBots
 * @project                  AI
 */

#include <BmpToJpgModule.h>

namespace rur {

/**
 * Your Description of this module.
 */
class BmpToJpgModuleExt: public BmpToJpgModule {
public:
	//! The constructor
	BmpToJpgModuleExt();

	//! The destructor
	virtual ~BmpToJpgModuleExt();

	//! The tick function is the "heart" of your module, it reads and writes to the ports
	void Tick();

	//! As soon as Stop() returns "true", the BmpToJpgModuleMain will stop the module
	bool Stop();
};

}

