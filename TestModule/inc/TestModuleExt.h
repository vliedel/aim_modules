/**
 * @file TestModuleExt.h
 * @brief TestModule extension
 *
 * This file is created at "DoBots". It is open-source software and part of "AI". 
 * This software is published under the GNU General Lesser Public Licence license (GPLv3).
 *
 * Copyright © 2014 Bart van Vliet <bart@almende.org>
 *
 * @author                   Bart van Vliet
 * @date                     okt 21, 2014
 * @organisation             DoBots
 * @project                  AI
 */

#include <TestModule.h>

namespace rur {

/**
 * Your Description of this module.
 */
class TestModuleExt: public TestModule {
public:
	//! The constructor
	TestModuleExt();

	//! The destructor
	virtual ~TestModuleExt();

	//! The tick function is the "heart" of your module, it reads and writes to the ports
	void Tick();

	//! As soon as Stop() returns "true", the TestModuleMain will stop the module
	bool Stop();
};

}

