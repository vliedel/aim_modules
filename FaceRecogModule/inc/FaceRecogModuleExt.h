/**
 * @file FaceRecogModuleExt.h
 * @brief FaceRecogModule extension
 *
 * This file is created at "DoBots". It is open-source software and part of "AI". 
 * This software is published under the GNU General Lesser Public Licence license (GPLv3).
 *
 * Copyright © 2014 Bart van Vliet <bart@almende.org>
 *
 * @author                   Bart van Vliet
 * @date                     feb  7, 2014
 * @organisation             DoBots
 * @project                  AI
 */

#include <FaceRecogModule.h>
#include <AimSerializationOpenCV.hpp>
#include <opencv2/contrib/contrib.hpp>
#include <opencv2/core/core.hpp>

namespace rur {

/**
 * Your Description of this module.
 */
class FaceRecogModuleExt: public FaceRecogModule {
public:
	//! The constructor
	FaceRecogModuleExt();

	//! The destructor
	virtual ~FaceRecogModuleExt();

	//! The tick function is the "heart" of your module, it reads and writes to the ports
	void Tick();

	//! As soon as Stop() returns "true", the FaceRecogModuleMain will stop the module
	bool Stop();

private:

	cv::Ptr<cv::FaceRecognizer> mModel;
	cv::Mat mFrame;
	int mTrain; // Determines if the module should train or predict
	int mThreshold;
	bool mTrained; // To keep up if the model has been trained, since you can't call update() before train()
	AimSerializationOpenCV mSerialization;
//	std::vector<cv::Mat> mGrayFrames;
//	std::vector<int> mLabels;
};

}

