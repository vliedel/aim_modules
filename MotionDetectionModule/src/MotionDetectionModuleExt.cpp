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
#include <CTime.h>

#include <opencv2/imgproc/imgproc.hpp>
//#include <opencv2/highgui/highgui.hpp>
//#include <opencv2/imgproc/imgproc.hpp>

using namespace rur;

//! Replace with your own code
MotionDetectionModuleExt::MotionDetectionModuleExt() {
	mSerialization.setTag("[MotionDetection]");

	// Assume opencv 2
	// version 2.4 has a different way to set nmixtures
#if CV_VERSION_MAJOR > 3
	mBackGroundSubtractor.set("nmixtures", 3);
#else
	mBackGroundSubtractor.nmixtures = 3;
#endif

}

//! Replace with your own code
MotionDetectionModuleExt::~MotionDetectionModuleExt() {

}

//! Replace with your own code
void MotionDetectionModuleExt::Tick() {
	std::vector<int>* readVec;
	readVec = readImage(false);

	long startTime = get_cur_1ms();
	if (mSerialization.deserializeRgbImage(readVec, mFrame)) {
		long endTime = get_cur_1ms();
		std::cout << "[MotionDetection] " << "Read image in " << get_duration(startTime, endTime) << "ms" << std::endl;
		startTime = endTime;

		mBackGroundSubtractor(mFrame, mForeground);

		endTime = get_cur_1ms();
		std::cout << "[MotionDetection] " << "Got foreground image in " << get_duration(startTime, endTime) << "ms" << std::endl;
		startTime = endTime;

		cv::erode(mForeground,mForeground,cv::Mat()); //if element=Mat() , a 3 x 3 rectangular structuring element is used.

		endTime = get_cur_1ms();
		std::cout << "[MotionDetection] " << "Erode foreground image in " << get_duration(startTime, endTime) << "ms" << std::endl;
		startTime = endTime;

		//std::vector<std::vector<cv::Point> > contours;
		//cv::findContours(mForeground,contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE);

//		mBackGroundSubtractor.getBackgroundImage(mBackground);
//		cv::namedWindow("test", cv::WINDOW_AUTOSIZE);
		//cv::drawContours(mFrame,contours,-1,cv::Scalar(0,0,255),2);
//		cv::imshow("test", mForeground);
//		cv::waitKey(10);
		//cv::updateWindow("test"); // requires openGL support

		long sum=0;
		cv::Mat_<unsigned char>::const_iterator itFore = mForeground.begin<unsigned char>(), itForeEnd = mForeground.end<unsigned char>();
		for (; itFore != itForeEnd; ++itFore) {
			if (*itFore>128) // 0 for background, 127 for shadow, 255 for foreground
				sum+=*itFore;
		}
//		float probability = (float)sum/width/height/255;
		int probability = sum/mForeground.rows/mForeground.cols;

		endTime = get_cur_1ms();
		std::cout << "[MotionDetection] " << "Calculated probability in " << get_duration(startTime, endTime) << "ms" << std::endl;
		startTime = endTime;

		std::cout << "[MotionDetection] " << "sum=" << sum << " probability=" << probability << std::endl;
		writeMotion(probability);

		readVec->clear();
	}
	usleep(10*1000);
}

//! Replace with your own code
bool MotionDetectionModuleExt::Stop() {
	return false;
}

