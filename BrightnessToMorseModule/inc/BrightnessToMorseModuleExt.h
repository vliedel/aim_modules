/**
 * @file BrightnessToMorseModuleExt.h
 * @brief BrightnessToMorseModule extension
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

#include <BrightnessToMorseModule.h>
#include <vector>
#include <deque>
#include "kMeans.h"

namespace rur {

/**
 * Your Description of this module.
 */
class BrightnessToMorseModuleExt: public BrightnessToMorseModule {
public:
	//! The constructor
	BrightnessToMorseModuleExt();

	//! The destructor
	virtual ~BrightnessToMorseModuleExt();

	//! The tick function is the "heart" of your module, it reads and writes to the ports
	void Tick();

	//! As soon as Stop() returns "true", the BrightnessToMorseModuleMain will stop the module
	bool Stop();

private:
	std::deque<float> mBrightnessHistory;
	float mBrightnessAvg;
	//std::vector<long> mSums;
	std::vector<float> mSums;
	std::vector<long> mTimes;
	KMeans* mKMeans;
};

}

