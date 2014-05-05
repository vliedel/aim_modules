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


MonotoneSweepModuleExt::MonotoneSweepModuleExt() {

}

MonotoneSweepModuleExt::~MonotoneSweepModuleExt() {

}

void MonotoneSweepModuleExt::Init(std::string &name) {
	MonotoneSweepModule::Init(name);

//	mOffsetWidth = 0.7/2;
	mOffsetWidth = 0.0;
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
	PolygonPtrVector offsetPolygons = CGAL::create_interior_skeleton_and_offset_polygons_2(mOffsetWidth, poly);
//	PolygonPtrVector outer_offset_polygons = CGAL::create_exterior_skeleton_and_offset_polygons_2(mOffsetWidth, poly);
	std::cout << "Offset:" << std::endl;
	print_polygons(offsetPolygons);
	std::cout << std::endl;

	// Create the sweep polygon
	Polygon sweepPoly;
	//getSweepPoly(poly, sweepDirectionIndex, mSweepWidth, sweepPoly);

	std::cout << "length = " << getLength(*(offsetPolygons[0])) << std::endl;


//	getSweepPoly(*(offsetPolygons[0]), sweepDirectionIndex, mSweepWidth, sweepPoly);
//	std::cout << "Sweep polygon:" << std::endl;
//	print_polygon(sweepPoly);
//	std::cout << std::endl;

	std::vector<Point> sweepPath;
	getSweepPoly(*(offsetPolygons[0]), sweepDirectionIndex, mSweepWidth, sweepPath);
//	std::vector<Point> sweepPath = getSweepPoly(*(offsetPolygons[0]), sweepDirectionIndex, mSweepWidth);
	std::cout << "Sweep path:" << std::endl;
	for (int i=0; i<sweepPath.size(); ++i) {
		std::cout << sweepPath[i] << std::endl;
	}

}

Ft MonotoneSweepModuleExt::getLength(const Polygon& poly) {
//	std::cout << "getLength" << std::endl;
	Ft length(0);

	Polygon::Edge_const_iterator edgeIt;
	for (edgeIt = poly.edges_begin(); edgeIt != poly.edges_end(); ++edgeIt) {
		length += CGAL::sqrt(edgeIt->squared_length());
//		std::cout << "edge len = " << CGAL::sqrt(edgeIt->squared_length()) << std::endl;
	}

	return length;
}

// Returns the signed distance between two points, along the given vector, which should be normalized.
Ft MonotoneSweepModuleExt::getDistance(const Point& start, const Point& end, const Vector& vec) {
	// Calculate the dot product
	return (end.x() - start.x()) * vec.x() + (end.y() - start.y()) * vec.y();
}

// Returns signed distance of the max abs distance between the given segment and a vertex on the given polygon.
// perpVec must be a normalized vector that is perpendicular to the segment.
Ft MonotoneSweepModuleExt::getMaxDistance(const Polygon& poly, const Segment& segment, const Vector& perpVec) {

	// Distance between line and point is equal to dot product between perpendicular vector and point
	// divided length of vector
	Point startPoint(segment.vertex(0));

	std::cout << "perpVec = " << perpVec << std::endl;
	std::cout << "startPoint=" << startPoint << std::endl;

	Ft maxDist(0);
	Ft maxDistAbs(0);
	Polygon::Vertex_const_iterator vertexIt;
	for (vertexIt = poly.vertices_begin(); vertexIt != poly.vertices_end(); ++vertexIt) {
		Ft d = getDistance(startPoint, *vertexIt, perpVec);
		std::cout << "distance = " << d << std::endl;
		if (fabs(d) > maxDistAbs) {
			maxDist = d;
			maxDistAbs = fabs(d);
		}
	}
	return maxDist;
}


// Returns a segment that is parallel to startSegment, at the given offset, within the given polygon
// perpVec must be a normalized vector that is perpendicular to startSegment.
Segment MonotoneSweepModuleExt::parallelSegment(const Polygon& poly, const Segment& startSegment, const Ft& offset, const Vector& perpVec) {

	// Calculate a point on the perpendicular vector at a distance of offset
	Point startPoint(startSegment.vertex(0));
	Point p(startPoint.x() + offset*perpVec.x(), startPoint.y() + offset*perpVec.y());
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

/*
void MonotoneSweepModuleExt::getSweepPoly(const Polygon& poly, const int sweepDirectionIndex,
		const Ft& sweepWidth, Polygon &sweepPoly) {

	// First sweep the edges
	int polySize = poly.size();
	Polygon::Vertex_const_circulator vertexCit = poly.vertices_circulator();
	vertexCit += sweepDirectionIndex+1;
	for (int i=0; i<polySize+1; ++i, ++vertexCit)
		sweepPoly.push_back(*vertexCit);

	Segment startSegment = poly.edge(sweepDirectionIndex);
	Vector perpVec = startSegment.to_vector().perpendicular(CGAL::POSITIVE);
	Ft perpVecNorm = sqrt(perpVec.squared_length());
	Vector perpVecNormalized = perpVec / perpVecNorm;

	std::cout << "perpVec=" << perpVec << std::endl;
	std::cout << "perpVecNormalized=" << perpVec << std::endl;
	std::cout << "perpVecDir=" << perpVec.direction() << std::endl;

	// Maximum distance of startSegment to a vertex of the polygon
	Ft maxDist = getMaxDistance(poly, startSegment, perpVecNormalized);

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
		Segment curSegment = parallelSegment(poly, startSegment, i*maxDist/numSweeps, perpVecNormalized);

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
*/

// Adds points to the sweep path to cover the given polygon
void MonotoneSweepModuleExt::getSweepPoly(const Polygon& poly, const int sweepDirectionIndex, const Ft& sweepWidth, std::vector<Point> &sweepPath) {

//	// First sweep the edges
//	// Start at one vertex of the sweepDirectionIndex edge
//	int polySize = poly.size();
//	Polygon::Vertex_const_circulator vertexCit = poly.vertices_circulator();
//	vertexCit += sweepDirectionIndex+1;
//	for (int i=0; i<polySize+1; ++i, ++vertexCit)
//		sweepPath.push_back(*vertexCit);

	// Determine which edge to use as sweep direction: the edge with minimal distance

	Segment startSegment;
	Ft minDist(FLT_MAX);
	Polygon::Edge_const_iterator edgeIt;
	for (edgeIt = poly.edges_begin(); edgeIt != poly.edges_end(); ++edgeIt) {


//		Segment startSegment = poly.edge(sweepDirectionIndex);
		Vector perpVec = edgeIt->to_vector().perpendicular(CGAL::POSITIVE);
		Ft perpVecNorm = sqrt(perpVec.squared_length());
		Vector perpVecNormalized = perpVec / perpVecNorm;

		// Maximum distance of the edge to a vertex of the polygon
		Ft maxDist = getMaxDistance(poly, *edgeIt, perpVecNormalized);
		std::cout << "maxDist=" << maxDist << std::endl;

		if (maxDist < minDist) {
			minDist = maxDist;
			startSegment = *edgeIt;
		}
	}

	Ft maxDist = minDist;
	Vector perpVec = startSegment.to_vector().perpendicular(CGAL::POSITIVE);
	Ft perpVecNorm = sqrt(perpVec.squared_length());
	Vector perpVecNormalized = perpVec / perpVecNorm;

	// The number of sweeps, needed to cover the whole polygon
	std::cout << "max distance=" << maxDist << std::endl;
	int numSweeps = ceil(fabs(maxDist)/sweepWidth); // Number of sweep lines including the start edge
	std::cout << "numSweeps=" << numSweeps << std::endl;

	// Create the sweep segments and add them to the sweep path
	Segment lastSweepSegment = startSegment;
	std::cout << "lastSweepSegment = " << lastSweepSegment << std::endl;
	for (int i=1; i<numSweeps; ++i) {

		// Get a segment parallel to startSegment at an offset of i* (maxDist / numSweeps)
		//   don't use the abs value, so that it goes in correct direction.
		Segment curSegment = parallelSegment(poly, startSegment, i*maxDist/numSweeps, perpVecNormalized);

		std::cout << "lastSweepSegment.direction() = " << lastSweepSegment.direction() << std::endl;

		// If the direction is equal to the previous segment, then get a segment in the opposite direction
		if (lastSweepSegment.direction().dx() * curSegment.direction().dx() >= 0 &&
				lastSweepSegment.direction().dy() * curSegment.direction().dy() >= 0) {
			lastSweepSegment = curSegment.opposite();
		}
		else {
			lastSweepSegment = curSegment;
		}
		sweepPath.push_back(Point(lastSweepSegment.vertex(0)));
		sweepPath.push_back(Point(lastSweepSegment.vertex(1)));
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

