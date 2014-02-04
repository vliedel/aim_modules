/**
 * @file MotionDetectionModuleExt.h
 * @brief MotionDetectionModule extension
 *
 * This file is created at "DoBots". It is open-source software and part of "AI". 
 * This software is published under the GNU General Lesser Public Licence license (GPLv3).
 *
 * Copyright © 2014 Bart van Vliet <bart@almende.org>
 *
 * @author                   Bart van Vliet
 * @date                     jan 28, 2014
 * @organisation             DoBots
 * @project                  AI
 */

#include <MotionDetectionModule.h>
#include "opencv2/video/background_segm.hpp"

namespace rur {

/**
 * Your Description of this module.
 */
class MotionDetectionModuleExt: public MotionDetectionModule {
public:
	//! The constructor
	MotionDetectionModuleExt();

	//! The destructor
	virtual ~MotionDetectionModuleExt();

	//! The tick function is the "heart" of your module, it reads and writes to the ports
	void Tick();

	//! As soon as Stop() returns "true", the MotionDetectionModuleMain will stop the module
	bool Stop();

private:
	cv::Mat mFrame;
	cv::Mat mFrameScaled;
	cv::Mat mBackground;
	cv::Mat mForeground;
	cv::BackgroundSubtractorMOG2 mBackGroundSubtractor;
};

}

