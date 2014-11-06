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

#include <cmath>
#define PI 3.14159265

using namespace rur;

SimulatedAnnealingModuleExt::SimulatedAnnealingModuleExt() {

//	SetState set;
//	std::vector<double> setValues;
//	setValues.push_back(1.0);
//	setValues.push_back(2.0);
//	setValues.push_back(4.0);
//	set.Init(setValues);
//	_setStates.push_back(set);

	ContState cont;
	cont.Init(-1.0, 1.0);
//	_contStates.push_back(cont);
	_state.addState(cont);

	ContState cont2;
	cont2.Init(-1.0, 1.0);
//	_contStates.push_back(cont);
	_state.addState(cont2);

	srand(time(NULL));

	// Initialize values randomly
	for (SetStateArr::iterator itSet=_state._setStates.begin(); itSet!=_state._setStates.begin(); ++itSet) {
		itSet->set(itSet->_set[rand() % itSet->size()]);
		std::cout << itSet->get() << std::endl;
	}
	for (ContStateArr::iterator itCont=_state._contStates.begin(); itCont != _state._contStates.end(); ++itCont) {
		itCont->set(randDouble(itCont->_min, itCont->_max));
//		std::cout << itCont->_min << " " << itCont->_max << std::endl;
		std::cout << itCont->get() << std::endl;
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
		// TODO: assuming only continuous statevars
		for (size_t i=0; i<_state._contStates.size(); ++i) {
			double val = _state[i] + randDouble(-0.5, 0.5);
			_candidateState.setStateVal(i, val); // TODO: Fix this, should depend on min and max
		}

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
			std::cout << "p=" << p << " deltaE=" << deltaE << " temp=" << temp << " deltaE_avg=" << deltaE_avg << std::endl;
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

		std::cout << k << " energy=" << energyBest << " state=";
		for (size_t i=0; i<_state.size(); ++i) {
			std::cout << " " << _state[i];
		}
		std::cout << std::endl;

		// Lower the temperature
		temp *= temp_frac;
	}


}

SimulatedAnnealingModuleExt::~SimulatedAnnealingModuleExt() {

}

//! Replace with your own code
void SimulatedAnnealingModuleExt::Tick() {

}

//! Replace with your own code
bool SimulatedAnnealingModuleExt::Stop() {
	return false;
}

// not super uniform, but good enough, see http://stackoverflow.com/questions/686353/c-random-float-number-generation
double SimulatedAnnealingModuleExt::randDouble(double min, double max) {
	return min + static_cast <double> (rand()) / ( static_cast <double> (RAND_MAX/(max - min)));
}

