/**
 * @file ImageDisplayModuleExt.cpp
 * @brief ImageDisplayModule extension
 *
 * This file is created at "Springfield Power Company". It is open-source software and part of "Project Bootstrap". 
 * This software is published under the State license (TR8OR).
 *
 * Copyright © 2013 Homer J. Simpson <chunkylover53@aol.com>
 *
 * @author                   Homer J. Simpson
 * @date                     nov 14, 2013
 * @organisation             Springfield Power Company
 * @project                  Project Bootstrap
 */

#include <ImageDisplayModuleExt.h>
#include <iostream>
#include <unistd.h>

using namespace cimg_library;
using namespace rur;

//! Replace with your own code
ImageDisplayModuleExt::ImageDisplayModuleExt() {
	mDisp = new CImgDisplay();
}

//! Replace with your own code
ImageDisplayModuleExt::~ImageDisplayModuleExt() {
	delete mDisp;
}

//! Replace with your own code
void ImageDisplayModuleExt::Tick() {
	long_seq* readVec = readImage(false);
	if (readVec != NULL && !readVec->empty()) {
		std::cout << "[ImageDisplay] Read something" << std::endl;
		long_seq::const_iterator it = readVec->begin();
		int dataType = *it++;
		if (dataType != 0) {
			std::cerr << "[ImageDisplay] WARNING: type is not 0" << std::endl;
			return;
		}
		int nTensors = *it++;
		if (nTensors != 1) {
			std::cerr << "[ImageDisplay] WARNING: nTensors is not 1" << std::endl;
			return;
		}
		int nDims = *it++;
		if (nDims != 3) {
			readVec->clear();
			std::cerr << "[ImageDisplay] WARNING: number of dimensions should be 3" << std::endl;
			return;
		}
		int height = *it++;
		int width = *it++;
		int channels = *it++;
		if (channels != 3) {
			std::cerr << "[ImageDisplay] WARNING: channels should be 3" << std::endl;
			readVec->clear();
			return;
		}
		CImg<unsigned char> img(width, height, 1, channels);
		for (int y=0; y<height; ++y) {
			for (int x=0; x<width; ++x) {
				for (int c=0; c<channels; ++c) {
					img(x,y,c) = *it++;
				}
			}
		}
		std::cout << "[ImageDisplay] height=" << height << " width=" << width << std::endl;


//		CImg<unsigned char> img(height, width, 1, channels);
//		for (int y=0; y<height; ++y) {
//			for (int x=0; x<width; ++x) {
//				for (int c=0; c<channels; ++c) {
//					img(y,x,c) = *it++;
//				}
//			}
//		}

		//CImgDisplay disp(img);
		img.resize_tripleXY();
		if (mDisp->is_closed())
			mDisp->assign();

		mDisp->display(img);


		readVec->clear();
	}

	usleep(100*1000);
}

//! Replace with your own code
bool ImageDisplayModuleExt::Stop() {
	return false;
}

