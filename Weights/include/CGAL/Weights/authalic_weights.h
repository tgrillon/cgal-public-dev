// Copyright (c) 2020 GeometryFactory SARL (France).
// All rights reserved.
//
// This file is part of CGAL (www.cgal.org).
//
// $URL$
// $Id$
// SPDX-License-Identifier: LGPL-3.0-or-later OR LicenseRef-Commercial
//
//
// Author(s)     : Dmitry Anisimov
//

#ifndef CGAL_AUTHALIC_WEIGHTS_H
#define CGAL_AUTHALIC_WEIGHTS_H

#include <CGAL/Weights/utils.h>

#include <CGAL/Point_2.h>
#include <CGAL/Point_3.h>

namespace CGAL {
namespace Weights {

/// \cond SKIP_IN_MANUAL

namespace authalic_ns {

template<typename FT>
FT half_weight(const FT cot, const FT r2)
{
  FT w = FT(0);
  CGAL_precondition(!is_zero(r2));
  if (!is_zero(r2))
    w = FT(2) * cot / r2;

  return w;
}

template<typename FT>
FT weight(const FT cot_gamma, const FT cot_beta, const FT r2)
{
  FT w = FT(0);
  CGAL_precondition(!is_zero(r2));
  if (!is_zero(r2))
    w = FT(2) * (cot_gamma + cot_beta) / r2;

  return w;
}

} // namespace authalic_ns

/// \endcond

/*!
  \ingroup PkgWeightsRefAuthalicWeights

  \brief computes the half value of the authalic weight.

  This function constructs the half of the authalic weight using the precomputed
  cotangent and squared distance values. The returned value is
  \f$\frac{2\textbf{cot}}{\textbf{d2}}\f$.

  \tparam FT a model of `FieldNumberType`

  \param cot the cotangent value
  \param d2 the squared distance value

  \pre d2 != 0

  \sa `authalic_weight()`
*/
template<typename FT>
FT half_authalic_weight(const FT cot, const FT d2)
{
  return authalic_ns::half_weight(cot, d2);
}

template<typename GeomTraits>
typename GeomTraits::FT authalic_weight(const typename GeomTraits::Point_2& t,
                                        const typename GeomTraits::Point_2& r,
                                        const typename GeomTraits::Point_2& p,
                                        const typename GeomTraits::Point_2& q,
                                        const GeomTraits& traits)
{
  using FT = typename GeomTraits::FT;

  auto squared_distance_2 = traits.compute_squared_distance_2_object();

  const FT cot_gamma = internal::cotangent_2(traits, t, r, q);
  const FT cot_beta  = internal::cotangent_2(traits, q, r, p);

  const FT d2 = squared_distance_2(q, r);
  return authalic_ns::weight(cot_gamma, cot_beta, d2);
}

template<typename GeomTraits>
typename GeomTraits::FT authalic_weight(const CGAL::Point_2<GeomTraits>& t,
                                        const CGAL::Point_2<GeomTraits>& r,
                                        const CGAL::Point_2<GeomTraits>& p,
                                        const CGAL::Point_2<GeomTraits>& q)
{
  const GeomTraits traits;
  return authalic_weight(t, r, p, q, traits);
}

template<typename GeomTraits>
typename GeomTraits::FT authalic_weight(const typename GeomTraits::Point_3& t,
                                        const typename GeomTraits::Point_3& r,
                                        const typename GeomTraits::Point_3& p,
                                        const typename GeomTraits::Point_3& q,
                                        const GeomTraits& traits)
{
  using FT = typename GeomTraits::FT;

  auto squared_distance_3 = traits.compute_squared_distance_3_object();

  const FT cot_gamma = internal::cotangent_3(traits, t, r, q);
  const FT cot_beta  = internal::cotangent_3(traits, q, r, p);
  const FT d2 = squared_distance_3(q, r);

  return authalic_ns::weight(cot_gamma, cot_beta, d2);
}

template<typename GeomTraits>
typename GeomTraits::FT authalic_weight(const CGAL::Point_3<GeomTraits>& t,
                                        const CGAL::Point_3<GeomTraits>& r,
                                        const CGAL::Point_3<GeomTraits>& p,
                                        const CGAL::Point_3<GeomTraits>& q)
{
  const GeomTraits traits;
  return authalic_weight(t, r, p, q, traits);
}

} // namespace Weights
} // namespace CGAL

#endif // CGAL_AUTHALIC_WEIGHTS_H
