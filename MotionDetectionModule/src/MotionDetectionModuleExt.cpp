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

//#include "opencv2/opencv.hpp"
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

using namespace rur;

//! Replace with your own code
MotionDetectionModuleExt::MotionDetectionModuleExt() {
	// Assume opencv 2
	// version 2.4 has a different way to set nmixtures
#if CV_VERSION_MAJOR > 3
	mBackGroundSubtractor.set("nmixtures", 3);
#else
	mBackGroundSubtractor.nmixtures = 3;
#endif


//	int height = 4;
//	int width = 8;
//	cv::Mat frame(height/2, width/2, CV_8UC3);
//
//	std::vector<int> pic;
//	int c=0;
//	for (int h=0; h<height; ++h) {
//		for (int w=0; w<width; ++w) {
//			pic.push_back(c++);
//			pic.push_back(c++);
//			pic.push_back(c++);
//		}
//	}
//	for (std::vector<int>::iterator itt=pic.begin(); itt != pic.end(); ++itt)
//		std::cout << *itt << " " << std::endl;
//	std::vector<int>::iterator it = pic.begin();
//
//	int rows=frame.rows, cols=frame.cols;
//	for (int i=0; i<rows; ++i, it+=3*width) { // skip 1 row of source = 3 subpixels * width
//		std::cout << "i=" << i << std::endl;
//		unsigned char* row = frame.ptr<unsigned char>(i);
//		for (int j=0; j<cols*3; j+=3, it+=3) { // skip 1 rgb pixel of source
//			std::cout << "j=" << j << " r=" << *it << std::endl;
//			row[j+2] = *it++;
//			row[j+1] = *it++;
//			row[j] = *it++;
//		}
//	}
//
//	std::cout << frame << std::endl;
//	cv::namedWindow("test", cv::WINDOW_AUTOSIZE);
//	cv::imshow("test", frame);
//	cv::waitKey(10);
//
//	//	cv::Mat_<cv::Vec3b>::iterator it = frame.begin<cv::Vec3b>(), itEnd = frame.end<cv::Vec3b>();
//	//	for (; it != itEnd; ++it) {
//	//		(*it)[0] = 255; // b
//	//		(*it)[1] = 0; // g
//	//		(*it)[2] = 0; // r
//	//	}
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

		long startTime = get_cur_1ms();

//#define scaleread
#ifdef scaleread
		mFrame.create(height/2, width/2, CV_8UC3);
		int cols = mFrame.cols, rows = mFrame.rows;
//		if (mFrame.isContinuous()) {
//			cols *= rows;
//			rows = 1;
//		}
		for (int i=0; i<rows; ++i, it+=3*width) {
//			std::cout << "i=" << i << std::endl;
			unsigned char* row = mFrame.ptr<unsigned char>(i);
			for (int j=0; j<cols*3; j+=3, it+=3) {
//				std::cout << "j=" << j << std::endl;
				row[j+2] = *it++;
				row[j+1] = *it++;
				row[j] = *it++;
			}
		}
		long endTime = get_cur_1ms();
		std::cout << "Read image in " << get_duration(startTime, endTime) << "ms" << std::endl;
		startTime = endTime;
//		cv::namedWindow("test", cv::WINDOW_AUTOSIZE);
//		cv::imshow("test", mFrame);
//		cv::waitKey(10);
#else

		// TODO: resize in this loop by skipping pixels
		mFrame.create(height, width, CV_8UC3); // 3 channel 8bit integer
		cv::Mat_<cv::Vec3b>::iterator itFrame = mFrame.begin<cv::Vec3b>(), itFrameEnd = mFrame.end<cv::Vec3b>();
		for (; itFrame != itFrameEnd; ++itFrame) {
			(*itFrame)[2] = *it++;
			(*itFrame)[1] = *it++;
			(*itFrame)[0] = *it++;
		}
		long endTime = get_cur_1ms();
		std::cout << "Read image in " << get_duration(startTime, endTime) << "ms" << std::endl;
		startTime = endTime;

//		cv::resize(mFrame, mFrameScaled, cv::Size(), 0.5, 0.5, cv::INTER_NEAREST);
#endif
		endTime = get_cur_1ms();
		std::cout << "Resized image in " << get_duration(startTime, endTime) << "ms" << std::endl;
		startTime = endTime;

		mBackGroundSubtractor(mFrame, mForeground);

		endTime = get_cur_1ms();
		std::cout << "Got foreground image in " << get_duration(startTime, endTime) << "ms" << std::endl;
		startTime = endTime;

		cv::erode(mForeground,mForeground,cv::Mat()); //if element=Mat() , a 3 x 3 rectangular structuring element is used.

		endTime = get_cur_1ms();
		std::cout << "Erode foreground image in " << get_duration(startTime, endTime) << "ms" << std::endl;
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

		endTime = get_cur_1ms();
		std::cout << "Calculated probability in " << get_duration(startTime, endTime) << "ms" << std::endl;
		startTime = endTime;

//		std::cout << "sum=" << sum << " probability=" << (float)sum/width/height/255 << std::endl;
		std::cout << "sum=" << sum << " probability=" << sum/mForeground.rows/mForeground.cols << std::endl;

		writeMotion(sum/mForeground.rows/mForeground.cols);

		readVec->clear();
	}
	usleep(10*1000);
}

//! Replace with your own code
bool MotionDetectionModuleExt::Stop() {
	return false;
}

