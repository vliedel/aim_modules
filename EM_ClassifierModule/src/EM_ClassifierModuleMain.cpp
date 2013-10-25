/**
 * @file EM_ClassifierModuleMain.cpp
 * @brief Entry function for EM_ClassifierModule
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

#include <stdlib.h>
#include <iostream>

using namespace rur;
using namespace std;

/**
 * Every module is a separate binary and hence has its own main method. It is recommended
 * to have a version of your code running without any middleware wrappers, so preferably
 * have this file and the EM_ClassifierModule header and code in a separate "aim" directory.
 */
int main(int argc, char *argv[])  {
	EM_ClassifierModuleExt *m = new EM_ClassifierModuleExt();

	if (argc < 2) {
		std::cout << "Usage: " << argv[0] << " <id> [filename]"  << endl;
		return EXIT_FAILURE;
	}
	std::string identifier = argv[1];
	m->Init(identifier);

	if (argc >= 3) {
		std::string file = argv[2];
		m->Load(file);
	}

	do {
		m->Tick();
	} while (!m->Stop()); 

	delete m;

	return EXIT_SUCCESS;
}
