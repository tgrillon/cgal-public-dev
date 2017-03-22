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

#define PROFILING_MODE

#include <boost/tuple/tuple.hpp>
#include <boost/random/linear_congruential.hpp>
#include <boost/random/uniform_smallint.hpp>
#include <boost/random/variate_generator.hpp>
#include <CGAL/point_generators_2.h>
#include <CGAL/Hyperbolic_random_points_in_disc_2.h>
#include <CGAL/Periodic_4_hyperbolic_Delaunay_triangulation_2.h>
#include <CGAL/Periodic_4_hyperbolic_Delaunay_triangulation_traits_2.h>
#include <CGAL/CORE_Expr.h>
#include <CGAL/Cartesian.h>
#include <CGAL/determinant.h>

#include <CGAL/Delaunay_triangulation_2.h>
#include <CGAL/Periodic_2_Delaunay_triangulation_traits_2.h>
#include <CGAL/Periodic_2_Delaunay_triangulation_2.h>

#include <CGAL/Timer.h>


typedef CORE::Expr                                                              NT;
typedef CGAL::Cartesian<NT>                                                     Kernel;
typedef CGAL::Periodic_4_hyperbolic_Delaunay_triangulation_traits_2<Kernel>     Traits;
typedef CGAL::Periodic_4_hyperbolic_Delaunay_triangulation_2<Traits>            Triangulation;
typedef Hyperbolic_octagon_translation_matrix<NT>                               Octagon_matrix;
typedef Kernel::Point_2                                                         Point;
typedef Triangulation::Vertex_handle                                            Vertex_handle;
typedef Traits::Side_of_fundamental_octagon                                     Side_of_fundamental_octagon;

typedef CGAL::Cartesian<double>::Point_2                                        Point_double;
typedef CGAL::Creator_uniform_2<double, Point_double >                          Creator;

typedef CGAL::Delaunay_triangulation_2<Kernel>                                  Euclidean_triangulation;
typedef CGAL::Periodic_2_Delaunay_triangulation_traits_2<Kernel>                Ptraits;
typedef CGAL::Periodic_2_Delaunay_triangulation_2<Ptraits>                      PEuclidean_triangulation;


long calls_apply_identity(0);
long calls_apply_non_identity(0);
long calls_append_identity(0);
long calls_append_non_identity(0);

int main(int argc, char** argv) {    

    if (argc < 2) {
        cout << "usage: " << argv[0] << " [number_of_points_to_insert] [optional: number_of_iterations]" << endl;
        return -1;
    }

    int N = atoi(argv[1]);
    int iters = 1;
    if (argc == 3) {
        iters = atoi(argv[2]);
    }

    Side_of_fundamental_octagon pred;

    
    cout << "---- for best results, make sure that you have compiled me in Release mode ----" << endl;
    
    double extime1 = 0.0;
    double extime3 = 0.0;

    for (int exec = 1; exec <= iters; exec++) {

        vector<Point_double> v;
        std::vector<Point> pts;

        Hyperbolic_random_points_in_disc_2_double(v, 5*N, -1, 0.159);

        int cnt = 0;
        int idx = 0;
        do {    
            Point pt = Point(v[idx].x(), v[idx].y());
            if (pred(pt) != CGAL::ON_UNBOUNDED_SIDE) {
                pts.push_back(pt);
                cnt++;
            } 
            idx++;
        } while (cnt < N && idx < v.size());

        if (cnt < N) {
            cout << "I failed to generate all the random points! Exiting..." << endl;
            return -1;
        }

        cout << "iteration " << exec << ": inserting into hyperbolic periodic triangulation...    "; cout.flush();
        Triangulation tr;
        tr.insert_dummy_points(true);  
        CGAL::Timer t1;
        t1.start();
        tr.insert(pts.begin(), pts.end());
        t1.stop();
        extime1 += t1.time();
        cout << "DONE! (# of vertices = " << tr.number_of_vertices() << ", time = " << t1.time() << " secs)" << endl;

        cout << "iteration " << exec << ": inserting into Euclidean non-periodic triangulation... "; cout.flush();
        Euclidean_triangulation etr;   
        CGAL::Timer t3;
        t3.start();
        etr.insert(pts.begin(), pts.end());
        t3.stop();
        extime3 += t3.time();
        cout << "DONE! (# of vertices = " << etr.number_of_vertices() << ", time = " << t3.time() << " secs)" << endl;

    }

    extime1 /= (double)iters;
    extime3 /= (double)iters;

    cout << "Hyperbolic periodic     triangulation: average time = " << extime1 << endl;
    cout << "Euclidean  non-periodic triangulation: average time = " << extime3 << endl;

    return 0;
}
