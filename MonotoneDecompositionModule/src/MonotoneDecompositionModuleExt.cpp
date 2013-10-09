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


#include <CGAL/Simple_cartesian.h>
#include <CGAL/Arr_segment_traits_2.h>
#include <CGAL/Arrangement_2.h>
#include "arr_print.h"

typedef float                                         Number_type;
typedef CGAL::Simple_cartesian<Number_type>           Kernel2;
typedef CGAL::Arr_segment_traits_2<Kernel2>           Traits_2;
typedef Traits_2::Point_2                             Point_2;
typedef Traits_2::X_monotone_curve_2                  Segment_2;
typedef CGAL::Arrangement_2<Traits_2>                 Arrangement_2;
typedef Arrangement_2::Vertex_handle                  Vertex_handle;
typedef Arrangement_2::Halfedge_handle                Halfedge_handle;



//#include <CGAL/Cartesian.h>
//#include <CGAL/MP_Float.h>
//#include <CGAL/Quotient.h>
//#include <CGAL/Arr_segment_traits_2.h>
//#include <CGAL/Arrangement_2.h>

//typedef CGAL::Quotient<CGAL::MP_Float>     Number_type;
//typedef CGAL::Cartesian<Number_type>       Kernel;
//typedef CGAL::Arr_segment_traits_2<Kernel> Traits_2;
//typedef Traits_2::Point_2                  Point_2;
//typedef Traits_2::X_monotone_curve_2       Segment_2;
//typedef CGAL::Arrangement_2<Traits_2>      Arrangement_2;


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
	Polygon outerPoly = polyHoles.outer_boundary();

	Arrangement_2 arr;
//	Arrangement_2 arr2;

	std::vector<Segment_2> segments;
	segments.push_back(Segment_2(Point_2(0,0), Point_2(10,0)));  // 0
	segments.push_back(Segment_2(Point_2(10,0), Point_2(10,5))); // 1
	segments.push_back(Segment_2(Point_2(10,5), Point_2(0,5)));  // 2
	segments.push_back(Segment_2(Point_2(0,5), Point_2(0,0)));   // 3
	CGAL::insert(arr, segments.begin(), segments.end());

//	Halfedge_handle e1 = arr2.insert_in_face_interior(segments[0], arr2.unbounded_face());
//
//	Vertex_handle   v1 = e1->source();
//	Vertex_handle   v2 = e1->target();
//
//	Halfedge_handle e2 = arr2.insert_from_left_vertex(segments[1], v2);
//
//	Vertex_handle   v3 = e2->target();
//	Halfedge_handle e3 = arr2.insert_from_right_vertex(segments[2], v3);
//
//	Vertex_handle   v4 = e3->target();
//	arr2.insert_at_vertices(segments[3], v4, v1);


	std::vector<Segment_2> segmentsHole;
	segmentsHole.push_back(Segment_2(Point_2(2,2), Point_2(4,2)));
	segmentsHole.push_back(Segment_2(Point_2(4,2), Point_2(4,3)));
	segmentsHole.push_back(Segment_2(Point_2(4,3), Point_2(2,3)));
	segmentsHole.push_back(Segment_2(Point_2(2,3), Point_2(2,2)));
	CGAL::insert(arr, segmentsHole.begin(), segmentsHole.end());

	std::vector<Segment_2> segmentsHole2;
	segmentsHole2.push_back(Segment_2(Point_2(6,2), Point_2(8,2)));
	segmentsHole2.push_back(Segment_2(Point_2(8,2), Point_2(8,3)));
	segmentsHole2.push_back(Segment_2(Point_2(8,3), Point_2(6,3)));
	segmentsHole2.push_back(Segment_2(Point_2(6,3), Point_2(6,2)));
	CGAL::insert(arr, segmentsHole2.begin(), segmentsHole2.end());

	Arrangement_2::Face_const_iterator faceIt;
	for (faceIt = arr.faces_begin(); faceIt != arr.faces_end(); ++faceIt) {
		if (faceIt->is_unbounded())
			continue;
		Arrangement_2::Hole_const_iterator  holeIt;
		for (holeIt = faceIt->holes_begin(); holeIt != faceIt->holes_end(); ++holeIt) {
			Arrangement_2::Ccb_halfedge_const_circulator  curr = *holeIt;
			Arrangement_2::Halfedge_const_handle he;
			do {
				he = curr;
				std::cout << he->curve() << std::endl;
//				he->curve().line();
				std::cout << he->curve().line() << std::endl;

				// Check other faces for intersections with this line
				Arrangement_2::Face_const_iterator faceIt2;
				for (faceIt2 = arr.faces_begin(); faceIt2 != arr.faces_end(); ++faceIt2) {

				}


				++curr;
			} while (curr != *holeIt);
		}
	}

//	Segment_2       s1(Point_2(1, 3), Point_2(3, 5));
//	Segment_2       s2(Point_2(3, 5), Point_2(5, 3));
//	Segment_2       s3(Point_2(5, 3), Point_2(3, 1));
//	Segment_2       s4(Point_2(3, 1), Point_2(1, 3));
//	Segment_2       s5(Point_2(1, 3), Point_2(5, 3));
//
//	Halfedge_handle e1 = arr.insert_in_face_interior(s1, arr.unbounded_face());
//	Vertex_handle   v1 = e1->source();
//	Vertex_handle   v2 = e1->target();
//	Halfedge_handle e2 = arr.insert_from_left_vertex(s2, v2);
//	Vertex_handle   v3 = e2->target();
//	Halfedge_handle e3 = arr.insert_from_right_vertex(s3, v3);
//	Vertex_handle   v4 = e3->target();
//	arr.insert_at_vertices(s4, v4, v1);
//	arr.insert_at_vertices(s5, v1, v3);

	print_arrangement(arr);

//	std::cout << "-- Arr 2 --" << std::endl;
//	print_arrangement(arr2);

	return;




	Polygon::Edge_const_iterator eIt;

//	// Minimum sum of altitudes is in the direction of an edge of the outer polygon or of a hole.
//	for (eIt = outerPoly.edges_begin(); eIt != outerPoly.edges_end(); ++eIt) {
//		Vector perpVec= eIt->to_vector().perpendicular(CGAL::POSITIVE);
//		Ft norm = sqrt(perpVec.squared_length());
//		Direction perpDir = perpVec.direction();
//
//		// closest vertex on outer polygon is the open event
//		// farthest vertex on outer polygon is the close event
//		// closest vertex on any hole polygon is a split event
//		// farthest vertext on any hole polygon is a merge event
//
//		// Since we're only looking for the min and max, we can calculate the distance to any point
//		Point startPoint(0,0);
//		float max = FLT_MIN;
//
//		float min = FLT_MAX;
//
//		Polygon::Vertex_const_iterator vIt;
//		for (vIt = outerPoly.vertices_begin(); vIt != outerPoly.vertices_end(); ++vIt) {
//			float d = (((*vIt)[0] - startPoint.x()) * perpDir.dx() + ((*vIt)[1] - startPoint.y()) * perpDir.dy()) / norm;
//			if (d > max) {
//				max = d;
//			}
//			if (d < min) {
//				min = d;
//			}
//		}
//
//
//	}

	float minDist[2];
	minDist[0] = 0;
	minDist[1] = 0;
	Polygon::Edge_const_iterator minDistEdgeIt[2];
	Point minDistPoint[2];


	PolygonWithHoles::Hole_const_iterator hIt;
	for (hIt = polyHoles.holes_begin(); hIt != polyHoles.holes_end(); ++hIt) {
		for (eIt = hIt->edges_begin(); eIt != hIt->edges_end(); ++eIt) {

//			std::cout << "eIt = " << *eIt << std::endl;

			// Test edges of all other polygons for intersections
			Polygon::Edge_const_iterator eIntersecIt;
			for (eIntersecIt = outerPoly.edges_begin(); eIntersecIt != outerPoly.edges_end(); ++eIntersecIt) {

//				std::cout << "eIntersectionIt = " << *eIntersectionIt << std::endl;
				Line line = eIt->supporting_line();

				CGAL::Object result = CGAL::intersection(*eIntersecIt, line);
				if (const CGAL::Point_2<Kernel> *iPoint = CGAL::object_cast<CGAL::Point_2<Kernel> >(&result)) {
					// Intersection with a point: *ipoint.
					std::cout << "Intersection at point: " << *iPoint << std::endl;

					float d0 = CGAL::squared_distance(eIt->point(0),*iPoint);
					float d1 = CGAL::squared_distance(eIt->point(1),*iPoint);

					std::cout << "d0=" << d0 << " d1=" << d1 << std::endl;

					if (d0 < d1) {
						// intersection at the side of eIt->point(0)
						if (d0 < minDist[0]) {
							minDist[0] = d0;
							minDistEdgeIt[0] = eIntersecIt;
							minDistPoint[0] = *iPoint;
						}

					}
					else {
						// intersection at the side of eIt->point(1)
						if (d1 < minDist[1]) {
							minDist[1] = d1;
							minDistEdgeIt[1] = eIntersecIt;
							minDistPoint[1] = *iPoint;
						}
					}

				}
//				else if (const CGAL::Segment_2<Kernel> *iSeg = CGAL::object_cast<CGAL::Segment_2<Kernel> >(&result)) {
//					// Intersection with a segment: *iSeg.
//					std::cout << "Intersection at segment: " << *iSeg << std::endl;
//				}
//				else {
//					// No intersection
//				}
			}
			if (minDist[0] > 0) {

			}

		}
	}


}

//! Replace with your own functionality
void MonotoneDecompositionModuleExt::Tick() {

}

//! Replace with your own functionality
bool MonotoneDecompositionModuleExt::Stop() {
	return false;
}

//float MonotoneDecompositionModuleExt::altitude(const Polygon& poly, const Segment& startSegment, Point* point) {
float MonotoneDecompositionModuleExt::altitude(const Polygon& poly, const Segment& startSegment) {
	//Segment startSegment = poly.edge(sweepDirectionIndex);
	Vector perpVec= startSegment.to_vector().perpendicular(CGAL::POSITIVE);
	Ft norm = sqrt(perpVec.squared_length());
	Direction perpDir = perpVec.direction();

	// Distance between line and point is equal to dot product between perpendicular vector and point
	// divided length of vector
	Point startPoint(startSegment.vertex(0));

	std::cout << "perpendicularDir = " << perpDir << std::endl;
	std::cout << "startPoint=" << startPoint << std::endl;

	float maxDist(0);
	float maxDistAbs(0);
//	Polygon::Vertex_const_iterator vIt, vMaxDistIt;
	Polygon::Vertex_const_iterator vIt;
	for (vIt = poly.vertices_begin(); vIt != poly.vertices_end(); ++vIt) {
		// Calculate the dot product between point and perpendicular vector and divide by norm
		float d = (((*vIt)[0] - startPoint.x()) * perpDir.dx() + ((*vIt)[1] - startPoint.y()) * perpDir.dy()) / norm;
		std::cout << "distance = " << d << std::endl;
		if (fabs(d) > maxDistAbs) {
			maxDist = d;
			maxDistAbs = fabs(d);
//			vMaxDistIt = vIt;
		}
	}
//	if (point != NULL)
//		*point = *vMaxDistIt;
	return maxDist;
}
