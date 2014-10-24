/*
 * AimSerializationHeader.hpp
 *
 *  Created on: Apr 22, 2014
 *      Author: vliedel
 */

#ifndef AIMSERIALIZATIONHEADER_HPP_
#define AIMSERIALIZATIONHEADER_HPP_

#include <AimSerialization.hpp>

namespace rur {

class AimSerializationHeader : public AimSerializationBase {
public:
	AimSerializationHeader() :
		mDataType(0),
		mNumTensors(0) {
		mHeaderSize = 2;
		mDataSize = 0;
	}
	~AimSerializationHeader() {}

	template <typename OutputIterator>
	OutputIterator serializeHeader(const OutputIterator first, const OutputIterator last) {
		__glibcxx_function_requires(_OutputIteratorConcept<OutputIterator>);
		__glibcxx_requires_valid_range(first, last);
		typename std::iterator_traits<OutputIterator>::difference_type size = std::distance(first, last);

		if (size < mHeaderSize) {
			std::cerr << mTag << "WARNING: container size too small to serialize header" << std::endl;
			throw AimException();
		}

		OutputIterator it = first;
		*it++ = mDataType;
		*it++ = mNumTensors;
		return it;
	}

	template <typename InputIterator>
	InputIterator deserializeHeader(const InputIterator first, const InputIterator last) {
		__glibcxx_function_requires(_InputIteratorConcept<InputIterator>);
		__glibcxx_requires_valid_range(first, last);
//		typedef typename std::iterator_traits<InputIterator>::value_type ValueType;
//		typedef typename std::iterator_traits<InputIterator>::difference_type DistanceType;
		typename std::iterator_traits<InputIterator>::difference_type size = std::distance(first, last);
		if (size < mHeaderSize) {
			std::cerr << mTag << "WARNING: container size too small to deserialize header" << std::endl;
			throw AimException();
		}

		InputIterator it = first;
		mDataType = *it++;
		if (mDataType != 0) {
			std::cerr << mTag << " WARNING: data type should be 0" << std::endl;
			throw AimException();
		}
		int mNumTensors = *it++;
		if (mNumTensors < 1) {
			std::cerr << mTag << " WARNING: number of tensors should be at least 1" << std::endl;
			throw AimException();
		}
		return it;
	}

	// Don't use this function, the header has no data
	template <typename OutputIterator>
	OutputIterator serializeData(const OutputIterator first, const OutputIterator last) {
		throw AimException();
//		serializeDataCheck(first, last);
		OutputIterator it = first;
		return it;
	}

	// Don't use this function, the header has no data
	template <typename InputIterator>
	InputIterator deserializeData(const InputIterator first, const InputIterator last) {
		throw AimException();
//		deserializeDataCheck(first, last);
		InputIterator it = first;
		return it;
	}


private:
	int mDataType;
	int mNumTensors;

	void calcHeaderSize() {
	}

	void calcDataSize() {
	}

};


} // namespace rur


#endif /* AIMSERIALIZATIONHEADER_HPP_ */
