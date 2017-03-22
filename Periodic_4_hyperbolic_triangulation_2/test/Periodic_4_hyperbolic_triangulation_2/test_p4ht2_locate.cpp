// Copyright (c) 2016-2017 INRIA Nancy Grand-Est (France).
// All rights reserved.
//
// This file is part of CGAL (www.cgal.org).
// You can redistribute it and/or modify it under the terms of the GNU
// General Public License as published by the Free Software Foundation,
// either version 3 of the License, or (at your option) any later version.
//
// Licensees holding a valid commercial license may use this file in
// accordance with the commercial license agreement provided with the software.
//
// This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
// WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
//
// $URL$
// $Id$
//
// Author(s)     : Iordan Iordanov <iordan.iordanov@loria.fr>

#include <boost/tuple/tuple.hpp>
#include <boost/random/linear_congruential.hpp>
#include <boost/random/uniform_smallint.hpp>
#include <boost/random/variate_generator.hpp>

#include <CGAL/Periodic_4_hyperbolic_Delaunay_triangulation_2.h>
#include <CGAL/Periodic_4_hyperbolic_Delaunay_triangulation_traits_2.h>
#include <CGAL/Periodic_4_hyperbolic_triangulation_dummy_14.h>
#include <CGAL/Algebraic_kernel_for_circles_2_2.h>
#include <CGAL/Circular_kernel_2.h>
#include <CGAL/CORE_Expr.h>
#include <CGAL/Cartesian.h>
#include <CGAL/determinant.h>

using namespace CGAL;

typedef CORE::Expr                                                                  NT;
typedef CGAL::Cartesian<NT>                                                         Kernel;
typedef CGAL::Periodic_4_hyperbolic_Delaunay_triangulation_traits_2<Kernel>         Traits;
typedef CGAL::Periodic_4_hyperbolic_Delaunay_triangulation_2<Traits>                Triangulation;
typedef Triangulation::Face_handle                                                  Face_handle;
typedef Triangulation::Vertex_handle                                                Vertex_handle;
typedef Triangulation::Locate_type                                                  Locate_type;
typedef Triangulation::Offset                                                       Offset;
typedef Triangulation::Point                                                        Point;


std::ostream& operator<<(std::ostream& s, const Locate_type& lt) {
    switch(lt) {
        case Triangulation::VERTEX: s << "VERTEX";  break;
        case Triangulation::FACE:   s << "FACE";    break;
        case Triangulation::EDGE:   s << "EDGE";    break;
        default:                    s << "ZABABA";  break;
    }
    return s;
}


int main(void) {

    Triangulation tr;    
    tr.insert_dummy_points(true);

    assert(tr.is_valid());

    Locate_type lt;
    int li;
    Face_handle fh;

    cout << "---- locating dummy points (all should be vertices) ----" << endl;
    for (int j = 0; j < 14; j++) {
        Point query = tr.get_dummy_point(j); 
        fh = tr.locate(query, lt, li);
        assert(lt == Triangulation::VERTEX);
        cout << "   dummy point " << j << ": OK " << endl;
    }

    cout << "---- locating the midpoint of a Euclidean segment ----" << endl;
    Point p1 = tr.get_dummy_point(0), p2 = tr.get_dummy_point(1);
    Point query = midpoint(p1, p2);
    fh = tr.locate(query, lt, li);
    assert(lt == Triangulation::EDGE);
    cout << "   located as edge OK" << endl;

    cout << "---- inserting a single point and locating it ----" << endl;
    Vertex_handle v = tr.insert(Point(-0.4, -0.1));
    fh = tr.locate(v->point(), lt, li);
    assert(lt == Triangulation::VERTEX);
    cout << "   located as vertex OK" << endl;

    // TODO: add a test case for a circular edge!

    return 0;
}