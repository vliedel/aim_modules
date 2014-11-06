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

#include "SearchSpace.h"

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
	typedef StateVariableSet<double> SetState;
	typedef StateVariableContinuous<double> ContState;
	typedef std::vector<SetState> SetStateArr;
	typedef std::vector<ContState> ContStateArr;

	State<double> _state;
	State<double> _candidateState;

//	SetStateArr _setStates;
//	ContStateArr _contStates;

	double randDouble(double start, double end);
};

}

