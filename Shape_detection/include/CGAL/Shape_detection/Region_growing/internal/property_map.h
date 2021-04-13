// Copyright (c) 2018 INRIA Sophia-Antipolis (France).
// All rights reserved.
//
// This file is part of CGAL (www.cgal.org).
//
// $URL$
// $Id$
// SPDX-License-Identifier: GPL-3.0-or-later OR LicenseRef-Commercial
//
//
// Author(s)     : Florent Lafarge, Simon Giraudot, Thien Hoang, Dmitry Anisimov
//

#ifndef CGAL_SHAPE_DETECTION_REGION_GROWING_INTERNAL_PROPERTY_MAP_H
#define CGAL_SHAPE_DETECTION_REGION_GROWING_INTERNAL_PROPERTY_MAP_H

#include <CGAL/license/Shape_detection.h>

// STL includes.
#include <map>
#include <vector>
#include <memory>

// CGAL includes.
#include <CGAL/assertions.h>
#include <CGAL/property_map.h>

// Internal includes.
#include <CGAL/Shape_detection/Region_growing/internal/region_growing_traits.h>

namespace CGAL {
namespace Shape_detection {
namespace internal {

  template<
  typename ItemRange,
  typename PropertyMap>
  class Item_property_map {

  public:
    using Item_range = ItemRange;
    using Property_map = PropertyMap;

    using value_type = typename Property_map::value_type;
    using reference = const value_type&;
    using key_type = std::size_t;
    using category = boost::lvalue_property_map_tag;

    Item_property_map(
      const Item_range& item_range,
      const Property_map& property_map) :
    m_item_range(item_range),
    m_property_map(property_map)
    { }

    reference operator[](const key_type item_index) const {
      CGAL_precondition(item_index < m_item_range.size());
      const auto& key = *(m_item_range.begin() + item_index);
      return get(m_property_map, key);
    }

    friend inline reference get(
      const Item_property_map& item_map, const key_type key) {
      return item_map[key];
    }

  private:
    const Item_range& m_item_range;
    const Property_map& m_property_map;
  };

  template<typename ItemRange>
  class Item_to_index_property_map {

  public:
    using Item_range = ItemRange;

    using Iterator = typename Item_range::const_iterator;
    using Item = typename Iterator::value_type;

    using value_type = std::size_t;
    using key_type = Item;
    using category = boost::readable_property_map_tag;

    using Item_map = std::map<key_type, value_type>;

    Item_to_index_property_map(const Item_range& item_range) :
    m_item_range(item_range) {

      value_type i = 0;
      for (const auto& item : item_range) {
        m_item_map[item] = i;
        ++i;
      }
    }

    value_type operator[](const key_type& key) const {

      const auto& value = m_item_map.find(key);
      if (value == m_item_map.end())
        return value_type(-1);
      return value->second;
    }

    friend inline value_type get(
      const Item_to_index_property_map& item_to_index_map, const key_type& key) {
      return item_to_index_map[key];
    }

  private:
    const Item_range& m_item_range;
    Item_map m_item_map;
  };

  class Seed_property_map {

  public:
    using key_type = std::size_t;
    using value_type = std::size_t;
    using category = boost::readable_property_map_tag;

    Seed_property_map(
      const std::vector<std::size_t>& seeds) :
    m_seeds(seeds)
    { }

    value_type operator[](const key_type key) const {
      return m_seeds[key];
    }

    friend value_type get(
      const Seed_property_map& seed_map, const key_type key) {
      return seed_map[key];
    }

  private:
    const std::vector<std::size_t>& m_seeds;
  };

  class Item_to_region_index_map {

  public:
    using key_type = std::size_t;
    using value_type = int;
    using reference = value_type;
    using category = boost::readable_property_map_tag;

    Item_to_region_index_map() { }

    template<typename ItemRange>
    Item_to_region_index_map(
      const ItemRange& items,
      const std::vector< std::vector<std::size_t> >& regions) :
    m_indices(items.size(), -1) {

      for (std::size_t i = 0; i < regions.size(); ++i) {
        for (const std::size_t index : regions[i]) {
          CGAL_precondition(index < m_indices.size());
          m_indices[index] = static_cast<int>(i);
        }
      }
    }

    inline friend value_type get(
      const Item_to_region_index_map& item_to_region_index_map,
      const key_type key) {

      const auto& indices = item_to_region_index_map.m_indices;
      CGAL_precondition(key < indices.size());
      return indices[key];
    }

  private:
    std::vector<int> m_indices;
  };

  template<
  typename KeyType,
  typename FaceGraph,
  typename EdgeRange,
  typename VertexToPointMap>
  class Polyline_graph_segment_map {

  public:
    using Key_type = KeyType;
    using Face_graph = FaceGraph;
    using Edge_range = EdgeRange;
    using Vertex_to_point_map = VertexToPointMap;

    using Point_type = typename Vertex_to_point_map::value_type;
    using Traits = typename Kernel_traits<Point_type>::Kernel;
    using RG_traits = typename std::conditional<
      std::is_same<typename Traits::Point_2, Point_type>::value,
      internal::Polyline_graph_traits_2<Traits>,
      internal::Polyline_graph_traits_3<Traits> >::type;

    using value_type = typename RG_traits::Segment;
    using reference = value_type;
    using key_type = Key_type;
    using category = boost::readable_property_map_tag;

    Polyline_graph_segment_map() :
    m_face_graph(nullptr),
    m_edge_range(nullptr),
    m_vertex_to_point_map(nullptr)
    { }

    Polyline_graph_segment_map(
      const Face_graph& face_graph,
      const Edge_range& edge_range,
      const Vertex_to_point_map& vertex_to_point_map) :
    m_face_graph(std::make_shared<Face_graph>(face_graph)),
    m_edge_range(std::make_shared<Edge_range>(edge_range)),
    m_vertex_to_point_map(std::make_shared<Vertex_to_point_map>(vertex_to_point_map))
    { }

    reference operator[](const key_type& edge) const {
      CGAL_precondition(edge.index < m_edge_range->size());
      const auto& key = *(m_edge_range->begin() + edge.index);
      const auto& s = get(*m_vertex_to_point_map, source(key, *m_face_graph));
      const auto& t = get(*m_vertex_to_point_map, target(key, *m_face_graph));
      const auto construct_segment = RG_traits().construct_segment_object();
      return construct_segment(s, t);
    }

    friend inline reference get(
      const Polyline_graph_segment_map& pgraph_map, const key_type& key) {
      return pgraph_map[key];
    }

  private:
    const std::shared_ptr<Face_graph> m_face_graph;
    const std::shared_ptr<Edge_range> m_edge_range;
    const std::shared_ptr<Vertex_to_point_map> m_vertex_to_point_map;
  };

} // namespace internal
} // namespace Shape_detection
} // namespace CGAL

#endif // CGAL_SHAPE_DETECTION_REGION_GROWING_INTERNAL_PROPERTY_MAP_H
