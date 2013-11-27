/**
 * @file SwiftenModuleMain.cpp
 * @brief Entry function for SwiftenModule
 *
 * This file is created at "DoBots". It is open-source software and part of "AI". 
 * This software is published under the GNU General Lesser Public Licence license (GPLv3).
 *
 * Copyright © 2013 Bart van Vliet <bart@almende.org>
 *
 * @author                   Bart van Vliet
 * @date                     nov 19, 2013
 * @organisation             DoBots
 * @project                  AI
 */
 
#include <SwiftenModuleExt.h>

#include <stdlib.h>
#include <iostream>

using namespace rur;
using namespace std;

/**
 * Every module is a separate binary and hence has its own main method. It is recommended
 * to have a version of your code running without any middleware wrappers, so preferably
 * have this file and the SwiftenModule header and code in a separate "aim" directory.
 */
int main(int argc, char *argv[])  {
	SwiftenModuleExt *m = new SwiftenModuleExt();

	if (argc < 6) {
		//                                        1        2        3          4         5
		std::cout << "Usage: " << argv[0] << " username password resource to_resource filename" << std::endl;
		return EXIT_FAILURE;
	}

	std::string identifier = argv[1];
	m->Init(std::string("0"),
			std::string(argv[1]),
			std::string(argv[2]),
			std::string(argv[3]),
			std::string(argv[4]),
			std::string(argv[5])
	);

	do {
		m->Tick();
	} while (!m->Stop()); 

	delete m;

	return EXIT_SUCCESS;
}
