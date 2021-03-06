/**
 * @file FaceRecogModuleExt.cpp
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

#include <FaceRecogModuleExt.h>
#include <unistd.h> // To use usleep()
#include <iostream>
#include <boost/property_tree/ptree.hpp>		// For command port
#include <boost/property_tree/json_parser.hpp>	// For command port

using namespace rur;

FaceRecogModuleExt::FaceRecogModuleExt():
		mTrain(false)
		,mThreshold(INT_MAX)
//		,mTrained(false)
//		,mGrayFrames(0)
//		,mLabels(0)
{
	mSerialization.setTag("[FaceRecog]");
	// Use local binary patterns histogram algorithm, only one that can be updated with new images
	// Set a threshold, use defaults for other parameters.
	mModel = cv::createLBPHFaceRecognizer(1, 8, 8, 8, DBL_MAX);
	if (mModel.empty())
		std::cerr << "[FaceRecog] could not init model!" << std::endl;
}

FaceRecogModuleExt::~FaceRecogModuleExt() {

}

void FaceRecogModuleExt::Tick() {
	std::vector<int>* readVec;
	readVec = readFaceImage(false);
	if (mSerialization.deserializeRgbImage(readVec, mFrame)) {
		std::cout << "[FaceRecog] height=" << mFrame.rows << " width=" << mFrame.cols << std::endl;

		// Grayscale required
		cv::Mat gray;
		cv::cvtColor(mFrame, gray, CV_BGR2GRAY);

		cv::Mat grayResized;
		cv::resize(gray, grayResized, cv::Size(64, 64), 0, 0);

		if (mTrain > 0) {
			// train() and update() expect a vector of frames, operator= doesn't copy
			std::vector<cv::Mat> frames(1);
			std::vector<int> labels(1);
			frames[0] = grayResized;
			labels[0] = mTrain;

			if (!mTrained) {
				mModel->train(frames, labels);
				mTrained = true;
			}
			else {
				mModel->update(frames, labels);
			}

//			mGrayFrames.push_back(gray);
//			mLabels.push_back(mLabels.size());
//
//			if (mGrayFrames.size() > 1) {
//
//				if (!mTrained) {
//					mModel->train(mGrayFrames, mLabels);
//					mTrained = true;
//				}
//				else {
//					mModel->update(mGrayFrames, mLabels);
//				}
//				mGrayFrames.clear();
//				mLabels.clear();
//			}
		}
		else {
			//int label = mModel->predict(gray);
			//std::cout << "[FaceRecog] Prediction=" << label << std::endl;
			int label;
			double confidence;
			mModel->predict(grayResized, label, confidence);
			std::cout << "[FaceRecog] Prediction=" << label << " confidence=" << confidence << std::endl;
			// 40 or 50 seems an ok threshold
		}

		readVec->clear();
	}


	std::string* readString = readCmd(false);
	if (readString != NULL && !readString->empty()) {

		int train;
		int threshold;

		boost::property_tree::ptree pt;
		std::stringstream ss;
		ss << *readString;
		try {
			boost::property_tree::read_json(ss,pt);
//			boost::property_tree::basic_ptree<std::string,std::string>::const_iterator iter = pt.begin(),iterEnd = pt.end();
//			for(;iter != iterEnd;++iter){
//				if (iter->first == "identifier") {
//					//	       same thing
//				} else if (iter->first == "server") {
//					record.host = pt.get<std::string>(iter->first);
//				} else if (iter->first == "port") {
//					record.port = pt.get<std::string>(iter->first);
//				} else if (iter->first == "pid") {
//					record.pid =pt.get<std::string>(iter->first);
//				}
//			}
			train = pt.get("train", -1);
			threshold = pt.get("threshold", -1);
		}
		catch (std::exception &e) {
			std::cerr << "[FaceRecog] " << "Error: Unable to parse json command: " << e.what() << std::endl;
		}

		if (train > -1 && train != mTrain) {
			mTrain = train;
			std::cout << "[FaceRecog] " << "Set train to " << mTrain << std::endl;
			if (train > 0) {
				// Reset training data
			}
		}

		if (threshold > 0) {
			mThreshold = threshold;
			// Assume opencv 2
			// version 2.4 has a different way to set parameters
#if CV_VERSION_MAJOR > 3
			mModel->set("threshold", threshold);
#else
			mModel.threshold = threshold;
#endif
			std::cout << "[FaceRecog] " << "Set threshold to " << mThreshold << std::endl;
		}

		readString->clear();
	}


	usleep(10*1000);
}

bool FaceRecogModuleExt::Stop() {
	return false;
}

