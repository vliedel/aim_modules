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

// For test only
//#include <fstream>
//#include <CImg.h>
//#include <stdio.h>
//using namespace cimg_library;

using namespace rur;

//! Replace with your own code
JpgToBmpModuleExt::JpgToBmpModuleExt() {

//#define TEST_JPG
#ifdef TEST_JPG
//	std::ifstream mFs("/home/vliedel/android/test.txt");
//	int dims;
//	mFs >> dims;
//	int size;
//	mFs >> size;
//	unsigned char* bufIn = new unsigned char[size];
//	int read;
//	for (int i=0; i<size; ++i) {
//		mFs >> read;
//		bufIn[i] = read;
//	}


//	FILE* inputFile;
//	inputFile = fopen("/home/vliedel/android/base64_0.txt", "r");
//	fclose(inputFile);
	std::ifstream mFs("/home/vliedel/android/base64_1.txt");
	std::string str;
	mFs >> str;


	unsigned char* bufIn;
	int bufInSize;
	bufIn = unbase64(str.c_str(), str.size(), &bufInSize);


	jpeg_decompress_struct cinfo;
	jpeg_error_mgr jerr;
	cinfo.err = jpeg_std_error(&jerr);
	jpeg_create_decompress(&cinfo);
	jpeg_mem_src(&cinfo, bufIn, bufInSize); // buf = unsigned char* , size = unsigned long
	jpeg_read_header(&cinfo, true);
	// Set parameters here (scale, quality, colormap)

	jpeg_calc_output_dimensions(&cinfo);
	// You will need output_width * output_components JSAMPLEs per scanline in your output buffer, and a total of output_height scanlines will be returned
//	unsigned char** bufOut = new unsigned char*[cinfo.output_height];
//	for (int i=0; i<cinfo.output_height; ++i) {
//		bufOut[i] = new unsigned char[cinfo.output_width*cinfo.output_components];
//	}
	unsigned char* bufOut[2];
	for (int i=0; i<2; ++i) {
		bufOut[i] = new unsigned char[cinfo.output_width*cinfo.output_components];
	}

	jpeg_start_decompress(&cinfo);

	int width = cinfo.output_width;
	int height = cinfo.output_height;
	int channels = cinfo.output_components;
	CImg<unsigned char> img(height, width, 1, channels);
	int lineStart=0;

	// Use bitmap data here
	std::cout << "cinfo.output_height=" << cinfo.output_height << " cinfo.output_width=" << cinfo.output_width << " cinfo.output_components=" << cinfo.output_components << std::endl;
	while (cinfo.output_scanline < cinfo.output_height) {
		// Try to read line per 2 lines (return value is number of lines actually read)
		jpeg_read_scanlines(&cinfo, bufOut, 2);

		for (int y=lineStart; y<cinfo.output_scanline; ++y) {
			for (int x=0; x<width; ++x) {
				for (int c=0; c<channels; ++c) {
					img(y,x,c) = bufOut[y-lineStart][x*channels+c];
				}
			}
		}
		lineStart = cinfo.output_scanline;
	}

	jpeg_finish_decompress(&cinfo);
	jpeg_destroy_decompress(&cinfo);

	delete [] bufIn;

	// Write output
	std::cout << "Decompressed jpeg" << std::endl;

	CImgDisplay disp(img);
	while (!disp.is_closed())
		disp.wait();

	for (int i=0; i<2; ++i) {
		delete [] bufOut[i];
	}
	//delete [] bufOut;
#endif
}

//! Replace with your own code
JpgToBmpModuleExt::~JpgToBmpModuleExt() {

}

//! Replace with your own code
void JpgToBmpModuleExt::Tick() {
//	long_seq* read = readJpg(false);
//	if (read != NULL && !read->empty()) {
//		std::cout << "Read: dims=" << read->at(0) << " ";
//		for (int i=0; i<read->at(0); ++i)
//			std::cout << "size" << i << "=" << read->at(i+1);
//		std::cout << std::endl;
//
//
//		long_seq::const_iterator itRead = read->begin();
//		int dims = *itRead++;
//		if (dims != 1) {
//			read->clear();
//			return;
//		}
//
//		int size = *itRead++;
//		if (size != read->size()-2) {
//			read->clear();
//			return;
//		}
//
//		unsigned char* bufIn = new unsigned char[size];
//		for (int i=0; i<size; ++i)
//			bufIn[i] = *itRead++;

	// Only one input port, so we can block.
	// To be sure, we assume it's non-blocking in case the middleware did not implement blocking reads.
	std::string* read = readJpg(false);
	if (read != NULL && !read->empty()) {
		std::cout << "read something" << std::endl;

		unsigned char* bufIn;
		int bufInSize;
		bufIn = unbase64(read->c_str(), read->size(), &bufInSize);

		jpeg_decompress_struct cinfo;
		jpeg_error_mgr jerr;
		cinfo.err = jpeg_std_error(&jerr);
		jpeg_create_decompress(&cinfo);
		jpeg_mem_src(&cinfo, bufIn, bufInSize); // buf = unsigned char* , size = unsigned long
		jpeg_read_header(&cinfo, true);
		// Set parameters here (scale, quality, colormap)

		jpeg_calc_output_dimensions(&cinfo);
		// You will need output_width * output_components JSAMPLEs per scanline in your output buffer, and a total of output_height scanlines will be returned
	//	unsigned char** bufOut = new unsigned char*[cinfo.output_height];
	//	for (int i=0; i<cinfo.output_height; ++i) {
	//		bufOut[i] = new unsigned char[cinfo.output_width*cinfo.output_components];
	//	}
		unsigned char* bufOut[2];
		for (int i=0; i<2; ++i) {
			bufOut[i] = new unsigned char[cinfo.output_width*cinfo.output_components];
		}

		jpeg_start_decompress(&cinfo);

//		int width = cinfo.output_width;
//		int height = cinfo.output_height;
//		int channels = cinfo.output_components;
//		CImg<unsigned char> img(width, height, 1, channels);
		long_seq writeVec(4+cinfo.output_height*cinfo.output_width*cinfo.output_components);
		long_seq::iterator itOut = writeVec.begin();
		*itOut++ = 3; // nDims
		*itOut++ = cinfo.output_height;
		*itOut++ = cinfo.output_width;
		*itOut++ = cinfo.output_components;
//		int lineStart=0;

		// Use bitmap data here
		std::cout << "cinfo.output_height=" << cinfo.output_height << " cinfo.output_width=" << cinfo.output_width << " cinfo.output_components=" << cinfo.output_components << std::endl;
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

		jpeg_finish_decompress(&cinfo);
		jpeg_destroy_decompress(&cinfo);

		delete [] bufIn;

		// Write output
		std::cout << "Decompressed jpeg" << std::endl;
		writeBmp(writeVec);

//		CImgDisplay disp(img);
//		while (!disp.is_closed())
//			disp.wait();

		for (int i=0; i<2; ++i) {
			delete [] bufOut[i];
		}
		//delete [] bufOut;

		read->clear();
	}
	usleep(10*1000);

}

//! Replace with your own code
bool JpgToBmpModuleExt::Stop() {
	return false;
}

