/*
 * AimSerializationHeader.hpp
 *
 *  Created on: Feb 12, 2014
 *      Author: vliedel
 */

#ifndef AIMSERIALIZATIONHEADER_HPP_
#define AIMSERIALIZATIONHEADER_HPP_

#include <string>
#include <iostream>

namespace rur {

class AimSerializationHeader {
public:
	AimSerializationHeader() {}
	~AimSerializationHeader() {}

	void setTag(std::string tag) { mTag = tag; };

	// Read the header of an RGB image from a vector
	// Sets height and width and sets iterator at start of data
	// Clears the vector on error
	bool deserializeRgbImage(std::vector<int>* portData, int& height, int& width, std::vector<int>::const_iterator& it) {
		if (portData == NULL || portData->empty())
			return false;

		it = portData->begin();
		int dataType = *it++;
		if (dataType != 0) {
			std::cerr << mTag << " WARNING: data type should be 0" << std::endl;
			portData->clear();
			return false;
		}
		int nTensors = *it++;
		if (nTensors != 1) {
			std::cerr << mTag << " WARNING: number of tensors should be 1" << std::endl;
			portData->clear();
			return false;
		}
		int nDims = *it++;
		if (nDims != 3) {
			std::cerr << mTag << " WARNING: number of dimensions should be 3" << std::endl;
			portData->clear();
			return false;
		}
		height = *it++;
		if (height < 1) {
			std::cerr << mTag << " WARNING: height must be larger than 1" << std::endl;
			portData->clear();
			return false;
		}
		width = *it++;
		if (width < 1) {
			std::cerr << mTag << " WARNING: width must be larger than 1" << std::endl;
			portData->clear();
			return false;
		}
		int channels = *it++;
		if (channels != 3) {
			std::cerr << mTag << " WARNING: number of channels should be 3" << std::endl;
			portData->clear();
			return false;
		}
		if (portData->size() < 6+height*width*channels) {
			std::cerr << mTag << " WARNING: data size should be " << 6+height*width*channels << std::endl;
			portData->clear();
			return false;
		}

		return true;
	}

	// Read the header of a gray scale image from a vector - Untested!
	// Sets height and width and sets iterator at start of data
	// Clears the vector on error
	bool deserializeGrayImage(std::vector<int>* portData, int& height, int& width, std::vector<int>::const_iterator& it) {
		if (portData == NULL || portData->empty())
			return false;

		it = portData->begin();
		int dataType = *it++;
		if (dataType != 0) {
			std::cerr << mTag << " WARNING: data type should be 0" << std::endl;
			portData->clear();
			return false;
		}
		int nTensors = *it++;
		if (nTensors != 1) {
			std::cerr << mTag << " WARNING: number of tensors should be 1" << std::endl;
			portData->clear();
			return false;
		}
		int nDims = *it++;
		if (nDims != 2) {
			std::cerr << mTag << " WARNING: number of dimensions should be 2" << std::endl;
			portData->clear();
			return false;
		}
		height = *it++;
		if (height < 1) {
			std::cerr << mTag << " WARNING: height must be larger than 1" << std::endl;
			portData->clear();
			return false;
		}
		width = *it++;
		if (width < 1) {
			std::cerr << mTag << " WARNING: width must be larger than 1" << std::endl;
			portData->clear();
			return false;
		}
		if (portData->size() < 5+height*width) {
			std::cerr << mTag << " WARNING: data size should be " << 5+height*width << std::endl;
			portData->clear();
			return false;
		}

		return true;
	}

	// Write the header of an RGB image to a vector
	// Sets iterator at start of data
	bool serializeRgbImage(std::vector<int>& portData, const int height, const int width, std::vector<int>::iterator& it) {
		portData.resize(6+height*width*3);
		it = portData.begin();
		*it++ = 0; // type
		*it++ = 1; // num tensors
		*it++ = 3; // num dims
		*it++ = height; // size dim 1 (height)
		*it++ = width; // size dim 2 (width)
		*it++ = 3; // size dim 3 (channels)
		return true;
	}

	// Write the header of a gray scale image to a vector
	// Sets iterator at start of data
	bool serializeGrayImage(std::vector<int>& portData, const int height, const int width, std::vector<int>::iterator& it) {
		portData.resize(5+height*width);
		it = portData.begin();
		*it++ = 0; // type
		*it++ = 1; // num tensors
		*it++ = 2; // num dims
		*it++ = height; // size dim 1 (height)
		*it++ = width; // size dim 2 (width)
		return true;
	}

private:
	std::string mTag;
};


} // namespace rur


#endif /* AIMSERIALIZATIONHEADER_HPP_ */
