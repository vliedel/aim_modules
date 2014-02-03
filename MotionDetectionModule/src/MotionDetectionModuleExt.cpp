/**
 * @file MotionDetectionModuleExt.cpp
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

#include <MotionDetectionModuleExt.h>
#include <unistd.h> // To use usleep()
#include <iostream>

//#include "opencv2/opencv.hpp"
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

using namespace rur;

//! Replace with your own code
MotionDetectionModuleExt::MotionDetectionModuleExt() {
	mBackGroundSubtractor.set("nmixtures", 3);

//	int height = 10;
//	int width = 20;
//	cv::Mat frame(height, width, CV_8UC3);
//	cv::Mat_<cv::Vec3b>::iterator it = frame.begin<cv::Vec3b>(), itEnd = frame.end<cv::Vec3b>();
//	for (; it != itEnd; ++it) {
//		(*it)[0] = 255; // b
//		(*it)[1] = 0; // g
//		(*it)[2] = 0; // r
//	}
//
//	std::cout << frame << std::endl;
//	cv::namedWindow("test", cv::WINDOW_AUTOSIZE);
//	cv::imshow("test", frame);
//	cv::waitKey(0);

}

//! Replace with your own code
MotionDetectionModuleExt::~MotionDetectionModuleExt() {

}

//! Replace with your own code
void MotionDetectionModuleExt::Tick() {
	std::vector<int>* readVec;
	readVec = readImage(false);
	if (readVec != NULL && !readVec->empty()) {
		// -- Read the image --
		std::cout << "Received new image..." << std::endl;
		long_seq::const_iterator it = readVec->begin();
		int dataType = *it++;
		if (dataType != 0)
			return;
		int nArrays = *it++;
		int nDims = *it++;
		if (nDims != 3) {
			std::cerr << "nDims=" << nDims << ", should be 3" << std::endl;
			readVec->clear();
			return;
		}
		int height = *it++;
		int width = *it++;
		int channels = *it++;
		if (channels != 3) {
			std::cerr << "channels=" << channels << ", should be 3" << std::endl;
			readVec->clear();
			return;
		}
		if (readVec->size() < 6+height*width*channels) {
			std::cerr << "read.size=" << readVec->size() << ", should be " << 6+height*width*channels << std::endl;
			readVec->clear();
			return;
		}
		mFrame.create(height, width, CV_8UC3); // 3 channel 8bit integer
		cv::Mat_<cv::Vec3b>::iterator itFrame = mFrame.begin<cv::Vec3b>(), itFrameEnd = mFrame.end<cv::Vec3b>();
		for (; itFrame != itFrameEnd; ++itFrame) {
			(*itFrame)[2] = *it++;
			(*itFrame)[1] = *it++;
			(*itFrame)[0] = *it++;
		}

		mBackGroundSubtractor(mFrame, mForeground);
		cv::erode(mForeground,mForeground,cv::Mat()); //if element=Mat() , a 3 x 3 rectangular structuring element is used.

		//std::vector<std::vector<cv::Point> > contours;
		//cv::findContours(mForeground,contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE);

		mBackGroundSubtractor.getBackgroundImage(mBackground);

//		cv::namedWindow("test", cv::WINDOW_AUTOSIZE);
		//cv::drawContours(mFrame,contours,-1,cv::Scalar(0,0,255),2);
//		cv::imshow("test", mForeground);
//		cv::waitKey(10);
		//cv::updateWindow("test"); // requires openGL support

		long sum=0;
		cv::Mat_<unsigned char>::iterator itFore = mForeground.begin<unsigned char>(), itForeEnd = mForeground.end<unsigned char>();
		for (; itFore != itForeEnd; ++itFore) {
			if (*itFore>128) // 0 for background, 127 for shadow, 255 for foreground
				sum+=*itFore;
		}

//		std::cout << "sum=" << sum << " probability=" << (float)sum/width/height/255 << std::endl;
		std::cout << "sum=" << sum << " probability=" << sum/width/height << std::endl;

		writeMotion(sum/width/height);

		readVec->clear();
	}
	usleep(10*1000);
}

//! Replace with your own code
bool MotionDetectionModuleExt::Stop() {
	return false;
}

