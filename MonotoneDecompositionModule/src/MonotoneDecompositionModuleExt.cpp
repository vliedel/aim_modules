/**
 * @file MonotoneDecompositionModuleExt.cpp
 * @brief MonotoneDecompositionModule extension
 *
 * This file is created at Almende B.V. It is open-source software and part of the Common 
 * Hybrid Agent Platform (CHAP). A toolbox with a lot of open-source tools, ranging from 
 * thread pools and TCP/IP components to control architectures and learning algorithms. 
 * This software is published under the GNU Lesser General Public license (LGPL).
 *
 * It is not possible to add usage restrictions to an open-source license. Nevertheless,
 * we personally strongly object against this software being used by the military, in the
 * bio-industry, for animal experimentation, or anything that violates the Universal
 * Declaration of Human Rights.
 *
 * Copyright © 2013 Your Name <your@email>
 *
 * @author	Your Name
 * @date	Current date
 * @company	Your Company
 * @project	Specific Software Project
 */

#include <MonotoneDecompositionModuleExt.h>

#include "print_utils.h"

using namespace rur;


void MonotoneDecompositionModuleExt::Init(std::string &name) {
	MonotoneDecompositionModule::Init(name);

	// Create an example polygon
	Polygon poly;
	poly.push_back(Point(0,0));
	poly.push_back(Point(3,1));
	poly.push_back(Point(2,4));
	poly.push_back(Point(-2,5));
	poly.push_back(Point(0,2));
	std::cout << "Input polygon:" << std::endl;
	//print_polygon(poly);
	std::cout << std::endl;
	int sweepDirectionIndex = 2;

	Polygon hole;
	hole.push_back(Point(1,1));
	hole.push_back(Point(2,2));
	hole.push_back(Point(0,3));

	PolygonWithHoles polyHoles(poly);
	polyHoles.add_hole(hole);
	print_polygon_with_holes(polyHoles);
}

//! Replace with your own functionality
void MonotoneDecompositionModuleExt::Tick() {

}

//! Replace with your own functionality
bool MonotoneDecompositionModuleExt::Stop() {
	return false;
}

