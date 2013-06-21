/**
 * @file MonotoneSweepModuleExt.cpp
 * @brief MonotoneSweepModule extension
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

#include <MonotoneSweepModuleExt.h>
#include <iostream>

#include <CGAL/create_offset_polygons_2.h>
#include "print.h"

using namespace rur;
//using namespace Eigen;


typedef CGAL::Straight_skeleton_2<Kernel>  Ss;
typedef boost::shared_ptr<Polygon>         PolygonPtr;
typedef boost::shared_ptr<Ss>              SsPtr;
typedef std::vector<PolygonPtr>            PolygonPtrVector;

// Given a monotone polygon and an edge for sweep direction: create sweep path
// Parameters: sweep width

void MonotoneSweepModuleExt::Init(std::string &name) {
	MonotoneSweepModule::Init(name);

	mSweepWidth = 0.5;

	Polygon poly ;

	poly.push_back(Point(0,0));
	poly.push_back(Point(3,1));
	poly.push_back(Point(2,4));
	poly.push_back(Point(-2,5));
	poly.push_back(Point(0,2));
	std::cout << "Input polygon:" << std::endl;
	print_polygon(poly);
	std::cout << std::endl;

	Ft lOffset = mSweepWidth/2;
	PolygonPtrVector offsetPolygons = CGAL::create_interior_skeleton_and_offset_polygons_2(lOffset, poly);
//	PolygonPtrVector outer_offset_polygons = CGAL::create_exterior_skeleton_and_offset_polygons_2(lOffset,poly);
	std::cout << "Offset:" << std::endl;
	print_polygons(offsetPolygons);
	std::cout << std::endl;

	int sweepDirectionIndex = 2;
	int polySize = poly.size();
	Polygon sweepPoly;

	// First sweep the edges
//	for (int i=0; i<polySize+1; ++i) {
//		sweepPoly.push_back(poly[(i+sweepDirectionIndex) % polySize]);
	Polygon::Vertex_const_circulator vertexCit = poly.vertices_circulator();
	vertexCit += sweepDirectionIndex+1;
	for (int i=0; i<polySize+1; ++i, ++vertexCit) {
		sweepPoly.push_back(*vertexCit);
	}

	// Then sweep the inside
	//sweepPoly.push_back((*offsetPolygons.at(0))[sweepDirectionIndex]);

	Segment startSegment = poly.edge(sweepDirectionIndex);
	Vector perpVec= startSegment.to_vector().perpendicular(CGAL::POSITIVE);
	Ft perpVecNorm = sqrt(perpVec.squared_length());

//	// Distance between line and point is equal to dot product between perpendicular vector and point
//	// divided length of vector
//
//	Point startPoint(startSegment.vertex(0));
//	// Vector perpendicular to start segment, translated such that first vertex of the segment is 0,0
//	Direction perpendicularDir = startSegment.direction().perpendicular(CGAL::POSITIVE);
//	// Norm of the vector
//	Ft norm = sqrt(startSegment.squared_length());
//
//
//	Ft maxDist(0);
//	Ft maxDistAbs(0);
//	Polygon::Vertex_const_iterator vertexIt;
//	for (vertexIt = poly.vertices_begin(); vertexIt != poly.vertices_end(); ++vertexIt) {
//		// Calculate the dot product between point and perpendicular vector and divide by norm
//		float d = (((*vertexIt)[0] - startPoint.x()) * perpendicularDir.dx() +
//				((*vertexIt)[1] - startPoint.y()) * perpendicularDir.dy()) / norm;
//		std::cout << "distance = " << d << std::endl;
//		if (fabs(d) > maxDistAbs) {
//			maxDist = d;
//			maxDistAbs = fabs(d);
//		}
//	}
	Ft maxDist = getMaxDistance(poly, startSegment, perpVecNorm, perpVec.direction());

	std::cout << "max distance=" << maxDist << std::endl;
	int numSweeps = ceil(fabs(maxDist)/mSweepWidth); // Number of sweep lines including the start edge
	float sweepWidth = fabs(maxDist)/numSweeps; // Distance between two sweep lines
	std::cout << "numSweeps=" << numSweeps << " sweepWidth=" << sweepWidth << std::endl;


	Point startPoint(startSegment.vertex(0));

	Polygon::Vertex_const_iterator vertexIt;
	vertexIt = sweepPoly.vertices_end();
	vertexIt -= 2;
	Segment lastSweepSegment(*vertexIt++, *vertexIt);
	std::cout << "lastSweepSegment = " << lastSweepSegment << std::endl;
	for (int i=1; i<numSweeps; ++i) {

		// Calculate a point on the perpendicular line at distance i*sweepWidth
		//   don't use the abs value, so that it goes in correct direction.
		Point p(startPoint.x() + maxDist/numSweeps/perpVecNorm*perpVec.direction().dx()*i, startPoint.y() +
				maxDist/numSweeps/perpVecNorm*perpVec.direction().dy()*i);
		std::cout << "Point: " << p << std::endl;

		// Place a line on that point in the direction of the startSegment
		//   so that that line is parallel to the startSegment at an offset of i*sweepWidth
		Line parallelLine(p, startSegment.direction());
		std::cout << "Line: " << parallelLine << std::endl;

		// Find out where the parallel line intersects the polygon
		// We will just check for intersections for each edge
		Point intersectionPoints[2];
		int j=0;
		Polygon::Edge_const_iterator edgeIt;
		for (edgeIt = poly.edges_begin(); edgeIt != poly.edges_end(); edgeIt++) {

			//std::cout << "Checking for intersections with edge: " << *edgeIt << std::endl;

			CGAL::Object result = CGAL::intersection(*edgeIt, parallelLine);
			if (const CGAL::Point_2<Kernel> *iPoint = CGAL::object_cast<CGAL::Point_2<Kernel> >(&result)) {
				// Intersection with a point: *ipoint.
				std::cout << "Intersection at point: " << *iPoint << std::endl;
				// TODO: check if j < 2
				if (j>0) {
					// If the intersection is exactly at a vertex of the polygon, it may be found twice
					if (intersectionPoints[0] != *iPoint)
						intersectionPoints[j++] = *iPoint;
				}
				else
					intersectionPoints[j++] = *iPoint;
			}
//			else if (const CGAL::Segment_2<Kernel> *iSeg = CGAL::object_cast<CGAL::Segment_2<Kernel> >(&result)) {
//				// Intersection with a segment: *iSeg.
//				std::cout << "Intersection at segment: " << *iSeg << std::endl;
//			}
//			else {
//				// No intersection
//			}

		}
		std::cout << "j=" << j << std::endl;

		// The new sweep segment should go in the opposite direction of the previous sweep segment
		std::cout << "lastSweepSegment.direction() = " << lastSweepSegment.direction() << std::endl;
		Segment curSegment(intersectionPoints[0], intersectionPoints[1]);
		//if (curSegment.direction() == lastSweepSegment.direction()) {
		if (lastSweepSegment.direction().dx() * curSegment.direction().dx() >= 0 &&
				lastSweepSegment.direction().dy() * curSegment.direction().dy() >= 0) {
			sweepPoly.push_back(intersectionPoints[1]);
			sweepPoly.push_back(intersectionPoints[0]);
			lastSweepSegment = Segment(intersectionPoints[1], intersectionPoints[0]);
		}
		else {
			sweepPoly.push_back(intersectionPoints[0]);
			sweepPoly.push_back(intersectionPoints[1]);
			lastSweepSegment = Segment(intersectionPoints[0], intersectionPoints[1]);
		}
		std::cout << "curSegment.direction() = " << curSegment.direction() << std::endl;
	}

	std::cout << "Sweep polygon:" << std::endl;
	print_polygon(sweepPoly);
	std::cout << std::endl;
}

Ft MonotoneSweepModuleExt::getMaxDistance(Polygon& poly, Segment& startSegment, Ft norm, const Direction& perpDir) {

	// Distance between line and point is equal to dot product between perpendicular vector and point
	// divided length of vector
	Point startPoint(startSegment.vertex(0));


	std::cout << "perpendicularDir = " << perpDir << std::endl;
	std::cout << "startPoint=" << startPoint << std::endl;

	Ft maxDist(0);
	Ft maxDistAbs(0);
	Polygon::Vertex_const_iterator vertexIt;
	for (vertexIt = poly.vertices_begin(); vertexIt != poly.vertices_end(); ++vertexIt) {
		// Calculate the dot product between point and perpendicular vector and divide by norm
		float d = (((*vertexIt)[0] - startPoint.x()) * perpDir.dx() +
				((*vertexIt)[1] - startPoint.y()) * perpDir.dy()) / norm;
		std::cout << "distance = " << d << std::endl;
		if (fabs(d) > maxDistAbs) {
			maxDist = d;
			maxDistAbs = fabs(d);
		}
	}
	return maxDist;
}

//! Replace with your own functionality
void MonotoneSweepModuleExt::Tick() {
	sleep(1);
}

//! Replace with your own functionality
bool MonotoneSweepModuleExt::Stop() {
	return false;
}

