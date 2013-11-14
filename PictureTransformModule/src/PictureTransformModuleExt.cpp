/**
 * @file PictureTransformModuleExt.cpp
 * @brief PictureTransformModule extension
 *
 * This file is created at "Springfield Power Company". It is open-source software and part of "Project Bootstrap". 
 * This software is published under the State license (TR8OR).
 *
 * Copyright © 2013 Homer J. Simpson <chunkylover53@aol.com>
 *
 * @author                   Homer J. Simpson
 * @date                     nov  5, 2013
 * @organisation             Springfield Power Company
 * @project                  Project Bootstrap
 */

#include <PictureTransformModuleExt.h>
#include <iostream>

#define cimg_display 0
#include <CImg.h>

using namespace rur;
using namespace cimg_library;

//! Replace with your own code
PictureTransformModuleExt::PictureTransformModuleExt(): mStop(false) {

}

//! Replace with your own code
PictureTransformModuleExt::~PictureTransformModuleExt() {

}

//! Replace with your own code
void PictureTransformModuleExt::Tick() {
	long_seq* read;

	read = readInImage(false);
	if (read != NULL && !read->empty()) {
		// -- Read the image --
		long_seq::const_iterator it = read->begin();
		int nDims = *it++;
		if (nDims != 3) {
			std::cerr << "nDims=" << nDims << ", should be 3" << std::endl;
			read->clear();
			return;
		}
		int height = *it++;
		int width = *it++;
		int channels = *it++;
		if (channels != 3) {
			std::cerr << "channels=" << channels << ", should be 3" << std::endl;
			read->clear();
			return;
		}
		if (read->size() < 4+height*width*channels) {
			std::cerr << "read.size=" << read->size() << ", should be " << 4+height*width*channels << std::endl;
			read->clear();
			return;
		}

//		// Not an efficient loop (CImg prefers for(c) for(y) for(x))
//		CImg<unsigned char> img(width, height, 1, channels);
//		for (int y=0; y<height; ++y) {
//			for (int x=0; x<width; ++x) {
//				for (int c=0; c<channels; ++c) {
//					img(x,y,c) = *it++;
//				}
//			}
//		}

		// We just rotate the image 90 degrees, should loop a bit faster
		CImg<unsigned char> img(height, width, 1, channels); // CImg(width, height, depth, channels)
		for (int y=0; y<height; ++y) {
			for (int x=0; x<width; ++x) {
				for (int c=0; c<channels; ++c) {
					img(y,x,c) = *it++;
				}
			}
		}

		// The actual image transformation
		img.index(img.default_LUT256()).map(img.HSV_LUT256());

		// -- Write the result --
		long_seq out(4 + img.size());
		long_seq::iterator outIt = out.begin();
		*outIt++ = 3;
//		*outIt++ = img.height();
//		*outIt++ = img.width();
		*outIt++ = img.width(); // Because we rotated 90 degrees
		*outIt++ = img.height();
		*outIt++ = img.spectrum();

//		// Not an efficient loop (CImg prefers for(c) for(y) for(x))
//		for (int y=0; y<height; ++y) {
//			for (int x=0; x<width; ++x) {
//				for (int c=0; c<channels; ++c) {
//					*outIt++ = img(x,y,c);
//				}
//			}
//		}

		// And we rotate the image 90 degrees back
		for (int y=0; y<height; ++y) {
			for (int x=0; x<width; ++x) {
				for (int c=0; c<channels; ++c) {
					*outIt++ = img(y,x,c);
				}
			}
		}
		writeOutImage(out);

		read->clear();
	}

	usleep(10*1000);
//	mStop = true;
}

//! Replace with your own code
bool PictureTransformModuleExt::Stop() {
	return mStop;
}

