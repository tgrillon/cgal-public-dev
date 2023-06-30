
#include "Geodesic_arcs.h"

#include <iostream>
#include <iterator>
#include <vector>

#include <qmath.h>
#include <qvector3d.h>

#include <CGAL/Exact_predicates_exact_constructions_kernel.h>
#include <CGAL/Arrangement_on_surface_2.h>
#include <CGAL/Arr_geodesic_arc_on_sphere_traits_2.h>
#include <CGAL/Arr_spherical_topology_traits_2.h>
#include <CGAL/Vector_3.h>

#include "arr_print.h"


using Kernel        = CGAL::Exact_predicates_exact_constructions_kernel;
using Geom_traits   = CGAL::Arr_geodesic_arc_on_sphere_traits_2<Kernel>;
using Point         = Geom_traits::Point_2;
using Curve         = Geom_traits::Curve_2;
using Topol_traits  = CGAL::Arr_spherical_topology_traits_2<Geom_traits>;
using Arrangement   = CGAL::Arrangement_on_surface_2<Geom_traits, Topol_traits>;


using Dir3 =  Kernel::Direction_3	;
std::ostream& operator << (std::ostream& os, const Dir3& d)
{
  os << d.dx() << ", " << d.dy() << ", " << d.dz();
  return os;
}

using Approximate_point_2 = Geom_traits::Approximate_point_2;
std::ostream& operator << (std::ostream& os, const Approximate_point_2& d)
{
  os << d.dx() << ", " << d.dy() << ", " << d.dz();
  return os;
}

using Approximate_number_type = Geom_traits::Approximate_number_type;
using Approximate_kernel      = Geom_traits::Approximate_kernel;
using Approximate_Vector_3    = CGAL::Vector_3<Approximate_kernel>;
using Approximate_Direction_3 = Approximate_kernel::Direction_3;
using Direction_3             = Kernel::Direction_3;


std::ostream& operator << (std::ostream& os, const Approximate_Vector_3& v)
{
  os << v.x() << ", " << v.y() << ", " << v.z();
  //os << v.hx() << ", " << v.hy() << ", " << v.hz() << ", " << v.hw();
  return os;
}


Geodesic_arcs::Approx_arcs Geodesic_arcs::get_approx_arcs(double error)
{
  // Construct the arrangement from 12 geodesic arcs.
  Geom_traits traits;
  Arrangement arr(&traits);

  auto ctr_p = traits.construct_point_2_object();
  auto ctr_cv = traits.construct_curve_2_object();


  std::vector<Curve>  xcvs;
  xcvs.push_back(ctr_cv(ctr_p(1, 0, 0), ctr_p(0, 1, 0)));
  xcvs.push_back(ctr_cv(ctr_p(1, 0, 0), ctr_p(0, 0, 1)));
  xcvs.push_back(ctr_cv(ctr_p(0, 1, 0), ctr_p(0, 0, 1)));
  //xcvs.push_back(ctr_cv(ctr_p(1, 0, 0), ctr_p(0, 1, 0), Dir3(0, 0, -1)));
  //xcvs.push_back(ctr_cv(Dir3(0, 0, -1)));

  auto approx = traits.approximate_2_object();


  std::vector<std::vector<QVector3D>>  arcs;
  for (const auto& xcv : xcvs)
  {
    std::vector<Approximate_point_2> v;
    auto oi2 = approx(xcv, error, std::back_insert_iterator(v));
    
    std::vector<QVector3D> arc_points;
    for (const auto& p : v)
    {
      const QVector3D arc_point(p.dx(), p.dy(), p.dz());
      arc_points.push_back(arc_point);
    }
    arcs.push_back(std::move(arc_points));
  }
  //std::cout << "offset count = " << m_arc_offsets.size() << std::endl;
  
  return arcs;
}


Geodesic_arcs::Approx_arcs Geodesic_arcs::get_approx_arcs(const Kml::Placemark&
                                                        placemark, double error)
{
  Geom_traits traits;
  auto ctr_p = traits.construct_point_2_object();
  auto ctr_cv = traits.construct_curve_2_object();

  std::vector<Curve>  xcvs;
  for (const auto& lring : placemark.polygons)
  {
    // convert the nodes to points on unit-sphere
    std::vector<Approximate_Vector_3> sphere_points;
    for (const auto& node : lring.nodes)
    {
      const auto p = node.get_coords_3d();
      Approximate_Vector_3 v(p.x, p.y, p.z);
      sphere_points.push_back(v);
    }

    // add curves
    int num_points = sphere_points.size();
    for (int i = 0; i < sphere_points.size(); i++)
    {
      const auto p1 = sphere_points[i];
      const auto p2 = sphere_points[(i + 1) % num_points];
      xcvs.push_back(ctr_cv(ctr_p(p1.x(), p1.y(), p1.z()),
        ctr_p(p2.x(), p2.y(), p2.z())));
    }
  }

  auto approx = traits.approximate_2_object();
  std::vector<std::vector<QVector3D>>  arcs;
  for (const auto& xcv : xcvs)
  {
    std::vector<Approximate_point_2> v;
    auto oi2 = approx(xcv, error, std::back_insert_iterator(v));

    std::vector<QVector3D> arc_points;
    for (const auto& p : v)
    {
      const QVector3D arc_point(p.dx(), p.dy(), p.dz());
      arc_points.push_back(arc_point);
    }
    arcs.push_back(std::move(arc_points));
  }
  //std::cout << "offset count = " << m_arc_offsets.size() << std::endl;

  return arcs;
}

Geodesic_arcs::Approx_arcs Geodesic_arcs::get_approx_arcs(
                                const Kml::Placemarks& placemarks, double error)
{
  Geom_traits traits;
  auto ctr_p = traits.construct_point_2_object();
  auto ctr_cv = traits.construct_curve_2_object();

  std::vector<Curve>  xcvs;
  for (const auto& pm : placemarks)
  {
    for (const auto& lring : pm.polygons)
    {
      // convert the nodes to points on unit-sphere
      std::vector<Approximate_Vector_3> sphere_points;
      for (const auto& node : lring.nodes)
      {
        const auto p = node.get_coords_3d();
        Approximate_Vector_3 v(p.x, p.y, p.z);
        sphere_points.push_back(v);
      }

      // add curves
      int num_points = sphere_points.size();
      for (int i = 0; i < sphere_points.size(); i++)
      {
        const auto p1 = sphere_points[i];
        const auto p2 = sphere_points[(i+1) % num_points];
        xcvs.push_back(ctr_cv(ctr_p(p1.x(), p1.y(), p1.z()), 
                              ctr_p(p2.x(), p2.y(), p2.z())));
      }
    }
  }

  auto approx = traits.approximate_2_object();
  std::vector<std::vector<QVector3D>>  arcs;
  for (const auto& xcv : xcvs)
  {
    std::vector<Approximate_point_2> v;
    auto oi2 = approx(xcv, error, std::back_insert_iterator(v));

    std::vector<QVector3D> arc_points;
    for (const auto& p : v)
    {
      const QVector3D arc_point(p.dx(), p.dy(), p.dz());
      arc_points.push_back(arc_point);
    }
    arcs.push_back(std::move(arc_points));
  }
  //std::cout << "offset count = " << m_arc_offsets.size() << std::endl;

  return arcs;
}