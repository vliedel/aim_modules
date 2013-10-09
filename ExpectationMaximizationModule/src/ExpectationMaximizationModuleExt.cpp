/**
 * @file ExpectationMaximizationModuleExt.cpp
 * @brief ExpectationMaximizationModule extension
 *
 * This file is created at Almende B.V. It is open-source software and part of the Common 
 * Hybrid Agent Platform (CHAP). A toolbox with a lot of open-source tools, ranging from 
 * thread pools and TCP/IP components to control architectures and learning algorithms. 
 * This software is published under the GNU Lesser General Public license (LGPL).
 *
 * It is not possible to add usage restrictions to an open-source license. Nevertheless,
 * we personally strongly object against this software being used by the military, in the
 * bio-industry, for animal experimentation, or anything that violates the Universal
 * Declaration of Human Rights.
 *
 * Copyright © 2013 Your Name <your@email>
 *
 * @author	Your Name
 * @date	Current date
 * @company	Your Company
 * @project	Specific Software Project
 */

#include <ExpectationMaximizationModuleExt.h>
#include <Print.hpp>
#include <unistd.h>
#include <fstream>
#include <stdint.h>
#include <iomanip> // To use std::setw()
#include <CTime.h>

using namespace rur;


ExpectationMaximizationModuleExt::ExpectationMaximizationModuleExt():
		mTrained(false)
{
//	Model = new cv::ExpectationMaximization;
	mModel = new cv::EM;
	mSamples = new cv::Mat(0, 0, CV_32FC1);
	mGroundTruth = new cv::Mat(0, 0, CV_16SC1);
}

ExpectationMaximizationModuleExt::~ExpectationMaximizationModuleExt()
{
	delete mModel;
//	delete Samples;
}

//! Add your own functionality, don't forget to call parent init
void ExpectationMaximizationModuleExt::Init(std::string & name) {
	ExpectationMaximizationModule::Init(name);

//    const int num_points = 5;
//    const int vec_length = 3;
//    cv::Mat A(num_points, vec_length, CV_32FC1);
//    cv::RNG rng(0); // Fill A with random values
//    rng.fill(A, cv::RNG::UNIFORM, 0, 1);
//    cv::Mat B = cv::Mat(0,vec_length, CV_32FC1);
//    B.push_back(A.row(0));
//    B.push_back(A.row(2));
//    B.push_back(A.row(4));
//    std::cout << "A: " << A << std::endl;
//    std::cout << "B: " << B << std::endl;
//
//	std::vector<double> v;
//	for (int i=0;i<10;++i)
//		v.push_back(1/(i+1.0));
//	cv::Mat m(1,v.size(),CV_32FC1);
//	for (int i=0; i<v.size(); ++i)
//		m.at<float>(0,i) = v[i];
//	m.resize(m.rows+1);
//	for (int i=0; i<v.size(); ++i)
//		m.at<float>(1,i) = 2*v[i];
//
//	std::cout << m << std::endl;
	//Samples->push_back(v);
	//std::cout << *Samples << std::endl;
/*

	//int nClusters = cv::ExpectationMaximization;
	cv::ExpectationMaximization* model;
	model = new cv::ExpectationMaximization;
	std::cout << "size=" << sizeof(*model) << std::endl;

	cv::EMParams params;
	params.nclusters = 4;
	std::cout << "nclusters=" << params.nclusters << std::endl;

	// Samples must be 32bit float (single channel): CV_32FC1 !
	// There must be at least <nclusters> samples
	cv::Mat samples(10*params.nclusters, 2, CV_32F);

//	cv::MatConstIterator it = samples.begin<float>(), it_end = samples.end<float>();
//	for(float v=0; it != it_end; ++it, ++v)
//		std::cout << *it;
	for (int i=0; i<samples.rows; ++i)
		for (int j=0; j<samples.cols; ++j)
			samples.at<float>(i,j)=std::rand()%10; // 0-9

	model->train(samples, cv::Mat(), params);
	std::cout << "NClusters=" << model->getNClusters() << std::endl;
	std::cout << "Trained model 1 with samples:\n" << samples << std::endl;
	std::cout << "Means:\n" << model->getMeans() << std::endl;
//	std::cout << "Probs:\n" << model->getProbs().rows << " " << model->getProbs().cols << std::endl << model->getProbs() << std::endl;
	std::cout << "Weights:\n" << model->getWeights() << std::endl;

	cv::Mat samples2(samples.rows, samples.cols, CV_32F);
	for (int i=0; i<samples2.rows; ++i)
		for (int j=0; j<samples2.cols; ++j)
			samples2.at<float>(i,j)=std::rand()%10 + 30; // 30-39
	cv::ExpectationMaximization* model2;
	model2 = new cv::ExpectationMaximization;
	model2->train(samples2, cv::Mat(), params);
	std::cout << "Trained model 2 with samples:\n" << samples2 << std::endl;
	std::cout << "Means:\n" << model2->getMeans() << std::endl;
//	std::cout << "Probs:\n" << model2->getProbs().rows << " " << model2->getProbs().cols << std::endl << model2->getProbs() << std::endl;
	std::cout << "Weights:\n" << model2->getWeights() << std::endl;

	cv::Mat samples_to_predict(1, 2, CV_32F);
	for (int i=0; i<samples_to_predict.rows; ++i)
		for (int j=0; j<samples_to_predict.cols; ++j)
			//samples_to_predict.at<float>(i,j) = std::rand()%10 + i*10; // 0-9, 10-19, 20-29
			samples_to_predict.at<float>(i,j) = samples.at<float>(i,j);

	//cv::Mat probs(samples_to_predict.size, CV_32F); // why doesn't this work?
	std::cout << "Predicting samples:\n" << samples_to_predict << std::endl;
	cv::Mat probs(samples_to_predict.rows, samples_to_predict.cols, CV_32F);
	float p = model->predict(samples_to_predict, &probs);
	std::cout << "model 1 p=" << p << " probs: " << probs << std::endl;
//	double llh = model->calcLikelihood(samples_to_predict);
//	std::cout << "model 1 llh=" << llh << std::endl;

//	double minVal, maxVal;
//	int minIdx, maxIdx;
//	minMaxIdx(probs, &minVal, &maxVal, &minIdx, &maxIdx, cv::noArray());
//	std::cout << "minVal=" << minVal << " maxVal=" << maxVal << " minIdx=" << minIdx << " maxIdx=" << maxIdx << std::endl;

	std::cout << "Predicting samples:\n" << samples_to_predict << std::endl;
	cv::Mat probs2(samples_to_predict.rows, samples_to_predict.cols, CV_32F);
	float p2 = model2->predict(samples_to_predict, &probs2);
	std::cout << "model 2 p=" << p2 << " probs: " << probs2 << std::endl;
//	minMaxIdx(probs2, &minVal, &maxVal, &minIdx, &maxIdx, cv::noArray());
//	std::cout << "minVal=" << minVal << " maxVal=" << maxVal << " minIdx=" << minIdx << " maxIdx=" << maxIdx << std::endl;
//	double llh2 = model2->calcLikelihood(samples_to_predict);
//	std::cout << "model 2 llh=" << llh2 << std::endl;



	cv::Mat samples3(samples.rows*2, samples.cols, CV_32F);
	for (int i=0; i<samples.rows; ++i)
		for (int j=0; j<samples.cols; ++j)
			samples3.at<float>(i,j)=samples.at<float>(i,j);
	for (int i=0; i<samples2.rows; ++i)
		for (int j=0; j<samples2.cols; ++j)
			samples3.at<float>(i+samples.rows,j)=samples2.at<float>(i,j);
	cv::ExpectationMaximization* model3;
	model3 = new cv::ExpectationMaximization;
	model3->train(samples3, cv::Mat(), params);
	std::cout << "Trained model 3 with samples:\n" << samples3 << std::endl;
	std::cout << "Means:\n" << model3->getMeans() << std::endl;
//	std::cout << "Probs:\n" << model3->getProbs().rows << " " << model3->getProbs().cols << std::endl << model3->getProbs() << std::endl;
	std::cout << "Weights:\n" << model3->getWeights() << std::endl;

	cv::Mat means3 = model3->getMeans();
	cv::Mat weights3 = model3->getWeights();

	std::vector<cv::Mat> covs3;
	model3->getCovs(covs3);
	for (std::vector<cv::Mat>::iterator it = covs3.begin(); it != covs3.end(); ++it)
		std::cout << "Cov: " << *it << std::endl;

	samples_to_predict.at<float>(0,0) = 4;
	samples_to_predict.at<float>(0,1) = 2;

	std::cout << "Predicting samples:\n" << samples_to_predict << std::endl;
	//cv::Mat probs3(samples_to_predict.rows, samples_to_predict.cols, CV_32F);
	cv::Mat probs3;
	float i3 = model3->predict(samples_to_predict, &probs3);
	std::cout << "model 3 i=" << i3 << " probs: " << probs3 << std::endl;
//	minMaxIdx(probs3, &minVal, &maxVal, &minIdx, &maxIdx, cv::noArray());
//	std::cout << "minVal=" << minVal << " maxVal=" << maxVal << " minIdx=" << minIdx << " maxIdx=" << maxIdx << std::endl;

	std::vector<double>probabilities(model3->getNClusters());
//	cv::Mat p3(model3->getNClusters(), 1, CV_64F);
	cv::Mat predictSample;
	samples_to_predict.convertTo(predictSample, CV_64F);
	double psum(0);
	for (int i=0; i<model3->getNClusters(); ++i)
	{
		cv::Mat r(1,1,CV_64F);
		cv::Mat diff(1,predictSample.cols,CV_64F);
		diff = predictSample - means3.row(i);
		std::cout << "diff: " << diff << std::endl;
		r = diff * covs3[i].inv() * diff.t();
		std::cout << "r=" << r << std::endl;
		double amp = 1/(std::pow(2*CV_PI,samples_to_predict.cols/2) * std::pow(cv::determinant(covs3[i]),0.5));
		std::cout << "amp=" << amp << std::endl;
//		p3.at<double>(i,0) = amp * std::exp(-0.5*r.at<double>(0,0));
		probabilities[i] = amp * std::exp(-0.5*r.at<double>(0,0));
//		std::cout << "p[" << i <<"]=" << p3.at<double>(i,0) << std::endl;
		std::cout << "p[" << i <<"]=" << probabilities[i] << " weight[" << i << "]=" << weights3.at<double>(0,i) << std::endl;
//		psum += weights3.at<double>(0,i) * p3.at<double>(i,0);
		psum += weights3.at<double>(0,i) * probabilities[i];
//		std::cout << "psum=" << psum << std::endl;
	}
	std::cout << "psum=" << psum << std::endl;
	for (int i=0; i<model3->getNClusters(); ++i)
	{
//		std::cout << "p" << i << "=" << weights3.at<double>(0,i) * p3.at<double>(i,0) / psum << std::endl;
		std::cout << "p" << i << "=" << weights3.at<double>(0,i) * probabilities[i] / psum << std::endl;
	}

//	double llh3 = model3->calcLikelihood(samples_to_predict);
//	std::cout << "model 3 llh=" << llh3 << std::endl;

*/

}

//! Replace with your own functionality
void ExpectationMaximizationModuleExt::Tick() {
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
		Evaluate();
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

	usleep(100);
}

//! Replace with your own functionality
bool ExpectationMaximizationModuleExt::Stop() {
	return false;
}

void ExpectationMaximizationModuleExt::AddSample(std::vector<float> &sample) {
//	std::cout << "add sample: ";
//	dobots::print(sample.begin(), sample.end());
//		std::cout << "Samples size=" << Samples->rows << "x" << Samples->cols << std::endl;
	int r = mSamples->rows;
	if (r == 0 || mSamples->cols == sample.size()-1) {
		if (r == 0) {
			delete mSamples;
			delete mGroundTruth;
			mSamples = new cv::Mat(1, sample.size()-1, CV_32FC1);
			mGroundTruth = new cv::Mat(1, 1, CV_16SC1);
			mTrained = false;
		}
		else {
			mSamples->resize(r+1);
			mGroundTruth->resize(r+1);
		}

		float* sampleRow = mSamples->ptr<float>(r);

		// Last number of the vector is the class
		for (int j=0; j<sample.size()-1; ++j)
			sampleRow[j] = sample.at(j);

		int16_t* labelRow = mGroundTruth->ptr<int16_t>(r);
		labelRow[0]=sample.back();


//				Samples->at<float>(r,j) = readVec->at(j);
		//Samples->push_back(*readVec);
//			std::cout << "Added sample" << std::endl;
	}
}

void ExpectationMaximizationModuleExt::Train() {

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

	// Fill mLabels, such that mLabels[label] maps to a number from 0 to nclusters-1
	mLabels.resize(*std::max_element(labels.begin(), labels.end()) + 1);
	int i=0;
	for (std::vector<int>::const_iterator it=labels.begin(); it!=labels.end(); ++it) {
		mLabels[*it] = i++;
	}
//	std::cout << "mLabels: ";
//	dobots::print(mLabels.begin(), mLabels.end());



	if (mSamples->rows > mModel->get<int>("nclusters"))
	{
		// Model->train(*Samples, cv::Mat(), Params);
//		std::cout << "Samples:\n";
//		std::cout << cv::format(*mSamples, "C") << std::endl;
		std::cout << "Training model with " << mModel->get<int>("nclusters") << " clusters" << std::endl;
		mModel->train(*mSamples);
		mTrained = true;
	}
}

bool ExpectationMaximizationModuleExt::WriteModel()
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

void ExpectationMaximizationModuleExt::Evaluate() {
	if (!mTrained)
		return;

	int features = mModel->get<cv::Mat>("means").cols;
	int clusters = mModel->get<int>("nclusters");

	//if (sampleVec.size() != features)
	//	return;

//	std::cout << "Labels:\n" << format(*mGroundTruth, "C") << std::endl;

	size_t a, b, c, d; a = b = c = d = 0;

	std::vector<cv::Vec2d> predictions(mSamples->rows);

	// Calculate predictions
	for (int r1=0; r1<mSamples->rows; ++r1)
		predictions[r1] = mModel->predict(mSamples->row(r1));

	// Calculate the label matrix (ground truth vs predictions)
	std::vector< std::vector <size_t> > lab;
	lab.resize(clusters);
	for (int i=0; i<clusters; ++i) {
		lab[i].resize(clusters);
		for (int j=0; j<clusters; ++j)
			lab[i][j]=0;
	}
	for (int r1=0; r1<mSamples->rows; ++r1) {
		lab[mLabels[mGroundTruth->at<int16_t>(r1)]][predictions[r1].val[1]]++;
	}

	// Print the label matrix
	for (int i=0; i<clusters; ++i) {
		std::cout << "label " << i <<": ";
		for (int j=0; j<clusters; ++j)
			std::cout << std::setw(3) << std::fixed << lab[i][j] << " ";
		std::cout << std::endl;
	}

	// Calculate Rand index
	for (int r1=0; r1<mSamples->rows; ++r1) {
		for (int r2=0; r2<mSamples->rows; ++r2) {
			if (mGroundTruth->at<int16_t>(r1) == mGroundTruth->at<int16_t>(r2)) {
				if ((int)predictions[r1].val[1] == (int)predictions[r2].val[1])
					++a;
				else
					++c;
			}
			else {
				if ((int)predictions[r1].val[1] == (int)predictions[r2].val[1])
					++d;
				else
					++b;
			}

		}
	}
	float quality = (a+b) / float(a+b+c+d);
	std::cout << "Rand index is " << quality << std::endl;
}

void ExpectationMaximizationModuleExt::Load(std::string &file) {
	long startTime = get_cur_1ms();

	std::ifstream fs;
	fs.open(file.c_str());
	int cols, rows;
	fs >> cols >> rows;
	std::cout << "rows=" << rows << " cols=" << cols << std::endl;
	std::vector<float> sample(cols);

	for (int r=0; r < rows; ++r) {
		for (float_seq::iterator it = sample.begin(); it != sample.end(); ++it) {
			fs >> *it;
			//std::cout << *it << " ";
		}
		AddSample(sample);
	}
	fs.close();
	long readTime = get_cur_1ms();
	std::cout << "Reading data took " << readTime - startTime << " ms" << std::endl;

	Train();
	long trainTime = get_cur_1ms();
	std::cout << "Training took " << trainTime - readTime << " ms" << std::endl;

	WriteModel();
	long writeTime = get_cur_1ms();
	std::cout << "Writing took " << writeTime - trainTime << " ms" << std::endl;

	Evaluate();
	long evalTime = get_cur_1ms();
	std::cout << "Evaluating took " << evalTime - writeTime << " ms" << std::endl;

	std::cout << "All took: " << get_cur_1ms() - startTime << " ms" << std::endl;
}

void ExpectationMaximizationModuleExt::SetNumClusters(int num) {
	mModel->set("nclusters", num);
	std::cout << "Set number of clusters to: " << num << std::endl;
}

/*
bool ExpectationMaximizationModuleExt::Predict(std::vector<float>& sampleVec, std::vector<float>* probs)
{
	if (!mTrained)
		return false;

	//int features = Model->getMeans().cols;
	int features = mModel->get<cv::Mat>("means").cols;
	//int clusters = Model->getNClusters();
	int clusters = mModel->get<int>("nclusters");

	if (sampleVec.size() != features)
		return false;

	cv::Mat sample(1, features, CV_64FC1);
	double* m = sample.ptr<double>(0);
	for (int j=0; j<sampleVec.size(); ++j)
		m[j] = sampleVec[j];

	//Model->predict(); // Bugged!

	double psum(0);
	cv::Mat r(1, 1, CV_64F);
	cv::Mat diff(1, features, CV_64F);
	//std::vector<cv::Mat> covs;
	//Model->getCovs(covs);
	std::vector<cv::Mat> covs = mModel->get<std::vector<cv::Mat> >("covs");
	std::vector<double> probsD;
	for (int i=0; i<clusters; ++i)
	{
		//diff = sample - Model->getMeans().row(i);
		diff = sample - mModel->get<cv::Mat>("means").row(i);
		r = diff * covs[i].inv() * diff.t();
		double amp = 1/(std::pow(2*CV_PI, features/2) * std::pow(cv::determinant(covs[i]), 0.5));
		probsD.push_back(amp * std::exp(-0.5*r.at<double>(0,0)));
		//psum += Model->getWeights().at<double>(0,i) * probsD[i];
		psum += mModel->get<cv::Mat>("weights").at<double>(0,i) * probsD[i];

	}

	// Normalize
	probs->clear();
	for (int i=0; i<clusters; ++i)
	{
		//probs->push_back(Model->getWeights().at<double>(0,i) * probsD[i] / psum);
		probs->push_back(mModel->get<cv::Mat>("weights").at<double>(0,i) * probsD[i] / psum);
	}

	return true;
}
*/
