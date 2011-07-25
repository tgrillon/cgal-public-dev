// Copyright (c) 2011  Tel-Aviv University (Israel).
// All rights reserved.
//
// This file is part of CGAL (www.cgal.org); you may redistribute it under
// the terms of the Q Public License version 1.0.
// See the file LICENSE.QPL distributed with CGAL.
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
//
// Author(s): Asaf Porat          <asafpor@post.tau.ac.il>
//            Efi Fogel           <efif@post.tau.ac.il>

#ifndef CGAL_RECTANGULAR_HYPERBOLA_WITH_ISO_ASYMPTOTES_2_H
#define CGAL_RECTANGULAR_HYPERBOLA_WITH_ISO_ASYMPTOTES_2_H

/*! \file
 * A header file for the class templates that represents a general and an
 * arbitrary rectangular hyperbola with vertical and horizontal asymptotes.
 */

#include <CGAL/basic.h>
#include <CGAL/Handle_for.h>
#include <CGAL/Arr_geometry_traits/Sqrt_extension_point_2.h>

namespace CGAL {

// Forward declaration:
template <typename NumberType_, bool Filter_>
class Rectangular_hyperbola_with_iso_asymptotes_2;

/*!
 * \class
 * A representation of a rectangular hyperbola with vertical and horizontal
 * asymptotes.
 */
template <typename Kernel_, bool Filter_>
class Rectangular_hyperbola_with_iso_asymptotes_2_rep {
public:
  typedef Kernel_                                      Kernel;
  typedef Filter_                                      Filter;

  typedef typename Kernel::NT                          NT;
  typedef Sqrt_extension_point_2<NT, Filter>           Point_2;

  friend class Rectangular_hyperbola_with_iso_asymptotes_2<Kernel, Filter>
  
private:
  typedef Rectangular_hyperbola_with_iso_asymptotes_2_rep<Kernel, Filter>
                                                        Self;
  
  /* axy + bx + cy + d = 0 */
  NT m_a, m_b, m_c, m_d;
  
  Point_2   m_source;        // The source point (if exists).
  Point_2   m_target;        // The target point (if exists).
  bool      m_has_source;    // Is the source point valid?
  bool      m_has_target;    // Is the target point valid?
  bool      m_is_directed_right; // Is the curve directed right?
  bool      m_has_left_x;    // Has the curve a vertical asymptote on the left 
  bool      m_has_right_x;   // Has the curve a vertical asymptote on the right 
  bool      m_is_continuous; // Is the curve continuous?

public:
  /*!
   * Default constructor.
   */
  Rectangular_hyperbola_with_iso_asymptotes_2_rep() :
    has_source(false),
    has_target(false)
  {}

  /*!
   * Constructor from all data fields.
   * \param a The a coefficient.
   * \param b The a coefficient.
   * \param c The a coefficient.
   * \param d The a coefficient.
   * \param sourse The source point.
   * \param target The target point.
   * \param has_source Indicates whether the curve has a valid source point.
   * \param has_source Indicates whether the curve has a valid target point.
   * \param is_directed_right Indicates whether the curve is directed right.
   * \param has_left_x Indicates whether the curve has a vertical asymptote
   *        on the left.
   * \param has_right_x Indicates whether the curve has a vertical asymptote
   *        on the right.
   * \param is_continuous Indicates the curve continuous.
   * \pre The two points must not be the same.
   * \pre The source is on the hyperbola.
   * \pre The target is on the hyperbola.
   */
  Rectangular_hyperbola_with_iso_asymptotes_2(const FT& a, const FT& b,
                                              const FT& c, const FT& d,
                                              const Point_2& source,
                                              const Point_2& target,
                                              bool has_source, bool has_target,
                                              bool is_directed_right,
                                              bool has_left_x, bool has_right_x,
                                              bool is_continuous) :
    m_a(a),
    m_b(b),
    m_c(c),
    m_d(d),
    m_source(source),
    m_target(target),
    m_has_source(has_source),
    m_has_target(has_target),
    m_is_directed_right(is_directed_right),
    m_has_left_x(has_left_x),
    m_has_right_x(has_right_x),
    m_is_continuous(is_continuous)
  {}

  /*!
   * Obtains the 'a' coefficient of the hyperbola.
   * \return The 'a' coefficient.
   */
  const NT& a() const { return m_a; }

  /*!
   * Obtains the 'b' coefficient of the hyperbola.
   * \return The 'b' coefficient.
   */
  const NT& b() const { return m_b; }

  /*!
   * Obtains the 'c' coefficient of the hyperbola.
   * \return The 'c' coefficient.
   */
  const NT& c() const { return m_c; }

  /*!
   * Obtains the 'd' coefficient of the hyperbola.
   * \return The 'd' coefficient.
   */
  const NT& d() const { return m_d; }

  /*!
   * Obtains the source point.
   * \return The source point.
   */
  Point_2& source() const { return m_source; }

  /*!
   * Obtains the target point.
   * \return The target point.
   */
  Point_2& target() const { return m_target; }

  /*!
   * Checks whether the curve has a source point.
   * \return If the curve has a source point, true; otherwise false.
   */
  bool has_source() const { return m_has_source; }

  /*!
   * Checks whether the curve has a target point.
   * \return If the curve has a target point, true; otherwise false.
   */
  bool has_target() const { return m_has_target; }

  /*!
   * Indicates whether the curve is directed right.
   * \return If the curve is directed right, true; otherwise false.
   */
  bool is_directed_right() const { return m_is_directed_right; }

  /*!
   * Indicates whether the curve has an asymptote on the left.
   * \return If the curve has an asymptote on the left, true; otherwise false.
   */
  bool has_left_x() const { return m_has_left_x; }

  /*!
   * Indicates whether the curve has an asymptote on the right.
   * \return If the curve has an asymptote on the right, true; otherwise false.
   */
  bool has_right_x() const { return m_has_right_x; }

  /*!
   * Indicates whether the curve is continuous.
   * \return If the curve is continuous, true; otherwise false.
   */
  bool is_continuous() const { return m_is_continuous; }
};

/*!
 * \class
 * A representation of a rectangular hyperbola with vertical and horizontal
 * asymptotes used by the traits class
 * Arr_rectangular_hyperbola_with_iso_asymptotes_traits_2.
 */
template <typename Kernel_, bool Filter_>
class Rectangular_hyperbola_with_iso_asymptotes_2 :
  public Handle_for<Rectangular_hyperbola_with_iso_asymptotes_2_rep<Kernel_,
                                                                    Filter_> >
{
public:
  typedef Kernel_                                       Kernel;
  typedef Filter_                                       Filter;

private:  
  typedef Rectangular_hyperbola_with_iso_asymptotes_2<Kernel, Filter>
                                                        Self;
  typedef Rectangular_hyperbola_with_iso_asymptotes_2_rep<NT, Filter>
                                                        Curve_rep;
  typedef Handle_for<Curve_rep>
                                                        Curve_handle;

public:
  typedef typename Kernel::FT                           NT;
  typedef Sqrt_extension_point_2<NT, Filter>            Point_2;
  typedef typename Curve_rep::Coord_NT                  Coord_NT;

  /*!
   * Default constructor.
   */
  Rectangular_hyperbola_with_iso_asymptotes_2() : Curve_handle(Curve_rep()) {}

  /*! Copy constructor. */
  Rectangular_hyperbola_with_iso_asymptotes_2(const Self& cv) :
    Curve_handle(cv) {}
  
  /*!
   * Constructor from all data fields.
   * \param a The a coefficient.
   * \param b The a coefficient.
   * \param c The a coefficient.
   * \param d The a coefficient.
   * \param sourse The source point.
   * \param target The target point.
   * \param has_source Indicates whether the curve has a valid source point.
   * \param has_source Indicates whether the curve has a valid target point.
   * \param is_directed_right Indicates whether the curve is directed right.
   * \param has_left_x Indicates whether the curve has a vertical asymptote
   *        on the left.
   * \param has_right_x Indicates whether the curve has a vertical asymptote
   *        on the right.
   * \param is_continuous Indicates the curve continuous.
   * \pre The two points must not be the same.
   * \pre The source is on the hyperbola.
   * \pre The target is on the hyperbola.
   */
  Rectangular_hyperbola_with_iso_asymptotes_2(const FT& a, const FT& b,
                                              const FT& c, const FT& d,
                                              const Point_2& source,
                                              const Point_2& target,
                                              bool has_source, bool has_target,
                                              bool is_directed_right,
                                              bool has_left_x, bool has_right_x,
                                              bool is_continuous) :
    Curve_handle(a, b, c, d, source, target, has_source, has_target,
                 is_directed_right, has_left_x, has_right_x, is_continuous)
  {}

  /*!
   * Assignment operator.
   */
  Self& operator=(const Self& cv)
  {
    if (this == &cv || this->identical(cv)) return *this;
    Curve_handle::operator=(cv);
    return *this;
  }

private:
  // Obtain the representation.
  inline const Curve_rep& rep() const { return (*(this->ptr())); }

  inline Curve_rep& rep() { return (*(this->ptr())); }

public:
  /*!
   * Checks whether the object is a segment.
   */
  bool is_segment() const
  { return (is_zero(rep().a()) && rep().has_target() && rep().has_source()); }

  /*!
   * Checks whether the object is a ray.
   */
  bool is_ray() const
  { return (is_zero(rep().a()) && (rep().has_target() != rep().has_source())); }

  /*!
   * Check whether the object is a line.
   */
  bool is_line() const
  { return (is_zero(rep().a()) && !rep().has_target() && !rep().has_source()); }

  /*!
   * Checks whether the curve is vertical.
   */
  bool is_vertical() const
  { return (is_zero(rep().a()) && is_zero(rep().c())); }

  /*!
   * Checks whether the curve is horizontal.
   */
  bool is_vertical() const
  { return (is_zero(rep().a()) && is_zero(rep().b())); }
  
  /*!
   * Get the source point.
   * \pre The object is a point, a segment or a ray.
   */
  const Point_2& source() const
  {
    CGAL_precondition(rep().has_source());
    return rep().source();
  }

  /*!
   * Get the target point.
   * \pre The object is a point or a segment.
   */
  const Point_2& target() const
  {
    CGAL_precondition(rep().has_target());
    return rep().target();
  }
};

/*!
 * Exporter for the segment class used by the traits-class.
 */
template <typename Kernel, bool Filter, class OutputStream>
OutputStream&
operator<<(OutputStream& os,
           const Rectangular_hyperbola_with_iso_asymptotes_2<Kernel, Filter>& cv)
{
  typedef typename Kernel::NT                           NT;
  typedef Sqrt_extension_point_2<NT, Filter>            Point_2;
  typedef Rectangular_hyperbola_with_iso_asymptotes_2<Kernel, Filter>
                                                        Curve_2;

  // Print a letter identifying the object type, then the object itself.
  if (cv.has_source()) os << "s " << cv.source();
  if (cv.has_target()) os << "s " << cv.target();
  os << cv.a() << " " << cv.b() << " " << cv.c() << " " << cv.d();
  return os;
}

/*!
 * Importer for the segment class used by the traits-class.
 */
template <typename Kernel, bool Filter, typename InputStream>
InputStream&
operator>>(InputStream& is,
           Rectangular_hyperbola_with_iso_asymptotes_2<Kernel, Filter>& cv)
{
  typedef typename Kernel::NT                           NT;
  typedef Sqrt_extension_point_2<NT, Filter>            Point_2;
  typedef Rectangular_hyperbola_with_iso_asymptotes_2<Kernel, Filter>
                                                        Curve_2;
  
  // Read the object type.
  bool source_point = true;
  bool target_point = true;
  Point_2 source;
  Point_2 target;
  char c;

  do is >> c;
  while ((c != 'P' && c != 'p') && (c != 'H' && c != 'h'));

  // Read the object accordingly.
  if (c == 'P' || c == 'p') {
    if (source_point) {
      source_point = false;
      is >> source;
    } else if (target_point) {
      target_point = false;
      is >> target;
    } else CGAL_error("Two many points!");
  }
  else if (c == 'H' || c == 'h') {
    is >> a >> b >> c >> d;
  }
  cv = Curve_2(a, b, c, d, source, target);
  return is;
}

}

#endif
