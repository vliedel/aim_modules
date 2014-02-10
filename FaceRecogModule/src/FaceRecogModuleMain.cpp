/**
 * @file FaceRecogModuleMain.cpp
 * @brief Entry function for FaceRecogModule
 *
 * This file is created at "DoBots". It is open-source software and part of "AI". 
 * This software is published under the GNU General Lesser Public Licence license (GPLv3).
 *
 * Copyright © 2014 Bart van Vliet <bart@almende.org>
 *
 * @author                   Bart van Vliet
 * @date                     feb  7, 2014
 * @organisation             DoBots
 * @project                  AI
 */
 
#include <FaceRecogModuleExt.h>

#include <stdlib.h>
#include <iostream>

using namespace rur;
using namespace std;

/**
 * Every module is a separate binary and hence has its own main method. It is recommended
 * to have a version of your code running without any middleware wrappers, so preferably
 * have this file and the FaceRecogModule header and code in a separate "aim" directory.
 */
int main(int argc, char *argv[])  {
	FaceRecogModuleExt *m = new FaceRecogModuleExt();

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
