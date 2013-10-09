/**
 * @file MonotoneSweepModuleExt.cpp
 * @brief Create a sweep path, given a sweep width and a monotone polygon w.r.t. given edge index.
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
 * Copyright © 2013 Bart van Vliet <bart@almende.org>
 *
 * @author	Bart van Vliet
 * @date	17-06-2013
 * @company	Almende B.V.
 * @project	Artificial Intelligence Framework
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


void MonotoneSweepModuleExt::Init(std::string &name) {
	MonotoneSweepModule::Init(name);

	mSweepWidth = 0.5;

	// Create an example polygon
	Polygon poly ;
	poly.push_back(Point(0,0));
	poly.push_back(Point(3,1));
	poly.push_back(Point(2,4));
	poly.push_back(Point(-2,5));
	poly.push_back(Point(0,2));
	std::cout << "Input polygon:" << std::endl;
	print_polygon(poly);
	std::cout << std::endl;
	int sweepDirectionIndex = 2;

	// We can create an offset of a given polygon, but this brings risk of a polygon being split in two.
	// It's better that the provided polygon already has an offset from the actual walls.
	Ft lOffset = mSweepWidth/2;
	PolygonPtrVector offsetPolygons = CGAL::create_interior_skeleton_and_offset_polygons_2(lOffset, poly);
//	PolygonPtrVector outer_offset_polygons = CGAL::create_exterior_skeleton_and_offset_polygons_2(lOffset,poly);
	std::cout << "Offset:" << std::endl;
	print_polygons(offsetPolygons);
	std::cout << std::endl;

	// Create the sweep polygon
	Polygon sweepPoly;
	//getSweepPoly(poly, sweepDirectionIndex, mSweepWidth, sweepPoly);
	getSweepPoly(*(offsetPolygons[0]), sweepDirectionIndex, mSweepWidth, sweepPoly);
	std::cout << "Sweep polygon:" << std::endl;
	print_polygon(sweepPoly);
	std::cout << std::endl;
}

Ft MonotoneSweepModuleExt::getMaxDistance(const Polygon& poly, const Segment& startSegment,
		const Ft& norm, const Direction& perpDir) {

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

Segment MonotoneSweepModuleExt::parallelSegment(const Polygon& poly, const Segment& startSegment, const Ft& offset,
			const Ft& norm, const Direction& perpDir) {

	// Calculate a point on the perpendicular vector at a distance of offset
	Point startPoint(startSegment.vertex(0));
	Point p(startPoint.x() + offset/norm*perpDir.dx(), startPoint.y() + offset/norm*perpDir.dy());
	std::cout << "Point: " << p << std::endl;

	// Place a line on that point in the direction of the startSegment
	//   so that that line is parallel to the startSegment and at a distance of offset
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
			assert(j < 2);
			if (j>0) {
				// If the intersection is exactly at a vertex of the polygon, it may be found twice
				if (intersectionPoints[0] != *iPoint)
					intersectionPoints[j++] = *iPoint;
			}
			else
				intersectionPoints[j++] = *iPoint;
		}
//		else if (const CGAL::Segment_2<Kernel> *iSeg = CGAL::object_cast<CGAL::Segment_2<Kernel> >(&result)) {
//			// Intersection with a segment: *iSeg.
//			std::cout << "Intersection at segment: " << *iSeg << std::endl;
//		}
//		else {
//			// No intersection
//		}
	}
	std::cout << "j=" << j << std::endl;
	return Segment(intersectionPoints[0], intersectionPoints[1]);
}

void MonotoneSweepModuleExt::getSweepPoly(const Polygon& poly, const int sweepDirectionIndex,
		const Ft& sweepWidth, Polygon &sweepPoly) {

	// First sweep the edges
	int polySize = poly.size();
	Polygon::Vertex_const_circulator vertexCit = poly.vertices_circulator();
	vertexCit += sweepDirectionIndex+1;
	for (int i=0; i<polySize+1; ++i, ++vertexCit)
		sweepPoly.push_back(*vertexCit);

	Segment startSegment = poly.edge(sweepDirectionIndex);
	Vector perpVec= startSegment.to_vector().perpendicular(CGAL::POSITIVE);
	Ft perpVecNorm = sqrt(perpVec.squared_length());

	// Maximum distance of startSegment to a vertex of the polygon
	Ft maxDist = getMaxDistance(poly, startSegment, perpVecNorm, perpVec.direction());

	// The number of sweeps, needed to cover the whole polygon
	std::cout << "max distance=" << maxDist << std::endl;
	int numSweeps = ceil(fabs(maxDist)/sweepWidth); // Number of sweep lines including the start edge
//	float newSweepWidth = fabs(maxDist)/numSweeps; // Distance between two sweep lines
//	std::cout << "numSweeps=" << numSweeps << " newSweepWidth=" << newSweepWidth << std::endl;

	// Create the sweep segments and add them to the sweep polygon
	Polygon::Vertex_const_iterator vertexIt;
	vertexIt = sweepPoly.vertices_end();
	vertexIt -= 2;
	Segment lastSweepSegment(*vertexIt++, *vertexIt);
	std::cout << "lastSweepSegment = " << lastSweepSegment << std::endl;
	for (int i=1; i<numSweeps; ++i) {

		// Get a segment parallel to startSegment at an offset of i* (maxDist / numSweeps)
		//   don't use the abs value, so that it goes in correct direction.
		Segment curSegment = parallelSegment(poly, startSegment, i*maxDist/numSweeps, perpVecNorm, perpVec.direction());

		std::cout << "lastSweepSegment.direction() = " << lastSweepSegment.direction() << std::endl;

		// If the direction is equal to the previous segment, then get a segment in the opposite direction
		if (lastSweepSegment.direction().dx() * curSegment.direction().dx() >= 0 &&
				lastSweepSegment.direction().dy() * curSegment.direction().dy() >= 0) {
			lastSweepSegment = curSegment.opposite();
		}
		else {
			lastSweepSegment = curSegment;
		}
		sweepPoly.push_back(Point(lastSweepSegment.vertex(0)));
		sweepPoly.push_back(Point(lastSweepSegment.vertex(1)));
		std::cout << "curSegment.direction() = " << curSegment.direction() << std::endl;
	}
}

//! Replace with your own functionality
void MonotoneSweepModuleExt::Tick() {
	writeLeftWheel(2);
	sleep(1);
}

//! Replace with your own functionality
bool MonotoneSweepModuleExt::Stop() {
	return false;
}

