/**
 * @file BmpToJpgModuleExt.cpp
 * @brief BmpToJpgModule extension
 *
 * This file is created at "DoBots". It is open-source software and part of "AI". 
 * This software is published under the GNU General Lesser Public Licence license (GPLv3).
 *
 * Copyright © 2013 Bart van Vliet <bart@almende.org>
 *
 * @author                   Bart van Vliet
 * @date                     dec 19, 2013
 * @organisation             DoBots
 * @project                  AI
 */

#include <BmpToJpgModuleExt.h>
#include <unistd.h>
#include <jpeglib.h>
#include <iostream>
#include <base64.h>

using namespace rur;

//! Replace with your own code
BmpToJpgModuleExt::BmpToJpgModuleExt() {

}

//! Replace with your own code
BmpToJpgModuleExt::~BmpToJpgModuleExt() {

}

//! Replace with your own code
void BmpToJpgModuleExt::Tick() {


	long_seq* read;

	read = readBmp(false);
	if (read != NULL && !read->empty()) {
		// -- Read the image --
		long_seq::const_iterator it = read->begin();
		int dataType = *it++;
		if (dataType != 0)
			return;
		int nArrays = *it++;
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

		char* buffer = new char[height*width*channels];
		size_t bufSize = height*width*channels;


		struct jpeg_compress_struct cinfo;
		struct jpeg_error_mgr jerr;
		cinfo.err = jpeg_std_error(&jerr);
		jpeg_create_compress(&cinfo);





		// http://apodeline.free.fr/DOC/libjpeg/libjpeg-3.html
		delete buffer;
		read->clear();
	}
}

//! Replace with your own code
bool BmpToJpgModuleExt::Stop() {
	return false;
}

