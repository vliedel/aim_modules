/**
 * @file BrightnessToMorseModuleExt.cpp
 * @brief BrightnessToMorseModule extension
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

#include <BrightnessToMorseModuleExt.h>
#include "CTime.h"
#include <unistd.h>

using namespace rur;

BrightnessToMorseModuleExt::BrightnessToMorseModuleExt() :
		mBrightnessAvg(0),
		mTimeUnit(500)
{
	// 2 clusters, 1 dimension
	mKMeans = new KMeans(2,1);
}

BrightnessToMorseModuleExt::~BrightnessToMorseModuleExt() {
	delete mKMeans;
}

void BrightnessToMorseModuleExt::Tick() {
//	long_seq* readVec = readVideo(false);
//	if (readVec != NULL && !readVec->empty()) {
//		std::cout << "Read something" << std::endl;
//		long_seq::const_iterator it = readVec->begin();
//		int dataType = *it++;
//		if (dataType != 0)
//			return;
//		int nArrays = *it++;
//		int nDims = *it++;
//		if (nDims != 3) {
//			readVec->clear();
//			return;
//		}
//		int height = *it++;
//		int width = *it++;
//		int channels = *it++;
//
//		long sum = 0;
//		for (int i=0; i<height*width*channels; ++i) {
//			sum += *it++;
//		}
//
//		mSums.push_back(sum);
//		mTimes.push_back(get_cur_1ms());
//
//		std::vector<float> sample;
//		sample.push_back(sum);
//		mKMeans->addSample(sample, 0);
//
//		for (int i=0; i<1; ++i) {
//			mKMeans->tick();
//		}
//
//
//
//		std::vector<long>::const_iterator itSum = mSums.begin();
//		std::vector<long>::const_iterator itTime = mTimes.begin();
//		for (; itSum!=mSums.end(); ++itSum, ++itTime) {
//			std::cout << *itTime << " " << *itSum << std::endl;
//		}
//
//		std::cout << "Means:" << std::endl;
//		mKMeans->print();
//		std::cout << std::endl;
//
//
////		CImg<unsigned char> img(width, height, 1, channels);
////		for (int y=0; y<height; ++y) {
////			for (int x=0; x<width; ++x) {
////				for (int c=0; c<channels; ++c) {
////					img(x,y,c) = *it++;
////				}
////			}
////		}
//
//		readVec->clear();
//	}

	float* brightness = readBrightness(false);
//	usleep(10*1000);
	if (brightness != NULL) {

		// Use a moving average of 5 samples
		if (mBrightnessHistory.size() >= 5) {
			mBrightnessAvg += (*brightness - mBrightnessHistory.front()) / mBrightnessHistory.size();
			mBrightnessHistory.pop_front();
			mBrightnessHistory.push_back(*brightness);
		}
		else {
			mBrightnessHistory.push_back(*brightness);
			mBrightnessAvg = 0;
			std::deque<float>::const_iterator it;
			for (it = mBrightnessHistory.begin(); it != mBrightnessHistory.end(); ++it) {
				mBrightnessAvg += *it;
			}
			mBrightnessAvg /= mBrightnessHistory.size();
		}

		std::cout << "Moving average=" << mBrightnessAvg << std::endl;

		mSums.push_back(mBrightnessAvg);
		mTimes.push_back(get_cur_1ms());

		std::vector<float> sample(1);
		sample[0] = mBrightnessAvg;
		mKMeans->addSample(sample, 0, 1);

		for (int i=0; i<1; ++i) {
			mKMeans->tick();
		}

//		writeMorse("hui");

		// After at least 30 seconds, try calculating the morse code
		if (mTimes.size() > 2 && get_duration(mTimes.front(), mTimes.back()) > 30*mTimeUnit) {
			calcMorse();
		}
	}

//	// Test
//	if (mSums.size() > 5*20) {
//		writeMorse("hi");
//		reset();
//	}



//	// If the module doesn't get data for some time, start analyzing the data
//	if (!mTimes.empty() && get_duration(mTimes.back(), get_cur_1ms()) > 3*1000) {
//		calcMorse();
//		reset();
//	}

	// lag=5; for i=[1:length(b)-lag]; c(i)=mean(b(i:i+lag)); end; d=find(c>m); e=find(b>m); figure(1); plot(b); hold on; plot(c, 'r'); plot(d,c(d),'x'); plot([0 length(b)], [m m]); hold off; figure(2); plot(diff(d),'x'); figure(3); plot(diff(e),'x');
	// t=A(:,1); b=A(:,2); e=find(b>m); figure(1); plot(b); hold on; plot(e,b(e),'x'); plot([0 length(b)], [m m]); hold off; figure(2); figure(2); plot(diff(e),'x');

	usleep(20*1000);
}

bool BrightnessToMorseModuleExt::Stop() {
	return false;
}

void BrightnessToMorseModuleExt::calcMorse() {
	// Train a bit more?
	for (int i=0; i<1; ++i) {
		mKMeans->tick();
	}

	std::vector<float>::const_iterator itSum;
	std::vector<long>::const_iterator itTime;

	// Print samples and kMeans
	itSum = mSums.begin();
	itTime = mTimes.begin();
	for (; itSum!=mSums.end(); ++itSum, ++itTime) {
//		std::cout << *itTime << " " << *itSum << std::endl;
	}
//	std::cout << "Means:" << std::endl;
//	mKMeans->print();
//	std::cout << std::endl;


	// Label data
	std::vector<float> sample(1);
	std::vector<int> labels(mSums.size());
	std::vector<int>::iterator itLabel = labels.begin();
	for (itSum=mSums.begin(); itSum!=mSums.end(); ++itSum, ++itLabel) {
		sample[0] = *itSum;
		*itLabel = mKMeans->test(sample, 1);
//			std::cout << "label " << *itSum << " as " << *itLabel << std::endl;
	}


	// Search for a switch in label: from 1 to 0 or 0 to 1
	// store the time at which this happens
//	std::cout << "Search for a switches in label" << std::endl;
	itTime = mTimes.begin();
	itLabel = labels.begin();
	int lastSwitchLabel = *itLabel;
	std::vector<long> switchTimeStamps;
	switchTimeStamps.push_back(*itTime);
	++itSum; ++itTime; ++itLabel;

	for (; itLabel!=labels.end(); ++itTime, ++itLabel) {
		if (*itLabel != lastSwitchLabel) {
			switchTimeStamps.push_back(*itTime);
			lastSwitchLabel = *itLabel;
		}
	}

	// Calculate time between switches
//	std::cout << "Switch times:" << std::endl;
	std::vector<long> switchTimeDurations;
	std::vector<long>::const_iterator itSwitchTime = switchTimeStamps.begin();
	long lastSwitchTime = *itSwitchTime++;
	for (; itSwitchTime != switchTimeStamps.end(); ++itSwitchTime) {
		switchTimeDurations.push_back(get_duration(lastSwitchTime, *itSwitchTime));
//			std::cout << switchTimeDurations.back() << std::endl;
		lastSwitchTime = *itSwitchTime;
	}

	bool start = false;
	bool up = false;
	std::string output; output.clear();
	std::vector<long>::const_iterator itDuration = switchTimeDurations.begin();
	for (; itDuration != switchTimeDurations.end(); ++itDuration) {
//		std::cout << "duration=" << *itDuration << " start=" << start << " up=" << up << " output=" << output << std::endl;
		if (start) {
			if (up) {
				if (*itDuration < 2*mTimeUnit)
					output += "1";
				else
					output += "111";
				up = false;
			}
			else {
				if (*itDuration < 2*mTimeUnit)
					output += "0";
				else if (*itDuration < 5*mTimeUnit)
					output += "000";
				else if (*itDuration < 11*mTimeUnit) {
					output += "0000000";
				}
				else {
					std::cout << "morse output: " << output << std::endl;
					writeMorse(output);
					std::cout << "still working!" << std::endl;
					// Clear data
					reset();
//					break;
					return;
				}
				up = true;
			}
		}
		if (!start && *itDuration >= 11*mTimeUnit) {
			start = true;
			up = true;
		}
	}

//	// Test
	std::cout << "mSums.size()=" << mSums.size() << std::endl;
	if (!output.empty() && !mSums.size() % 20) {
		std::cout << "test output: " << output << std::endl;
		writeMorse(output);
//		std::cout << "still working!" << std::endl;
	}
}

void BrightnessToMorseModuleExt::reset() {
	mBrightnessHistory.clear();
	mSums.clear();
	mTimes.clear();
	delete mKMeans;
	mKMeans = new KMeans(2,1);
}

