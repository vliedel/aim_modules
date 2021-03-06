/**
 * @file SimulatedAnnealingModuleExt.h
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

#include <SimulatedAnnealingModule.h>

#include "StateVariable.hpp"

namespace rur {

/**
 * Your Description of this module.
 */

class SimulatedAnnealingModuleExt: public SimulatedAnnealingModule {
public:
	//! The constructor
	SimulatedAnnealingModuleExt();

	//! The destructor
	virtual ~SimulatedAnnealingModuleExt();

	//! The tick function is the "heart" of your module, it reads and writes to the ports
	void Tick();

	//! As soon as Stop() returns "true", the SimulatedAnnealingModuleMain will stop the module
	bool Stop();
private:
	typedef std::string SetStateType;
	typedef int IntStateType;
	typedef double RealStateType;

	typedef StateVariableSet<SetStateType> SetState;
	typedef StateVariableInt<IntStateType> IntState;
	typedef StateVariableReal<RealStateType> RealState;
	typedef std::vector<SetState> SetStateArr;
	typedef std::vector<IntState> IntStateArr;
	typedef std::vector<RealState> RealStateArr;

	State<double> _state;
	State<double> _candidateState;

//	SetStateArr _setStates;
//	ContStateArr _contStates;

	std::string _stateVarsId;

	/* Parameters */
	// Probability of accepting worse solution at the start
	double _p_start;// = 0.7;
	// Probability of accepting worse solution at the end
	double _p_end;// = 0.001;
	// Number of iterations
	double _numSteps;//= 2500;

	/* algorithm vars */
	double _avgDeltaE; // average delta energy
	double _tempMult; // temperature is multiplied by this number each iteration
	double _temp; // current temperature
	double _energyBest; // current best energy
	int _numIterations;
	int _numUpdates;

	void initCandidate();
	void calcNewCandidate(double energy);
	void sendCandidate();

	double randDouble(double start, double end);
};

}

