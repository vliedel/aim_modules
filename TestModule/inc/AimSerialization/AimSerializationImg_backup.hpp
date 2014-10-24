/*
 * AimSerializationImg.hpp
 *
 * This file is created at "DoBots".
 * This software is published under the GNU General Lesser Public Licence license (GPLv3).
 *
 * Copyright © 2014 Bart van Vliet <bart@almende.org>
 *
 * @author                   Bart van Vliet
 * @date                     Feb 14, 2014
 * @organisation             DoBots
 * @project                  AIM
 */

#ifndef AIMSERIALIZATIONIMG_HPP_
#define AIMSERIALIZATIONIMG_HPP_

#include <AimSerialization.hpp>
#include <string>
#include <iterator>

namespace rur {

class AimSerializationImg : public AimSerialization {
public:
	AimSerializationImg() : mChannels(1) {}
	~AimSerializationImg() {}

	// Set expected number of color channels
	void setChannels(int c) { if (c > 0) mChannels = c; }

	template <typename T, typename InputIterator>
	bool serialize(InputIterator& it, const InputIterator end) {
		size_t size = end-it;
		int dataType = *it++;
		if (dataType != 0) {
			std::cerr << mTag << " WARNING: data type should be 0" << std::endl;
			return false;
		}
		int nTensors = *it++;
		if (nTensors != 1) {
			std::cerr << mTag << " WARNING: number of tensors should be 1" << std::endl;
			return false;
		}

		int headSize;
		int dims;
		if (mChannels < 1) {

		}
		else if (mChannels == 1) {
			headSize = 5;
			dims = 2;
		}
		else {
			headSize = 6;
			dims = 3;
		}

		int nDims = *it++;
		if (nDims != dims) {
			std::cerr << mTag << " WARNING: number of dimensions should be " << dims << std::endl;
			return false;
		}

		mHeight = *it++;
		if (mHeight < 1) {
			std::cerr << mTag << " WARNING: height must be larger than 1" << std::endl;
			return false;
		}
		mWidth = *it++;
		if (mWidth < 1) {
			std::cerr << mTag << " WARNING: width must be larger than 1" << std::endl;
			return false;
		}
		int channels = *it++;
		if (channels != mChannels) {
			std::cerr << mTag << " WARNING: number of channels should be " << mChannels << std::endl;
			return false;
		}
		std::iterator_traits<InputIterator>::difference_type dist = std::distance(it, end);
		if (dist < headSize+mHeight*mWidth*channels) {
			std::cerr << mTag << " WARNING: data size should be " << headSize+mHeight*mWidth*channels << std::endl;
			return false;
		}

		return true;
	}

	template <typename T, typename OutputIterator>
	bool deserialize(OutputIterator& it) {
		return false;
	}



protected:
	int mHeight;
	int mWidth;
	int mChannels;
};


} // namespace rur

#endif /* AIMSERIALIZATIONIMG_HPP_ */
