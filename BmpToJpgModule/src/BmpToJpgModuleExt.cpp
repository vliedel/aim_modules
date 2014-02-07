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

#include <boost/property_tree/ptree.hpp>		// For command port
#include <boost/property_tree/json_parser.hpp>	// For command port

using namespace rur;

//class JpgDataDest {
//public:
//	unsigned char* mBuf;
//	unsigned char* mNextOutputByte;
//	size_t mBytesLeft;
//
//	void init_destination(j_compress_ptr cinfo) {
//
//	}
//
//	// This is called whenever the buffer has filled (mBytesLeft reaches zero).
//	bool empty_output_buffer (j_compress_ptr cinfo) {
//		// In typical applications, it should write out the *entire* buffer
//		// (use the saved start address and buffer length; ignore the current state of mNextOutputByte and mBytesLeft).
//		// Then reset the pointer & count to the start of the buffer, and return TRUE indicating that the buffer has been dumped.
//		// mBytesLeft must be set to a positive value when TRUE is returned.
//		// A FALSE return should only be used when I/O suspension is desired.
//
//		return true;
//	}
//
//	// Terminate destination --- called by jpeg_finish_compress() after all data has been written.
//	void term_destination (j_compress_ptr cinfo) {
//		// In most applications, this must flush any data remaining in the buffer.
//		// Use either mNextOutputByte or mBytesLeft to determine how much data is in the buffer.
//	}
//
//
//
//};

//! Replace with your own code
BmpToJpgModuleExt::BmpToJpgModuleExt(): mQuality(75) {

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
			std::cerr << "[BmpToJpg] nDims=" << nDims << ", should be 3" << std::endl;
			read->clear();
			return;
		}
		int height = *it++;
		int width = *it++;
		int channels = *it++;
		if (channels != 3) {
			std::cerr << "[BmpToJpg] channels=" << channels << ", should be 3" << std::endl;
			read->clear();
			return;
		}
		if (read->size() < 6+height*width*channels) {
			std::cerr << "[BmpToJpg] read.size=" << read->size() << ", should be " << 6+height*width*channels << std::endl;
			read->clear();
			return;
		}

		unsigned char* bufIn = new unsigned char[width*channels];


		struct jpeg_compress_struct cinfo;
		struct jpeg_error_mgr jerr;
		cinfo.err = jpeg_std_error(&jerr);
		jpeg_create_compress(&cinfo);

//		FILE * outfile;
//		if ((outfile = fopen(filename, "wb")) == NULL) {
//			fprintf(stderr, "can't open %s\n", filename);
//			exit(1);
//		}

//		jpeg_stdio_dest(&cinfo, );

//		unsigned long bufOutSize = read->size();
//		unsigned char* bufOut = new unsigned char[bufOutSize];
		unsigned long bufOutSize = 0;
		unsigned char* bufOut = NULL;

		jpeg_mem_dest(&cinfo, &bufOut, &bufOutSize);

		cinfo.image_height = height;
		cinfo.image_width = width;
		cinfo.input_components = channels;
		cinfo.in_color_space = JCS_RGB;
		jpeg_set_defaults(&cinfo);
		// Make optional parameter settings here
		jpeg_set_quality(&cinfo, mQuality, TRUE);

		jpeg_start_compress(&cinfo, TRUE);

		unsigned char* pRow[1];        /* pointer to a single row */
		int row_stride;                 /* physical row width in buffer */
		row_stride = cinfo.image_width * cinfo.input_components;   /* JSAMPLEs per row in image_buffer */
		while (cinfo.next_scanline < cinfo.image_height) {
			for (int i=0; i<row_stride; ++i) {
				bufIn[i] = *it++;
			}

			//row_pointer[0] = &buffer[cinfo.next_scanline * row_stride];
			pRow[0] = bufIn;
			jpeg_write_scanlines(&cinfo, pRow, 1);
		}
		jpeg_finish_compress(&cinfo);

		// Write the result
		//..
		//..
		// Converts binary data of length=len to base64 characters.
		// Length of the resultant string is stored in flen
		// (you must pass pointer flen).
		int base64Len;
		char* base64String = base64(bufOut, bufOutSize, &base64Len);

		writeJpg(std::string(base64String));
		//std::cout << std::string(base64String) << std::endl;

		jpeg_destroy_compress(&cinfo);


		// http://apodeline.free.fr/DOC/libjpeg/libjpeg-3.html
		delete [] bufIn;
//		delete [] bufOut;
		read->clear();


	}

	std::string* readStr = readCommand(false);
	if (readStr != NULL && !readStr->empty()) {

		int quality;
		boost::property_tree::ptree pt;
		std::stringstream ss;
		ss << *readStr;
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
			quality = pt.get("quality", 75);
		}
		catch (std::exception &e) {
			std::cerr << "[BmpToJpg] Error: Unable to parse json command: " << e.what() << std::endl;
		}

		// Numerator must be 1, so ignore it
		// Denominator can be 1, 2, 4 or 8
		if (0 <= quality && quality <= 100) {
			mQuality = quality;
			std::cout << "[BmpToJpg] Changed quality to " << quality << std::endl;
		}

		readStr->clear();
	}
}

//! Replace with your own code
bool BmpToJpgModuleExt::Stop() {
	return false;
}

