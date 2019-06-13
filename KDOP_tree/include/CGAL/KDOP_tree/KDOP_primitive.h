// Copyright (c) 2019  University of Cambridge (UK), GeometryFactory (France)
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
// SPDX-License-Identifier: GPL-3.0+
//
//
// Author(s) : Xiao Xiao, Fehmi Cirak, Andreas Fabri

#ifndef CGAL_KDOP_TREE_KDOP_PRIMITIVE_H_
#define CGAL_KDOP_TREE_KDOP_PRIMITIVE_H_

#include <CGAL/disable_warnings.h>

#include <CGAL/KDOP_tree/internal/Has_nested_type_Shared_data.h>
#include <CGAL/property_map.h>
#include <CGAL/tags.h>

/// \file KDOP_primitive.h

namespace CGAL {
namespace KDOP_tree {

  // class for typedefs
  template < class Id_,
             class ObjectPropertyMap,
             class PointPropertyMap >
  struct KDOP_primitive_base
  {
    typedef typename boost::property_traits< ObjectPropertyMap >::value_type Datum; //datum type
    typedef typename boost::property_traits< PointPropertyMap  >::value_type Point; //point type
    typedef typename boost::property_traits< ObjectPropertyMap >::reference Datum_reference; //reference datum type
    typedef typename boost::property_traits< PointPropertyMap  >::reference Point_reference; //reference point type
    typedef Id_ Id; // Id type

  protected:
    Id m_id;

  public:
    // constructors
    KDOP_primitive_base(Id id) : m_id(id) {}

    Id id() const {return m_id;}
  };

  template <  class Id,
              class ObjectPropertyMap,
              class PointPropertyMap,
              class ExternalPropertyMaps,
              class CacheDatum  >
  class KDOP_primitive;

  //no caching, property maps internally stored
  template <  class Id,
  class ObjectPropertyMap,
  class PointPropertyMap >
  class KDOP_primitive<Id, ObjectPropertyMap, PointPropertyMap,Tag_false,Tag_false>
  : public KDOP_primitive_base<Id,ObjectPropertyMap,PointPropertyMap>
  {
    typedef KDOP_primitive_base<Id,ObjectPropertyMap,PointPropertyMap> Base;
    ObjectPropertyMap m_obj_pmap;
    PointPropertyMap m_pt_pmap;
  public:
    KDOP_primitive(Id id, ObjectPropertyMap obj_pmap=ObjectPropertyMap(), PointPropertyMap pt_pmap=PointPropertyMap())
  : Base(id), m_obj_pmap(obj_pmap), m_pt_pmap(pt_pmap) {}

    template <class Iterator>
    KDOP_primitive(Iterator it, ObjectPropertyMap obj_pmap=ObjectPropertyMap(), PointPropertyMap pt_pmap=PointPropertyMap())
    : Base(*it), m_obj_pmap(obj_pmap), m_pt_pmap(pt_pmap) {}

    typename Base::Datum_reference
    datum() const { return get(m_obj_pmap,this->m_id); }

    typename Base::Point_reference
    reference_point() const { return get(m_pt_pmap,this->m_id); }
  };

  //caching, property maps internally stored
  template <  class Id,
  class ObjectPropertyMap,
  class PointPropertyMap >
  class KDOP_primitive<Id, ObjectPropertyMap, PointPropertyMap,Tag_false,Tag_true>
  : public KDOP_primitive_base<Id,ObjectPropertyMap,PointPropertyMap>
  {
    typedef KDOP_primitive_base<Id,ObjectPropertyMap,PointPropertyMap> Base;
    typename boost::property_traits< ObjectPropertyMap >::value_type m_datum;
    PointPropertyMap m_pt_pmap;
  public:
    typedef const typename Base::Datum& Datum_reference;

    KDOP_primitive(Id id, ObjectPropertyMap obj_pmap=ObjectPropertyMap(), PointPropertyMap pt_pmap=PointPropertyMap())
    : Base(id), m_datum( get(obj_pmap,id) ), m_pt_pmap(pt_pmap){}

    template <class Iterator>
    KDOP_primitive(Iterator it, ObjectPropertyMap obj_pmap=ObjectPropertyMap(), PointPropertyMap pt_pmap=PointPropertyMap())
    : Base(*it), m_datum( get(obj_pmap,*it) ), m_pt_pmap(pt_pmap){}


    Datum_reference datum() const { return m_datum; }

    typename Base::Point_reference
    reference_point() const { return get(m_pt_pmap,this->m_id); }
  };

  //no caching, property maps are stored outside the class
  template <  class Id,
  class ObjectPropertyMap,
  class PointPropertyMap >
  class KDOP_primitive<Id, ObjectPropertyMap, PointPropertyMap,Tag_true,Tag_false>
  : public KDOP_primitive_base<Id,ObjectPropertyMap,PointPropertyMap>
  {
    typedef KDOP_primitive_base<Id,ObjectPropertyMap,PointPropertyMap> Base;
  public:
    typedef std::pair<ObjectPropertyMap,PointPropertyMap> Shared_data;

    KDOP_primitive(Id id, ObjectPropertyMap=ObjectPropertyMap(), PointPropertyMap=PointPropertyMap())
    : Base(id) {}

    template <class Iterator>
    KDOP_primitive(Iterator it, ObjectPropertyMap=ObjectPropertyMap(), PointPropertyMap=PointPropertyMap())
    : Base(*it) {}

    typename Base::Datum_reference
    datum(const Shared_data& data) const { return get(data.first,this->m_id); }

    typename Base::Point_reference
    reference_point(const Shared_data& data) const { return get(data.second,this->m_id); }

    static Shared_data construct_shared_data(ObjectPropertyMap obj, PointPropertyMap pt) {return Shared_data(obj,pt);}
  };


  //caching, property map is stored outside the class
  template <  class Id,
  class ObjectPropertyMap,
  class PointPropertyMap >
  class KDOP_primitive<Id, ObjectPropertyMap, PointPropertyMap,Tag_true,Tag_true>
  : public KDOP_primitive_base<Id,ObjectPropertyMap,PointPropertyMap>
  {
    typedef KDOP_primitive_base<Id,ObjectPropertyMap,PointPropertyMap> Base;
    typename boost::property_traits< ObjectPropertyMap >::value_type m_datum;
  public:
    typedef PointPropertyMap Shared_data;
    typedef const typename Base::Datum& Datum_reference;

    KDOP_primitive(Id id, ObjectPropertyMap obj_pmap=ObjectPropertyMap(), PointPropertyMap=PointPropertyMap())
    : Base(id), m_datum( get(obj_pmap,id) ) {}

    template <class Iterator>
    KDOP_primitive(Iterator it, ObjectPropertyMap obj_pmap=ObjectPropertyMap(), PointPropertyMap=PointPropertyMap())
    : Base(*it), m_datum( get(obj_pmap,*it) ) {}

    Datum_reference datum(Shared_data) const { return m_datum; }

    typename Base::Point_reference
    reference_point(const Shared_data& data) const { return get(data,this->m_id); }

    static Shared_data construct_shared_data(ObjectPropertyMap, PointPropertyMap pt) {return pt;}
  };

#ifdef DOXYGEN_RUNNING
/*!
 * \ingroup PkgKDOPTree
 * Generic primitive type.
 * The two property maps which are template parameters of the class enable to get the datum and the reference point of
 * the primitive from the identifier. The last template parameter controls whether the primitive class holds a copy of the datum.
 *
 * \cgalModels `KDOPPrimitive` if `ExternalPropertyMaps` is `CGAL::Tag_false`,
 *    and `KDOPPrimitiveWithSharedData` if `ExternalPropertyMaps` is `CGAL::Tag_true`.
 *
 *
 * \tparam ObjectPropertyMap is a model of `ReadablePropertyMap` with `Id` as
 *           `key_type`. It must be a model of `CopyConstructible`, `DefaultConstructible`, and `CopyAssignable`.
 * \tparam PointPropertyMap is a model of `ReadablePropertyMap` with `Id` as
 *           `key_type`. It must be a model of `CopyConstructible`, `DefaultConstructible`, and `CopyAssignable`.
 * \tparam ExternalPropertyMaps either `CGAL::Tag_true` or `CGAL::Tag_false`. In the former
 *          case, the property maps will be stored in the traits class, while
 *          in the latter they will be stored in the primitive
 *          (which increases the size of each primitive).
 * \tparam CacheDatum either `CGAL::Tag_true` or `CGAL::Tag_false`. In the former case,
 *           the datum is stored in the primitive, while in the latter
 *           it is constructed on the fly to reduce the memory footprint.
 *           The default is `CGAL::Tag_false` (datum is not stored).
 *
 * \sa `KDOPPrimitive`
 * \sa `KDOP_segment_primitive<Iterator,CacheDatum>`
 * \sa `KDOP_triangle_primitive<Iterator,CacheDatum>`
 * \sa `KDOP_halfedge_graph_segment_primitive<HalfedgeGraph,OneHalfedgeGraphPerTree,CacheDatum>`
 * \sa `KDOP_face_graph_triangle_primitive<FaceGraph,OneFaceGraphPerTree,CacheDatum>`
 */

  template <  class Id,
              class ObjectPropertyMap,
              class PointPropertyMap,
              class ExternalPropertyMaps,
              class CacheDatum>
  struct KDOP_primitive
  {
    /// \name Types
    /// @{
    /*!
    The datum type.
    */
    typedef boost::property_traits< ObjectPropertyMap >::value_type Datum;

    /*!
    The point type.
    */
    typedef boost::property_traits< PointPropertyMap >::value_type Point;

    /*!
    The reference datum type.
    */
    typedef boost::property_traits< ObjectPropertyMap >::reference Datum_reference;

    /*!
    The reference point type.
    */
    typedef boost::property_traits< PointPropertyMap >::reference Point_reference;

    /*!
    Id type.
    */
    typedef Id Id;
    /// @}

    /*!
    Constructs a primitive and initializes the property maps.
    */
    KDOP_primitive(Id id,
                   ObjectPropertyMap o_pmap=ObjectPropertyMap(),
                   PointPropertyMap p_pmap=PointPropertyMap());

    /*!
    Constructs a primitive from an iterator with `Id` as value type
    and initializes the property maps.
    */
    template <class Iterator>
    KDOP_primitive(Iterator it,
                   ObjectPropertyMap o_pmap=ObjectPropertyMap(),
                   PointPropertyMap p_pmap=PointPropertyMap());
  };

#endif

}
}

#include <CGAL/enable_warnings.h>

#endif // CGAL_KDOP_TREE_KDOP_PRIMITIVE_H_
