/**
 * @file MonotoneDecompositionModuleExt.h
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
 * Copyright © 2012 Anne van Rossum <anne@almende.com>
 *
 * @author  ...
 * @date    ...
 * @company Almende B.V.
 * @case    Artificial Intelligence Framework
 */

#include <MonotoneDecompositionModule.h>

#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Polygon_with_holes_2.h>

typedef CGAL::Exact_predicates_inexact_constructions_kernel Kernel;
typedef Kernel::FT                          Ft;
typedef Kernel::Point_2                     Point;
typedef Kernel::Segment_2                   Segment;
typedef Kernel::Direction_2                 Direction;
typedef Kernel::Line_2                      Line;
typedef Kernel::Vector_2                    Vector;
typedef CGAL::Polygon_2<Kernel>             Polygon;
typedef CGAL::Polygon_with_holes_2<Kernel>  PolygonWithHoles;


namespace rur {

class MonotoneDecompositionModuleExt: public MonotoneDecompositionModule {
public:
	void Init(std::string& name);

	// The tick function will be called from the MonotoneDecompositionModuleMain file
	void Tick();

	// As soon as Stop() returns "true", the MonotoneDecompositionModuleMain will stop the module
	bool Stop();
};

}

