/**
 * @file JpgToBmpModuleExt.cpp
 * @brief JpgToBmpModule extension
 *
 * This file is created at "Springfield Power Company". It is open-source software and part of "Project Bootstrap". 
 * This software is published under the State license (TR8OR).
 *
 * Copyright © 2013 Homer J. Simpson <chunkylover53@aol.com>
 *
 * @author                   Homer J. Simpson
 * @date                     nov 12, 2013
 * @organisation             Springfield Power Company
 * @project                  Project Bootstrap
 */

#include <JpgToBmpModuleExt.h>
#include <unistd.h>
#include <jpeglib.h>
#include <iostream>
#include <base64.h>

#include <boost/property_tree/ptree.hpp>		// For command port
#include <boost/property_tree/json_parser.hpp>	// For command port

#include <CTime.h> // For profiling

// For test only
//#include <fstream>
//#include <CImg.h>
//#include <stdio.h>
//using namespace cimg_library;

using namespace rur;

JpgToBmpModuleExt::JpgToBmpModuleExt():
				mScaleNum(1),
				mScaleDenom(1) {

	mSerialization.setTag("[JpgToBmp]");
}

JpgToBmpModuleExt::~JpgToBmpModuleExt() {

}

void JpgToBmpModuleExt::Tick() {

	std::string* read = readJpg(false);
	if (read != NULL && !read->empty()) {
		std::cout << "[JpgToBmp] " << "read something" << std::endl;

		long startTime = get_cur_1ms();
		long endTime;

		unsigned char* bufIn;
		int bufInSize;
		bufIn = unbase64(read->c_str(), read->size(), &bufInSize);

		endTime = get_cur_1ms();
		std::cout << "[JpgToBmp] " << "Converted base64 string to binary in " << get_duration(startTime, endTime) << "ms" << std::endl;
		startTime = endTime;

		EXIFInfo exif;
		int code = exif.parseFrom(bufIn, bufInSize);
		if (code) {
			printf("[JpgToBmp] Error parsing EXIF: code %d\n", code);
		}

		jpeg_decompress_struct cinfo;
		jpeg_error_mgr jerr;
		cinfo.err = jpeg_std_error(&jerr);
		jpeg_create_decompress(&cinfo);
		jpeg_mem_src(&cinfo, bufIn, bufInSize); // buf = unsigned char* , size = unsigned long
		jpeg_read_header(&cinfo, TRUE);

		// Set parameters here (scale, quality, colormap)

		// scale by num/denom, 1/1, 1/2, 1/4, 1/8 are supported
		cinfo.scale_num = mScaleNum;
		cinfo.scale_denom = mScaleDenom;

		jpeg_calc_output_dimensions(&cinfo);

		jpeg_start_decompress(&cinfo);

		endTime = get_cur_1ms();
		std::cout << "[JpgToBmp] " << "Read jpg header in " << get_duration(startTime, endTime) << "ms" << std::endl;
		startTime = endTime;


		std::vector<int> writeVec;
		std::vector<int>::iterator itOut;
		mSerialization.serializeRgbImage(writeVec, cinfo.output_height, cinfo.output_width, itOut);

		// You will need output_width * output_components JSAMPLEs per scanline in your output buffer, and a total of output_height scanlines will be returned
		//	unsigned char** bufOut = new unsigned char*[cinfo.output_height];
		//	for (int i=0; i<cinfo.output_height; ++i) {
		//		bufOut[i] = new unsigned char[cinfo.output_width*cinfo.output_components];
		//	}
		unsigned char* bufOut[2];
		for (int i=0; i<2; ++i) {
			bufOut[i] = new unsigned char[cinfo.output_width*cinfo.output_components];
		}

		endTime = get_cur_1ms();
		std::cout << "[JpgToBmp] " << "Allocated bufOut in " << get_duration(startTime, endTime) << "ms" << std::endl;
		startTime = endTime;

		// Use bitmap data here
		std::cout << "[JpgToBmp] " << "cinfo.output_height=" << cinfo.output_height << " cinfo.output_width=" << cinfo.output_width << " cinfo.output_components=" << cinfo.output_components << std::endl;
		while (cinfo.output_scanline < cinfo.output_height) {
			// Try to read line per 2 lines (return value is number of lines actually read)
			int linesRead = jpeg_read_scanlines(&cinfo, bufOut, 2);

//			for (int y=lineStart; y<cinfo.output_scanline; ++y) {
//				for (int x=0; x<width; ++x) {
//					for (int c=0; c<channels; ++c) {
//						img(x,y,c) = bufOut[y-lineStart][x*channels+c];
//					}
//				}
//			}
			for (int i=0; i<linesRead; ++i) {
				for (int j=0; j<cinfo.output_width*cinfo.output_components; ++j)
					*itOut++ = bufOut[i][j];
			}
//			lineStart = cinfo.output_scanline;
		}

		endTime = get_cur_1ms();
		std::cout << "[JpgToBmp] Wrote result to int array in " << get_duration(startTime, endTime) << "ms" << std::endl;
		startTime = endTime;

		rotate(writeVec, cinfo.output_width, cinfo.output_height, exif, &writeVec);

		endTime = get_cur_1ms();
		std::cout << "[JpgToBmp] image rotated in " << get_duration(startTime, endTime) << "ms" << std::endl;
		startTime = endTime;

		jpeg_finish_decompress(&cinfo);
		jpeg_destroy_decompress(&cinfo);

		delete [] bufIn;

		// Write output
		writeBmp(writeVec);

		endTime = get_cur_1ms();
		std::cout << "[JpgToBmp] " << "Wrote result to port in " << get_duration(startTime, endTime) << "ms" << std::endl;
		startTime = endTime;

//		CImgDisplay disp(img);
//		while (!disp.is_closed())
//			disp.wait();

		for (int i=0; i<2; ++i) {
			delete [] bufOut[i];
		}
		//delete [] bufOut;

		endTime = get_cur_1ms();
		std::cout << "[JpgToBmp] " << "Deallocated bufOut in " << get_duration(startTime, endTime) << "ms" << std::endl;
		startTime = endTime;

		read->clear();
	}

	read = readCommand(false);
	if (read != NULL && !read->empty()) {

		int nom, denom;
		boost::property_tree::ptree pt;
		std::stringstream ss;
		ss << *read;
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
			nom = pt.get("scale_numerator",1);
			denom = pt.get("scale_denominator",1);
		}
		catch (std::exception &e) {
			std::cerr << "[JpgToBmp] " << "Error: Unable to parse json command: " << e.what() << std::endl;
		}

		// Numerator must be 1, so ignore it
		// Denominator can be 1, 2, 4 or 8
		if (denom == 1 || denom == 2 || denom == 4 || denom == 8) {
			mScaleDenom = denom;
			std::cout << "[JpgToBmp] " << "Changed scale denominator to " << denom << std::endl;
		}

		read->clear();
	}

	usleep(10*1000);

}

int JpgToBmpModuleExt::getRotatedIdx(int rotation, int row, int col, int width, int height) {
	rotation %= 360;

	switch(rotation) {
	case 90:
	case -270:
		return ((width - col - 1) * height + row) * 3;
	case 180:
	case -180:
		return ((height - row - 1) * height + (height - col - 1)) * 3;
	case 270:
	case -90:
		return (col * height + height - row - 1) * 3;
	}
}

void JpgToBmpModuleExt::adjustRotatedSize(int rotation, int width, int height, int* newWidth, int* newHeight) {
	rotation %= 360;

	switch(rotation) {
	case 90:
	case -270:
	case 270:
	case -90:
		*newWidth = height;
		*newHeight = width;
		break;
	case 180:
	case -180:
		*newWidth = width;
		*newHeight = height;
	}

	printf("new size: w=%d, h=%d\n", *newWidth, *newHeight);
}

void JpgToBmpModuleExt::rotate(long_seq img, int width, int height, EXIFInfo exif, long_seq* newImg) {

	int rotation;
	switch(exif.Orientation) {
	case 6:
		rotation = -90;
		break;
	case 3:
		rotation = -180;
		break;
	case 8:
		rotation = -270;
		break;
	default:
		rotation = 0;
		break;
	}


	if (rotation != 0) {
		printf("rotating image by %d °\n", rotation);

		int size = width * height * 3;
//		long_seq* newImg = new long_seq(size);
//		newImg->reserve(size);

		long_seq::iterator itOut = newImg->begin();

		int newWidth, newHeight;
		int oldIndex = 0;

		adjustRotatedSize(rotation, width, height, &newWidth, &newHeight);

		*itOut++ = (img)[oldIndex++];
		*itOut++ = (img)[oldIndex++];
		*itOut++ = (img)[oldIndex++];
		*itOut++ = newHeight; oldIndex++;
		*itOut++ = newWidth; oldIndex++;
		*itOut++ = (img)[oldIndex++];

		int offset = oldIndex;

		int row, col;
		for (row = 0; row < newHeight; row++) {
			for (col = 0; col < newWidth; col++) {
				oldIndex = getRotatedIdx(-rotation, row, col, newWidth, newHeight);
				*itOut++ = (img)[offset + oldIndex];
				*itOut++ = (img)[offset + oldIndex+1];
				*itOut++ = (img)[offset + oldIndex+2];
			}
		}
		return;
	} else {
		printf("no rotation needed\n");
		*newImg = img;
		return;
	}
}

bool JpgToBmpModuleExt::Stop() {
	return false;
}

