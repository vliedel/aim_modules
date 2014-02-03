/**
 * @file FaceDetectModuleMain.cpp
 * @brief Entry function for FaceDetectModule
 *
 * This file is created at "DoBots". It is open-source software and part of "AI". 
 * This software is published under the GNU General Lesser Public Licence license (GPLv3).
 *
 * Copyright © 2014 Bart van Vliet <bart@almende.org>
 *
 * @author                   Bart van Vliet
 * @date                     jan 31, 2014
 * @organisation             DoBots
 * @project                  AI
 */
 
#include <FaceDetectModuleExt.h>

#include <stdlib.h>
#include <iostream>

using namespace rur;
using namespace std;

/**
 * Every module is a separate binary and hence has its own main method. It is recommended
 * to have a version of your code running without any middleware wrappers, so preferably
 * have this file and the FaceDetectModule header and code in a separate "aim" directory.
 */
int main(int argc, char *argv[])  {
	FaceDetectModuleExt *m = new FaceDetectModuleExt();

//	std::cout << "Running: " << argv[0] << " pwd=" << getenv("PWD") << std::endl;

	std::string exe = argv[0];
	size_t pos = exe.find_last_of('/');

	std::string path;
	if (pos == std::string::npos)
		path = "";
	else
		path = exe.substr(0, pos);

	std::cout << "exe=" << exe << " pos=" << pos << " path=" << path << std::endl;

	stringstream ss;
//	ss << getenv("PWD") << "/data/haarcascade_frontalface_alt.xml";
	ss << path << "/../../data/haarcascade_frontalface_alt.xml";
	std::string cascadeFilename = ss.str();
	ss.clear(); ss.str("");
	ss << path << "/../../data/haarcascade_eye_tree_eyeglasses.xml";
//	ss << getenv("PWD") << "/data/haarcascade_eye_tree_eyeglasses.xml";
	std::string nestedCascadeFilename = ss.str();
	ss.clear(); ss.str("");


	if (argc < 2) {
		std::cout << "Use an identifier as argument for this instance" << endl;
		return EXIT_FAILURE;
	}
	std::string identifier = argv[1];
	m->Init(identifier);

	m->loadCascade(cascadeFilename);
	m->loadNestedCascade(nestedCascadeFilename);

	do {
		m->Tick();
	} while (!m->Stop()); 

	delete m;

	return EXIT_SUCCESS;
}
