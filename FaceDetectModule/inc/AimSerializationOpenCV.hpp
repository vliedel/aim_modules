/*
 * AimSerializationOpenCV.hpp
 *
 *  Created on: Feb 11, 2014
 *      Author: vliedel
 */

#ifndef AIMSERIALIZATIONOPENCV_HPP_
#define AIMSERIALIZATIONOPENCV_HPP_

#include <AimSerializationHeader.hpp>
#include <string>
#include <iostream>
#include <opencv2/core/core.hpp>

namespace rur {

class AimSerializationOpenCV {
public:
	AimSerializationOpenCV() {}
	~AimSerializationOpenCV() {}

	void setTag(std::string tag) {
		mTag = tag;
		mSerializationHeader.setTag(mTag);
	}

	// Read an RGB image (8 bit) from a vector
	// Clears the vector on error
	bool deserializeRgbImage(std::vector<int>* portData, cv::Mat& img) {
		std::vector<int>::const_iterator it;
		int height, width;
		if (!mSerializationHeader.deserializeRgbImage(portData, height, width, it))
			return false;

//		// Scale image
//		img.create(height/2, width/2, CV_8UC3);
//		int cols = img.cols, rows = img.rows;
//	//		if (mFrame.isContinuous()) {
//	//			cols *= rows;
//	//			rows = 1;
//	//		}
//		for (int i=0; i<rows; ++i, it+=3*width) {
//			unsigned char* row = img.ptr<unsigned char>(i);
//			for (int j=0; j<cols*3; j+=3, it+=3) {
//				row[j+2] = *it++;
//				row[j+1] = *it++;
//				row[j] = *it++;
//			}
//		}

		img.create(height, width, CV_8UC3); // 3 channel 8bit integer
		cv::Mat_<cv::Vec3b>::iterator itImg = img.begin<cv::Vec3b>(), itImgEnd = img.end<cv::Vec3b>();
		for (; itImg != itImgEnd; ++itImg) {
			(*itImg)[2] = *it++;
			(*itImg)[1] = *it++;
			(*itImg)[0] = *it++;
		}

		return true;
	}

	// Read a gray scale image (8bit) from a vector
	// Clears the vector on error
	bool deserializeGrayImage(std::vector<int>* portData, cv::Mat& img) {
		std::vector<int>::const_iterator it;
		int height, width;
		if (!mSerializationHeader.deserializeGrayImage(portData, height, width, it))
			return false;

		img.create(height, width, CV_8U); // 1 channel 8bit integer
		cv::Mat_<unsigned char>::iterator itImg = img.begin<unsigned char>(), itImgEnd = img.end<unsigned char>();
		for (; itImg != itImgEnd; ) {
			*itImg++ = *it++;
		}

		return true;
	}

	// Write an RGB (CV_8UC3) image to a vector
	bool serializeRgbImage(std::vector<int>& portData, const cv::Mat& img) {
		if (img.channels() != 3) {
			std::cerr << mTag << " WARNING: image should be an RGB (CV_8UC3) image" << std::endl;
			return false;
		}

		std::vector<int>::iterator itOut;
		if (!mSerializationHeader.serializeRgbImage(portData, img.rows, img.cols, itOut))
			return false;

		cv::Mat_<cv::Vec3b>::const_iterator itImg = img.begin<cv::Vec3b>(), itImgEnd = img.end<cv::Vec3b>();
		for (; itImg != itImgEnd; ++itImg) {
			*itOut++ = (*itImg)[2];
			*itOut++ = (*itImg)[1];
			*itOut++ = (*itImg)[0];
		}

		return true;
	}

	// Write a gray scale image (unsigned char) to a vector
	bool serializeGrayImage(std::vector<int>& portData, const cv::Mat& img) {
		if (img.channels() != 1 || img.depth() != CV_8U) {
			std::cerr << mTag << " WARNING: image should be an gray scale (CV_8U) image" << std::endl;
			return false;
		}

		std::vector<int>::iterator itOut;
		if (!mSerializationHeader.serializeGrayImage(portData, img.rows, img.cols, itOut))
			return false;

		cv::Mat_<unsigned char>::const_iterator itImg = img.begin<unsigned char>(), itImgEnd = img.end<unsigned char>();
		for (; itImg != itImgEnd; ) {
			*itOut++ = *itImg++;
		}

		return true;
	}

private:
	std::string mTag;
	AimSerializationHeader mSerializationHeader;
};


} // namespace rur


#endif /* AIMSERIALIZATIONOPENCV_HPP_ */
