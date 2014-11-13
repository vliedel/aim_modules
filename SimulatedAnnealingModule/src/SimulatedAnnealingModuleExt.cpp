/**
 * @file SimulatedAnnealingModuleExt.cpp
 * @brief SimulatedAnnealingModule extension
 *
 * This file is created at "Springfield Power Company". It is open-source software and part of "Project Bootstrap". 
 * This software is published under the State license (TR8OR).
 *
 * Copyright © 2014 Homer J. Simpson <chunkylover53@aol.com>
 *
 * @author                   Homer J. Simpson
 * @date                     nov  5, 2014
 * @organisation             Springfield Power Company
 * @project                  Project Bootstrap
 */

#include <SimulatedAnnealingModuleExt.h>

//#include <random>
#include <cstdlib> // rand()
//#include <ctime>
#include <iostream>
#include <sstream>

#include <cmath>
#define PI 3.14159265

#include "json/json.h"

using namespace rur;
//using namespace StateVariable;

SimulatedAnnealingModuleExt::SimulatedAnnealingModuleExt() {

//	SetState set;
//	std::vector<double> setValues;
//	setValues.push_back(1.0);
//	setValues.push_back(2.0);
//	setValues.push_back(4.0);
//	set.Init(setValues);
//	_setStates.push_back(set);

	RealState real;
	real.init(-1.1, 1.1);
//	_contStates.push_back(cont);
	_state.addState(real);

	RealState real2;
	real2.init(-1.2, 1.2);
//	_contStates.push_back(cont);
	_state.addState(real2);

	srand(time(NULL));

	// Initialize values randomly
	for (SetStateArr::iterator itSet=_state._setStates.begin(); itSet!=_state._setStates.begin(); ++itSet) {
		itSet->set(rand() % itSet->size());
		std::cout << itSet->get() << std::endl;
	}
	for (RealStateArr::iterator itReal=_state._realStates.begin(); itReal != _state._realStates.end(); ++itReal) {
		itReal->set(randDouble(itReal->min(), itReal->max()));
		std::cout << itReal->get() << std::endl;
	}



	int numSteps = 2500;

	// Probability of accepting worse solution at the start
	double p_start = 0.7;
	// Probability of accepting worse solution at the end
	double p_end = 0.001;

	double temp_start = -1.0/log(p_start);
	double temp_end = -1.0/log(p_end);
	double temp_frac = std::pow(temp_end/temp_start, 1.0/(numSteps-1));
	double temp = temp_start;

	double energyBest = 0.2 + _state[0]*_state[0] + _state[1]*_state[1]
						- 0.1*cos(6.0*PI*_state[0]) - 0.1*cos(6.0*PI*_state[1]);
	double deltaE;
	double deltaE_avg = 0.0;
	int numUpdates = 0;

	_candidateState = _state;

	for (int k=0; k<numSteps; ++k) {

		// generate random neighbour
		for (size_t i=0; i<_state._realStates.size(); ++i) {
			double val = _state[i] + randDouble(-0.5, 0.5);
			_candidateState.setStateVal(i, val); // TODO: Fix this, should depend on min and max
		}

		// TODO: assuming only continuous statevars
		// Calculate energy (writeCandidate())
		double energy = 0.2 + _candidateState[0]*_candidateState[0] + _candidateState[1]*_candidateState[1]
				- 0.1*cos(6.0*PI*_candidateState[0]) - 0.1*cos(6.0*PI*_candidateState[1]);

		deltaE = fabs(energy - energyBest);
		bool accept = false;
		if (energy > energyBest) { // Energy is worse, maybe accept it
			// init deltaE_avg
			if (k==1)
				deltaE_avg = deltaE;
			double p = exp(-deltaE/(deltaE_avg*temp));
//			std::cout << "p=" << p << " deltaE=" << deltaE << " temp=" << temp << " deltaE_avg=" << deltaE_avg << std::endl;
			if (randDouble(0.0, 1.0) < p) {
				accept = true;
			}
		}
		else { // Energy is better, accept it
			accept = true;
		}

		if (accept) {
			_state = _candidateState;
			energyBest = energy;
			numUpdates++;
			deltaE_avg = (deltaE_avg * (numUpdates-1.0) +  deltaE) / (double)numUpdates;
		}

//		std::cout << k << " energy=" << energyBest << " state=";
//		for (size_t i=0; i<_state.size(); ++i) {
//			std::cout << " " << _state[i];
//		}
//		std::cout << std::endl;

		// Lower the temperature
		temp *= temp_frac;
	}






}

SimulatedAnnealingModuleExt::~SimulatedAnnealingModuleExt() {

}

void SimulatedAnnealingModuleExt::Tick() {
	std::string* readString;
	readString = readSearchspace(false);
	if (readString != NULL && !readString->empty()) {
		std::cout << "Read: " << *readString << std::endl;
		Json::Value root;
		Json::Reader reader;
		bool parsingSuccessful = reader.parse(*readString, root, false);
		if (!parsingSuccessful) {
			// report to the user the failure and their locations in the document.
			std::cout << "Failed to parse configuration\n" << reader.getFormattedErrorMessages();
		}
		else {
//			std::cout << root << std::endl;

/* Shows how to loop over items in an object
			// Root is always of type: object
			Json::Value::Members members(root.getMemberNames());
			Json::Value::Members::iterator it = members.begin();
			for (; it != members.end(); ++it) {
				const std::string& name = *it;
				const Json::Value& childValue = root[name];
				std::cout << name << "=" << childValue << std::endl;
			}
*/

			const Json::Value jsonStateVars = root["statevars"];
			_state.readJson(jsonStateVars);

			_candidateState = _state;

			Json::Value output = _candidateState.writeJson();
			Json::FastWriter jsonWriter;
			std::string outStr = jsonWriter.write(output);
			writeCandidate(outStr);

//			// Echo to test
//			std::stringstream ss;
//			ss << root;
//			writeCandidate(ss.str());
		}
		readString->clear();
	}
}

//! Replace with your own code
bool SimulatedAnnealingModuleExt::Stop() {
	return false;
}

// not super uniform, but good enough, see http://stackoverflow.com/questions/686353/c-random-float-number-generation
double SimulatedAnnealingModuleExt::randDouble(double min, double max) {
	return min + static_cast <double> (rand()) / ( static_cast <double> (RAND_MAX/(max - min)));
}

