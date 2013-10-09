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
 * Copyright © 2012 Anne van Rossum <anne@almende.com>
 *
 * @author  ...
 * @date    ...
 * @company Almende B.V.
 * @case    Artificial Intelligence Framework
 */

#include <ExpectationMaximizationModule.h>
#include "opencv2/ml/ml.hpp"
//#include "/opt/ros/hydro/include/opencv2/ml/ml.hpp"
namespace rur {

class ExpectationMaximizationModuleExt: public ExpectationMaximizationModule {
private:
	//std::vector<cv::ExpectationMaximization::CvEM*> Models;
//		cv::ExpectationMaximization* Model;
	cv::EM* mModel;
	cv::Mat* mSamples;
	cv::Mat* mGroundTruth;
	std::vector<int> mLabels;
//		cv::InputArray Samples;
//		cv::EMParams Params;
	bool mTrained;

	// Write the gmm to the output port
	bool WriteModel();

	// Train the model
	void Train();

	// Evaluate how similar the model is to the ground truth (labels)
	void Evaluate();

	// Add a sample, last number in the vector is the label
	void AddSample(std::vector<float> &sample);

	// Set the number of clusters
	void SetNumClusters(int num);

public:
	ExpectationMaximizationModuleExt();
	~ExpectationMaximizationModuleExt();
	void Init(std::string & name);

	// The tick function will be called from the ExpectationMaximizationModuleMain file
	void Tick();

	// As soon as Stop() returns "true", the ExpectationMaximizationModuleMain will stop the module
	bool Stop();

	// Load samples from a file, train and evaluate.
	// First like of the file should be: nr_of_columns nr_of_rows
	// Next nr_of_rows lines should contain nr_of_columns numbers separated by a space
	void Load(std::string &file);
};

}

