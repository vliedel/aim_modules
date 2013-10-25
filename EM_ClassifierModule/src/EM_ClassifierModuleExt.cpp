/**
 * @file EM_ClassifierModuleExt.cpp
 * @brief EM_ClassifierModule extension
 *
 * This file is created at "Springfield Power Company". It is open-source software and part of "Project Bootstrap". 
 * This software is published under the State license (TR8OR).
 *
 * Copyright © 2013 Homer J. Simpson <chunkylover53@aol.com>
 *
 * @author                   Homer J. Simpson
 * @date                     okt 25, 2013
 * @organisation             Springfield Power Company
 * @project                  Project Bootstrap
 */

#include <EM_ClassifierModuleExt.h>
#include <Print.hpp>
#include <unistd.h> // To use usleep()
#include <fstream>
#include <stdint.h>
#include <iomanip> // To use std::setw()
#include <CTime.h>

using namespace rur;

EM_ClassifierModuleExt::EM_ClassifierModuleExt():
		mTrained(false) {
	mModel = new cv::EM;
	mSamples = new cv::Mat(0, 0, CV_32FC1);
	mGroundTruth = new cv::Mat(0, 0, CV_16SC1);
}

EM_ClassifierModuleExt::~EM_ClassifierModuleExt() {
	delete mModel;
//	delete mSamples;
}

//! Add your own functionality, don't forget to call parent init
void EM_ClassifierModuleExt::Init(std::string & name) {
	EM_ClassifierModule::Init(name);
}

void EM_ClassifierModuleExt::Tick() {
	std::vector<float>* readVec;

	readVec = readSample(false);
	if (!readVec->empty()) {
		AddSample(*readVec);
		readVec->clear();
	}

	int* readInt = readTrain(false);
	if (readInt != NULL) {
		std::cout << "read train: " << *readInt << std::endl;
		Train();
		WriteModel();
	}

	readVec = readClassify(false);
	if (!readVec->empty()) {
		std::cout << "read classify: ";
		dobots::print(readVec->begin(), readVec->end());
		std::vector<float> writeVec;
//		if (Predict(*readVec, &writeVec))
//			writeClassified(writeVec);

		readVec->clear();
	}

	readVec = readSetParameters(false);
	if (!readVec->empty()) {
		int clusters = readVec->at(0);
		SetNumClusters(clusters);
		readVec->clear();
	}

	usleep(1000);
}

bool EM_ClassifierModuleExt::Stop() {
	return false;
}



void EM_ClassifierModuleExt::AddSample(std::vector<float> &sample) {
//	std::cout << "add sample: ";
//	dobots::print(sample.begin(), sample.end());
//		std::cout << "Samples size=" << Samples->rows << "x" << Samples->cols << std::endl;

	int sampleRows = sample.front();
	int sampleCols = (sample.size() - 1) / sampleRows;

	int rows = mSamples->rows;

	if (rows == 0 || mSamples->cols == sampleCols) {
		if (rows == 0) {
			delete mSamples;
			mSamples = new cv::Mat(sampleRows, sampleCols, CV_32FC1);

			delete mGroundTruth;
			mGroundTruth = new cv::Mat(sampleRows, 1, CV_16SC1);

			mTrained = false;
		}
		else {
			mSamples->resize(rows+sampleRows);
			mGroundTruth->resize(rows+sampleRows);
		}

		for (int r = rows; r < rows+sampleRows; ++r) {
			// First number of the row is the class
			int16_t* labelRow = mGroundTruth->ptr<int16_t>(r);
			labelRow[0]=sample.at(1+r*sampleCols);
			float* sampleRow = mSamples->ptr<float>(r);
			for (int c=1; c<sampleCols; ++c)
				sampleRow[c] = sample.at(1+r*sampleCols+c+1);

//				Samples->at<float>(r,j) = readVec->at(j);
		//Samples->push_back(*readVec);
//			std::cout << "Added sample" << std::endl;
		}
	}
}

void EM_ClassifierModuleExt::Train() {

	// Find the number of unique labels
	std::vector<int> labels;
	bool found;
	int16_t label;
	for (int r=0; r<mSamples->rows; ++r) {
		found = false;
		label = mGroundTruth->at<int16_t>(r);
//		std::cout << label << " ";
		for (int l=0; l<labels.size(); ++l) {
			if (label == labels[l]) {
				found = true;
				break;
			}
		}
		if (!found)
			labels.push_back(label);
	}
	std::cout << "\nUnique labels (" << labels.size() << "): " << std::endl;
	dobots::print(labels.begin(), labels.end());

	SetNumClusters(labels.size());

	if (mSamples->rows > mModel->get<int>("nclusters"))
	{
		// Model->train(*Samples, cv::Mat(), Params);
//		std::cout << "Samples:\n";
//		std::cout << cv::format(*mSamples, "C") << std::endl;
		std::cout << "Training model with " << mModel->get<int>("nclusters") << " clusters" << std::endl;
		mModel->train(*mSamples, *mGroundTruth);
		mTrained = true;
	}
}

bool EM_ClassifierModuleExt::WriteModel()
{
	// Send the model
	// cv::Mat means = Model->getMeans();
	cv::Mat means = mModel->get<cv::Mat>("means");
	// cv::Mat weights = Model->getWeights();
	cv::Mat weights = mModel->get<cv::Mat>("weights");
	// std::vector<cv::Mat> covs
	// Model->getCovs(covs);
	std::vector<cv::Mat> covs = mModel->get<std::vector<cv::Mat> >("covs");
	std::vector<float> write;

	std::cout << "means:\n" << cv::format(means, "C") << std::endl;
	std::cout << "weights:\n" << cv::format(weights, "C") << std::endl;
	std::cout << "covs: " << std::endl;
	for (std::vector<cv::Mat>::const_iterator it=covs.begin(); it != covs.end(); ++it) {
		std::cout << cv::format(*it, "C") << std::endl;
	}

	write.push_back(means.rows);
	write.push_back(means.cols);
	for (int i=0; i<means.rows; ++i) {
		const double* mi = means.ptr<double>(i);
		for (int j=0; j<means.cols; ++j)
			write.push_back(mi[j]);
	}
	for (int i=0; i<weights.rows; ++i) {
		const double* mi = weights.ptr<double>(i);
		for (int j=0; j<weights.cols; ++j)
			write.push_back(mi[j]);
	}
	for (int k=0; k<covs.size(); ++k) {
		for (int i=0; i<covs[k].rows; ++i) {
			const double* mi = covs[k].ptr<double>(i);
			for (int j=0; j<covs[k].cols; ++j)
				write.push_back(mi[j]);
		}
	}
	return writeModel(write);
}

void EM_ClassifierModuleExt::Load(std::string &file) {
	long startTime = get_cur_1ms();

	std::ifstream fs;
	fs.open(file.c_str());
	int cols, rows;
	fs >> cols >> rows;
	std::cout << "rows=" << rows << " cols=" << cols << std::endl;
	std::vector<float> samples(cols*rows+1);

	float_seq::iterator it = samples.begin();
	*it++ = rows;
	for (int r=0; r < rows; ++r) {
		for (; it != samples.end(); ++it) {
			fs >> *it;
			//std::cout << *it << " ";
		}
	}
	AddSample(samples);
	fs.close();
	long readTime = get_cur_1ms();
	std::cout << "Reading data took " << readTime - startTime << " ms" << std::endl;

	Train();
	long trainTime = get_cur_1ms();
	std::cout << "Training took " << trainTime - readTime << " ms" << std::endl;

	WriteModel();
	long writeTime = get_cur_1ms();
	std::cout << "Writing took " << writeTime - trainTime << " ms" << std::endl;

	std::cout << "All took: " << get_cur_1ms() - startTime << " ms" << std::endl;
}

void EM_ClassifierModuleExt::SetNumClusters(int num) {
	mModel->set("nclusters", num);
	std::cout << "Set number of clusters to: " << num << std::endl;
}
