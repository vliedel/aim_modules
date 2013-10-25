/**
 * @file EM_ClassifierModuleExt.h
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

#include <EM_ClassifierModule.h>
#include "opencv2/ml/ml.hpp"

namespace rur {

/**
 * Your Description of this module.
 */
class EM_ClassifierModuleExt: public EM_ClassifierModule {
public:
	EM_ClassifierModuleExt();
	virtual ~EM_ClassifierModuleExt();
	void Init(std::string & name);

	//! The tick function is the "heart" of your module, it reads and writes to the ports
	void Tick();

	//! As soon as Stop() returns "true", the EM_ClassifierModuleMain will stop the module
	bool Stop();

	// Load samples from a file, train and evaluate.
	// First like of the file should be: nr_of_columns nr_of_rows
	// Next nr_of_rows lines should contain nr_of_columns numbers separated by a space
	void Load(std::string &file);


private:
	cv::EM* mModel;
	cv::Mat* mSamples;
	cv::Mat* mGroundTruth;
	bool mTrained;

	// Write the gmm to the output port
	bool WriteModel();

	// Train the model
	void Train();

	// Add a sample, last number in the vector is the label
	void AddSample(std::vector<float> &sample);

	// Set the number of clusters
	void SetNumClusters(int num);
};

}

