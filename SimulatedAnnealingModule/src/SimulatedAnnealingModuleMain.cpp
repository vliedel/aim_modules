/**
 * @file SimulatedAnnealingModuleMain.cpp
 * @brief Entry function for SimulatedAnnealingModule
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

#include <stdlib.h>
#include <iostream>

using namespace rur;
using namespace std;

/**
 * Every module is a separate binary and hence has its own main method. It is recommended
 * to have a version of your code running without any middleware wrappers, so preferably
 * have this file and the SimulatedAnnealingModule header and code in a separate "aim" directory.
 */
int main(int argc, char *argv[])  {
	SimulatedAnnealingModuleExt *m = new SimulatedAnnealingModuleExt();

	if (argc < 2) {
		std::cout << "Use an identifier as argument for this instance" << endl;
		return EXIT_FAILURE;
	}
	std::string identifier = argv[1];
	m->Init(identifier);

	do {
		m->Tick();
	} while (!m->Stop()); 

	delete m;

	return EXIT_SUCCESS;
}
