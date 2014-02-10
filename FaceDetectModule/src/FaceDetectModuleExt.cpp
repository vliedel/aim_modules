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

#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>

using namespace rur;

//! Replace with your own code
FaceDetectModuleExt::FaceDetectModuleExt() {

}

//! Replace with your own code
FaceDetectModuleExt::~FaceDetectModuleExt() {

}

//! Replace with your own code
void FaceDetectModuleExt::Tick() {
	std::vector<int>* readVec;
	readVec = readImage(false);
	if (readVec != NULL && !readVec->empty()) {
		// -- Read the image --
		std::cout << "[FaceDetect] Received new image..." << std::endl;
		long_seq::const_iterator it = readVec->begin();
		int dataType = *it++;
		if (dataType != 0) {
			std::cerr << "[FaceDetect] WARNING: dataType=" << dataType << ", should be 0" << std::endl;
			readVec->clear();
			return;
		}
		int nTensors = *it++;
		if (nTensors != 1) {
			std::cerr << "[FaceDetect] WARNING: nTensors=" << nTensors << ", should be 1" << std::endl;
			readVec->clear();
			return;
		}
		int nDims = *it++;
		if (nDims != 3) {
			std::cerr << "[FaceDetect] WARNING: nDims=" << nDims << ", should be 3" << std::endl;
			readVec->clear();
			return;
		}
		int height = *it++;
		int width = *it++;
		int channels = *it++;
		if (channels != 3) {
			std::cerr << "[FaceDetect] WARNING: channels=" << channels << ", should be 3" << std::endl;
			readVec->clear();
			return;
		}
		if (readVec->size() < 6+height*width*channels) {
			std::cerr << "[FaceDetect] read.size=" << readVec->size() << ", should be " << 6+height*width*channels << std::endl;
			readVec->clear();
			return;
		}
		cv::Mat frame(height, width, CV_8UC3); // 3 channel 8bit integer
		//mFrame.create(height, width, CV_8UC3); // 3 channel 8bit integer
		cv::Mat_<cv::Vec3b>::iterator itFrame = frame.begin<cv::Vec3b>(), itFrameEnd = frame.end<cv::Vec3b>();
		for (; itFrame != itFrameEnd; ++itFrame) {
			(*itFrame)[2] = *it++;
			(*itFrame)[1] = *it++;
			(*itFrame)[0] = *it++;
		}
		std::cout << "[FaceDetect] height=" << height << " width=" << width << std::endl;

		detectAndDraw(frame, mCascade, mNestedCascade, 1.0);


		//double scale = 1.0;
		std::vector<cv::Rect> faces;
		cv::Mat gray;
		//cv::Mat smallImg(cvRound(frame.rows/scale), cvRound(frame.cols/scale), CV_8UC1);
		cv::cvtColor(frame, gray, CV_BGR2GRAY);
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
			cv::Mat face(frame, faces[0]);

			std::vector<int> out(6+face.rows*face.cols*3);
			std::vector<int>::iterator itOut = out.begin();
			*itOut++ = 0; // type
			*itOut++ = 1; // num tensors
			*itOut++ = 3; // num dims
			*itOut++ = face.rows; // size dim 1 (height)
			*itOut++ = face.cols; // size dim 2 (width)
			*itOut++ = 3; // size dim 3 (channels)

			cv::Mat_<cv::Vec3b>::iterator itFace = face.begin<cv::Vec3b>(), itFaceEnd = face.end<cv::Vec3b>();
			for (; itFace != itFaceEnd; ++itFace) {
				*itOut++ = (*itFace)[2];
				*itOut++ = (*itFace)[1];
				*itOut++ = (*itFace)[0];
			}

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
