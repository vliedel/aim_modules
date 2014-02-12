/**
 * @file FaceDetectModuleExt.cpp
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

#include <FaceDetectModuleExt.h>
#include <unistd.h> // To use usleep()
#include <iostream>

#include <opencv2/core/core_c.h>
#include <opencv2/imgproc/imgproc.hpp>

// For testing only
#include <opencv2/highgui/highgui.hpp>


using namespace rur;

//! Replace with your own code
FaceDetectModuleExt::FaceDetectModuleExt() {
	mSerialization.setTag("[FaceDetect]");
}

//! Replace with your own code
FaceDetectModuleExt::~FaceDetectModuleExt() {

}

//! Replace with your own code
void FaceDetectModuleExt::Tick() {
	std::vector<int>* readVec;
	readVec = readImage(false);
	if (mSerialization.deserializeRgbImage(readVec, mFrame)) {
		std::cout << "[FaceDetect] height=" << mFrame.rows << " width=" << mFrame.cols << std::endl;

		// For testing only
		detectAndDraw(mFrame, mCascade, mNestedCascade, 1.0);


		//double scale = 1.0;
		std::vector<cv::Rect> faces;
		cv::Mat gray;
		//cv::Mat smallImg(cvRound(frame.rows/scale), cvRound(frame.cols/scale), CV_8UC1);
		cv::cvtColor(mFrame, gray, CV_BGR2GRAY);
		//cv::resize(gray, smallImg, smallImg.size(), 0, 0, cv::INTER_LINEAR);
		//cv::equalizeHist(smallImg, smallImg);
		cv::equalizeHist(gray, gray);
		mCascade.detectMultiScale(gray, faces, 1.1, 2, 0
					//|CV_HAAR_FIND_BIGGEST_OBJECT
					//|CV_HAAR_DO_ROUGH_SEARCH
					|CV_HAAR_SCALE_IMAGE
					, cv::Size(30, 30));

		if (!faces.empty()) {
			std::cout << "[FaceDetect] Found a face!" << std::endl;
			cv::Mat face(mFrame, faces[0]);

			std::vector<int> out;
			if (mSerialization.serializeRgbImage(out, face))
				writeFaceImage(out);
		}
		readVec->clear();
	}
	usleep(10*1000);
}

//! Replace with your own code
bool FaceDetectModuleExt::Stop() {
	return false;
}


bool FaceDetectModuleExt::loadCascade(std::string filename) {
	std::cout << "[FaceDetect] Loading " << filename << std::endl;
	if (!mCascade.load(filename)) {
		std::cerr << "[FaceDetect] ERROR: Could not load classifier cascade" << std::endl;
		return false;
	}
	return true;

}

bool FaceDetectModuleExt::loadNestedCascade(std::string filename) {
	std::cout << "[FaceDetect] Loading " << filename << std::endl;
	if (!mNestedCascade.load(filename)) {
		std::cerr << "[FaceDetect] WARNING: Could not load classifier cascade for nested objects" << std::endl;
		return false;
	}
	return true;
}


// Function to detect and draw any faces that is present in an image
void FaceDetectModuleExt::detectAndDraw(cv::Mat& img, cv::CascadeClassifier& cascade, cv::CascadeClassifier& nestedCascade, double scale) {
	if (cascade.empty()) {
		return;
	}
	int i = 0;
	double t = 0;
	std::vector<cv::Rect> faces;
	const static cv::Scalar colors[] =  { CV_RGB(0,0,255),
			CV_RGB(0,128,255),
			CV_RGB(0,255,255),
			CV_RGB(0,255,0),
			CV_RGB(255,128,0),
			CV_RGB(255,255,0),
			CV_RGB(255,0,0),
			CV_RGB(255,0,255)} ;
	cv::Mat gray, smallImg(cvRound(img.rows/scale), cvRound(img.cols/scale), CV_8UC1);

	cv::cvtColor( img, gray, CV_BGR2GRAY );
	cv::resize( gray, smallImg, smallImg.size(), 0, 0, cv::INTER_LINEAR );
	cv::equalizeHist( smallImg, smallImg );

	t = (double)cvGetTickCount();
	cascade.detectMultiScale( smallImg, faces,
			1.1, 2, 0
			//|CV_HAAR_FIND_BIGGEST_OBJECT
			//|CV_HAAR_DO_ROUGH_SEARCH
			|CV_HAAR_SCALE_IMAGE
			,
			cv::Size(30, 30) );
	t = (double)cvGetTickCount() - t;
	//printf( "detection time = %g ms\n", t/((double)cvGetTickFrequency()*1000.) );
	std::cout << "[FaceDetect] detection time = " << t/((double)cvGetTickFrequency()*1000.) << " ms" << std::endl;
	for(std::vector<cv::Rect>::const_iterator r = faces.begin(); r != faces.end(); r++, i++ )
	{
		cv::Mat smallImgROI;
		std::vector<cv::Rect> nestedObjects;
		cv::Point center;
		cv::Scalar color = colors[i%8];
		int radius;
//		center.x = cvRound((r->x + r->width*0.5)*scale);
//		center.y = cvRound((r->y + r->height*0.5)*scale);
//		radius = cvRound((r->width + r->height)*0.25*scale);
//		cv::circle( img, center, radius, color, 3, 8, 0 );

		cv::rectangle(img, *r, color, 3, 8 ,0);
		if( nestedCascade.empty() )
			continue;
		smallImgROI = smallImg(*r);
		nestedCascade.detectMultiScale( smallImgROI, nestedObjects,
				1.1, 2, 0
				//|CV_HAAR_FIND_BIGGEST_OBJECT
				//|CV_HAAR_DO_ROUGH_SEARCH
				//|CV_HAAR_DO_CANNY_PRUNING
				|CV_HAAR_SCALE_IMAGE
				,
				cv::Size(30, 30) );
		for(std::vector<cv::Rect>::const_iterator nr = nestedObjects.begin(); nr != nestedObjects.end(); nr++ )
		{
			center.x = cvRound((r->x + nr->x + nr->width*0.5)*scale);
			center.y = cvRound((r->y + nr->y + nr->height*0.5)*scale);
			radius = cvRound((nr->width + nr->height)*0.25*scale);
			cv::circle( img, center, radius, color, 3, 8, 0 );
		}
	}
	cv::imshow( "result", img );
	cv::waitKey(10);
}
