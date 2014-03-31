/*
 * AimSerialization.hpp
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

#ifndef AIMSERIALIZATION_HPP_
#define AIMSERIALIZATION_HPP_


#include <string>
#include <iterator>
#include <vector>
//#include <assert.h>
#include <numeric>
#include <stdexcept>

namespace rur {

// Idea:
// Have 1 class that can load several serialization classes, one for each tensor
// For each tensor, work like this:
// If nDims is set, check if data has that amount of dims. Else, set nDims and work with that
// Same for height, width, channels, etc.
// Check size with: http://en.cppreference.com/w/cpp/iterator/distance
// Also see: https://github.com/mrquincle/equids/blob/master/bridles/common/dim1algebra.hpp


class AimException : public std::runtime_error {
public:
	AimException() : std::runtime_error("AimException") {}
};


class AimSerializationBase {
public:
	AimSerializationBase(): mNumDims(0), mDataSize(0), mDimSizes(0), mHeaderSize(0) {}
	~AimSerializationBase() {}

	int getHeaderSize() {
		if (mHeaderSize == 0) {
			calcHeaderSize();
		}
		return mHeaderSize;
	}

	int getDataSize() {
		if (mDataSize == 0) {
			calcDataSize();
		}
		return mDataSize;
	}

	void setTag(std::string tag) { mTag = tag; };

	void setNumDims(size_t numDims) {
		mNumDims = numDims;
		mDimSizes.resize(numDims);
		mHeaderSize = 0;
		mDataSize = 0;
	}

	void setDimSize(size_t dim, size_t dimSize) {
		if (dim >= mNumDims) {
			throw AimException();
		}
		mDimSizes[dim] = dimSize;
		mDataSize = 0;
	}

	template <typename OutputIterator>
	OutputIterator serializeHeader(const OutputIterator first, const OutputIterator last) {
		__glibcxx_function_requires(_OutputIteratorConcept<OutputIterator>);
		__glibcxx_requires_valid_range(first, last);
		//typedef typename std::iterator_traits<OutputIterator>::difference_type DistanceType;
		typename std::iterator_traits<OutputIterator>::difference_type size = std::distance(first, last);

		getHeaderSize();
		if (size < mHeaderSize) {
			std::cerr << mTag << "WARNING: container size too small to serialize header" << std::endl;
			throw AimException();
		}

		OutputIterator it = first;
		*it++ = mNumDims;
		for (int i=0; i<mNumDims; ++i) {
			*it++ = mDimSizes[i];
		}
		return it;
	}

	template <typename InputIterator>
	InputIterator deserializeHeader(const InputIterator first, const InputIterator last) {
		__glibcxx_function_requires(_InputIteratorConcept<InputIterator>);
		__glibcxx_requires_valid_range(first, last);
//		typedef typename std::iterator_traits<InputIterator>::value_type ValueType;
//		typedef typename std::iterator_traits<InputIterator>::difference_type DistanceType;
		typename std::iterator_traits<InputIterator>::difference_type size = std::distance(first, last);
		if (size < 1)
			throw AimException();
		InputIterator it = first;
		mNumDims = *it++;
		if (mNumDims < 0) {
			std::cerr << mTag << " WARNING: number of dimensions should be >= 0" << std::endl;
			throw AimException();
		}
		mHeaderSize = 1+mNumDims;
		if (size < mHeaderSize) {
			std::cerr << mTag << " WARNING: container size too small to deserialize header" << std::endl;
			throw AimException();
		}
		mDataSize = 1;
		for (int i=0; i<mNumDims; ++i) {
			int dimSize = *it++;
			if (dimSize < 1) {
				std::cerr << mTag << " WARNING: dim size should be >= 1" << std::endl;
				throw AimException();
			}
			mDimSizes.push_back(dimSize);
			mDataSize *= dimSize;
		}
		return it;
	}

	template <typename OutputIterator>
	OutputIterator serializeData(const OutputIterator first, const OutputIterator last) {
		serializeDataCheck(first, last);

		// Increase the iterator ?
		OutputIterator it = first + mDataSize;
		return it;
	}

	template <typename InputIterator>
	InputIterator deserializeData(const InputIterator first, const InputIterator last) {
		deserializeDataCheck(first, last);

		// Increase the iterator ?
		InputIterator it = first + mDataSize;
		return it;
	}

protected:

	std::string mTag;
	int mNumDims;
	std::vector<size_t> mDimSizes;
	int mHeaderSize;
	int mDataSize;

	template <typename OutputIterator>
	void serializeDataCheck(const OutputIterator first, const OutputIterator last) {
		__glibcxx_function_requires(_OutputIteratorConcept<OutputIterator>);
		__glibcxx_requires_valid_range(first, last);
//		typedef typename std::iterator_traits<OutputIterator>::difference_type DistanceType;
		typename std::iterator_traits<OutputIterator>::difference_type size = std::distance(first, last);

		getDataSize();
		if (size < mDataSize) {
			std::cerr << mTag << " WARNING: container size too small to serialize data" << std::endl;
			throw AimException();
		}
	}

	template <typename InputIterator>
	void deserializeDataCheck(const InputIterator first, const InputIterator last) {
		__glibcxx_function_requires(_InputIteratorConcept<InputIterator>);
		__glibcxx_requires_valid_range(first, last);
//		typedef typename std::iterator_traits<InputIterator>::difference_type DistanceType;
		typename std::iterator_traits<InputIterator>::difference_type size = std::distance(first, last);
		if (size < mDataSize) {
			std::cerr << mTag << " WARNING: container size too small to deserialize data" << std::endl;
			throw AimException();
		}
	}

private:

	void calcHeaderSize() {
		mHeaderSize = 1+mNumDims;
		if (mNumDims != mDimSizes.size()) {
			std::cerr << mTag << " WARNING: number of dimensions does not match" << std::endl;
			throw AimException();
		}
	}

	void calcDataSize() {
		getHeaderSize();
		mDataSize = 1;
		for (int i=0; i<mNumDims; ++i) {
			if (mDimSizes[i] < 1) {
				std::cerr << mTag << " WARNING: dim size should be >= 1" << std::endl;
				throw AimException();
			}
			mDataSize *= mDimSizes[i];
		}
	}
};


class AimSerialization {
public:
	AimSerialization(): mDataType(0), mNumTensors(0), mSerializedSize(0), mSerializations(0), mBaseSerializations(0) {}
	~AimSerialization() {}

	void setTag(std::string tag) {
		mTag = tag;
		for (size_t i=0; i<mSerializations.size(); ++i) {
			mSerializations[i]->setTag(tag);
		}
	};

	void add(AimSerializationBase* serialization) {
		serialization->setTag(mTag);
		mSerializations.push_back(serialization);
	}

	size_t getRequiredSize() {
		mNumTensors = mSerializations.size();
		mSerializedSize=2;
		for (size_t i=0; i<mNumTensors; ++i) {
			mSerializedSize += mSerializations[i]->getHeaderSize();
			mSerializedSize += mSerializations[i]->getDataSize();
		}
		return mSerializedSize;
	}

	template <typename OutputIterator>
	OutputIterator serialize(const OutputIterator first, const OutputIterator last) {
		__glibcxx_function_requires(_OutputIteratorConcept<OutputIterator>);
		__glibcxx_requires_valid_range(first, last);
		typename std::iterator_traits<OutputIterator>::difference_type size = std::distance(first, last);

		// Check size
		getRequiredSize(); // shouldn't be necessary, since it should be called before serializing
		if (size < mSerializedSize) {
			std::cerr << mTag << " WARNING: container size too small to serialize data" << std::endl;
			throw AimException();
		}

		OutputIterator it = first;

		// Write headers
		*it++ = mDataType;
		*it++ = mNumTensors;
		for (size_t i=0; i<mNumTensors; ++i) {
			it = mSerializations[i]->serializeHeader(it, last);
		}

		// Write data
		for (size_t i=0; i<mNumTensors; ++i) {
			it = mSerializations[i]->serializeData(it, last);
		}

		return it;
	}

	template <typename InputIterator>
	InputIterator deserialize(const InputIterator first, const InputIterator last) {
		__glibcxx_function_requires(_InputIteratorConcept<InputIterator>)
		__glibcxx_requires_valid_range(first, last);
		typename std::iterator_traits<InputIterator>::difference_type size = std::distance(first, last);

		if (size < 1)
			throw AimException();

		InputIterator it(first);

		int mDataType = *it++;
		if (mDataType != 0) {
			std::cerr << mTag << " WARNING: data type should be 0" << std::endl;
			throw AimException();
		}
		int mNumTensors = *it++;
		if (mNumTensors < mSerializations.size()) {
			std::cerr << mTag << " WARNING: number of tensors should be at least " << mSerializations.size() << std::endl;
			throw AimException();
		}

		// Read header of tensors
		for (size_t i=0; i<mSerializations.size(); ++i) {
			it = mSerializations[i]->deserializeHeader(it, last);
		}

		// Read header of remaining tensors
		mBaseSerializations.resize(mNumTensors - mSerializations.size());
		for (size_t i=0; i<mBaseSerializations.size(); ++i) {
			mBaseSerializations.push_back(AimSerializationBase());
			it = mBaseSerializations.back().deserializeHeader(it, last);
		}

		// Read data of tensors
		for (size_t i=0; i<mSerializations.size(); ++i) {
			it = mSerializations[i]->deserializeData(it, last);
		}

		// Read data of remaining tensors
		for (size_t i=0; i<mBaseSerializations.size(); ++i) {
			it = mBaseSerializations.back().deserializeHeader(it, last);
		}

		return it;
	}

private:
	std::string mTag;
	int mDataType;
	size_t mNumTensors;
	size_t mSerializedSize;
	std::vector<AimSerializationBase*> mSerializations;
	std::vector<AimSerializationBase> mBaseSerializations;
};


} // namespace rur

#endif /* AIMSERIALIZATION_HPP_ */
