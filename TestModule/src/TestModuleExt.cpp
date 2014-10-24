/**
 * @file TestModuleExt.cpp
 * @brief TestModule extension
 *
 * This file is created at "DoBots". It is open-source software and part of "AI". 
 * This software is published under the GNU General Lesser Public Licence license (GPLv3).
 *
 * Copyright © 2014 Bart van Vliet <bart@almende.org>
 *
 * @author                   Bart van Vliet
 * @date                     okt 21, 2014
 * @organisation             DoBots
 * @project                  AI
 */

#include <TestModuleExt.h>
#include <unistd.h> // To use usleep()

using namespace rur;

//! Replace with your own code
TestModuleExt::TestModuleExt() {

}

//! Replace with your own code
TestModuleExt::~TestModuleExt() {

}

//! Replace with your own code
void TestModuleExt::Tick() {
	//std::vector<float>* readVec;
	std::string* readString;
	readString = readParamDesc();
	if (readString != NULL) {
		writeParam(*readString + " lol");
		readString->clear();
	}
	usleep(1000);
}

//! Replace with your own code
bool TestModuleExt::Stop() {
	return false;
}

