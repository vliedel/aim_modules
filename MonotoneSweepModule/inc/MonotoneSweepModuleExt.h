/**
 * @file MonotoneSweepModuleExt.h
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
 * @author  Bart van Vliet
 * @date    17-06-2013
 * @company Almende B.V.
 * @project Artificial Intelligence Framework
 */

#include <MonotoneSweepModule.h>
//#include <Eigen/Dense>
#include <vector>

#include <boost/shared_ptr.hpp>
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Polygon_2.h>

typedef CGAL::Exact_predicates_inexact_constructions_kernel Kernel;
typedef Kernel::FT                        Ft;
typedef Kernel::Point_2                   Point;
typedef Kernel::Segment_2                 Segment;
typedef Kernel::Direction_2               Direction;
typedef Kernel::Line_2                    Line;
typedef Kernel::Vector_2                  Vector;
typedef CGAL::Polygon_2<Kernel>           Polygon;

namespace rur {

class MonotoneSweepModuleExt: public MonotoneSweepModule {
public:
	// The tick function will be called from the MonotoneSweepModuleMain file
	void Tick();

	// As soon as Stop() returns "true", the MonotoneSweepModuleMain will stop the module
	bool Stop();

	void Init(std::string& name);
private:
	Polygon mPoly;
	Ft mSweepWidth;

	Ft getMaxDistance(const Polygon& poly, const Segment& startSegment,
			const Ft& norm, const Direction& perpendicularDir);

	Segment parallelSegment(const Polygon& poly, const Segment& startSegment, const Ft& offset,
			const Ft& norm, const Direction& perpendicularDir);

	void getSweepPoly(const Polygon& poly, const int sweepDirectionIndex, const Ft& sweepWidth, Polygon &sweepPoly);
};
}

