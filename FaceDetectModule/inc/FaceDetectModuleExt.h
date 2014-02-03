/**
 * @file FaceDetectModuleExt.h
 * @brief FaceDetectModule extension
 *
 * This file is created at "DoBots". It is open-source software and part of "AI". 
 * This software is published under the GNU General Lesser Public Licence license (GPLv3).
 *
 * Copyright © 2014 Bart van Vliet <bart@almende.org>
 *
 * @author                   Bart van Vliet
 * @date                     jan 31, 2014
 * @organisation             DoBots
 * @project                  AI
 */

#include <FaceDetectModule.h>
#include <opencv2/opencv.hpp>

namespace rur {

/**
 * Your Description of this module.
 */
class FaceDetectModuleExt: public FaceDetectModule {
public:
	//! The constructor
	FaceDetectModuleExt();

	//! The destructor
	virtual ~FaceDetectModuleExt();

	//! The tick function is the "heart" of your module, it reads and writes to the ports
	void Tick();

	//! As soon as Stop() returns "true", the FaceDetectModuleMain will stop the module
	bool Stop();

	bool loadCascade(std::string name);
	bool loadNestedCascade(std::string name);


private:
	std::string mCascadeName, mNestedCascadeName;
	cv::CascadeClassifier mCascade, mNestedCascade;

	void detectAndDraw(cv::Mat& img, cv::CascadeClassifier& cascade, cv::CascadeClassifier& nestedCascade, double scale);
};

}

