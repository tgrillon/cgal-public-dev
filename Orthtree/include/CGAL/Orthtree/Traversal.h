// Copyright (c) 2007-2020  INRIA (France).
// All rights reserved.
//
// This file is part of CGAL (www.cgal.org).
//
// $URL$
// $Id$
// SPDX-License-Identifier: GPL-3.0-or-later OR LicenseRef-Commercial
//
// Author(s)     : Jackson Campolattaro, Cédric Portaneri, Tong Zhao

#ifndef CGAL_ORTHTREE_TRAVERSAL_CRITERION_H
#define CGAL_ORTHTREE_TRAVERSAL_CRITERION_H

#include <CGAL/license/Orthtree.h>

#include <iostream>
#include <boost/range/iterator_range.hpp>
#include <CGAL/Orthtree/Traversal_iterator.h>

namespace CGAL {

namespace Orthtrees {

/// \cond SKIP_IN_MANUAL

template <typename Node>
const Node* next_sibling(const Node* n) {

  // Passing null returns the first node
  if (nullptr == n)
    return nullptr;

  // If this node has no parent, it has no siblings
  if (nullptr == n->parent())
    return nullptr;

  // Find out which child this is
  std::size_t index = n->index().to_ulong();

  constexpr static int degree = Node::Degree::value;
  // Return null if this is the last child
  if (index == degree - 1)
    return nullptr;

  // Otherwise, return the next child
  return &((*n->parent())[index + 1]);
}

template <typename Node>
const Node* next_sibling_up(const Node* n) {

  if (!n)
    return nullptr;

  auto up = n->parent();

  while (nullptr != up) {

    if (nullptr != next_sibling(up))
      return next_sibling(up);

    up = up->parent();
  }

  return nullptr;
}

template <typename Node>
const Node* deepest_first_child(const Node* n) {

  if (!n)
    return nullptr;

  // Find the deepest child on the left
  auto first = n;
  while (!first->is_leaf())
    first = &(*first)[0];
  return first;
}

/// \endcond

namespace Traversal {

/*!
  \ingroup PkgOrthtreeTraversal
  \brief walker for preorder traversal
 */
struct Preorder {

  /*!
   * \brief retrieve the first node of a tree in a preorder traversal, given the root
   *
   * \tparam Point_index
   * \param root
   * \return
   */
  template <typename Node>
  const Node* first(const Node* root) const {
    return root;
  }

  /*!
   * \brief retrieve the next node of a tree in a preorder traversal, given the current one
   *
   * \tparam Point_index
   * \param n
   * \return
   */
  template <typename Node>
  const Node* next(const Node* n) const {

    if (n->is_leaf()) {

      auto next = next_sibling(n);

      if (nullptr == next) {

        return next_sibling_up(n);
      }

      return next;

    } else {

      // Return the first child of this node
      return &(*n)[0];
    }

  }
};

/*!
  \ingroup PkgOrthtreeTraversal
  \todo
 */
struct Postorder {

  /*!
   * \brief retrieve the first node of a tree in a postorder traversal, given the root
   *
   * \tparam Point_index
   * \param root
   * \return
   */
  template <typename Node>
  const Node* first(const Node* root) const {

    return deepest_first_child(root);
  }

  /*!
   * \brief retrieve the next node of a tree in a postorder traversal, given the current one
   *
   * \tparam Point_index
   * \param n
   * \return
   */
  template <typename Node>
  const Node* next(const Node* n) const {

    auto next = deepest_first_child(next_sibling(n));

    if (!next)
      next = n->parent();

    return next;
  }
};

/*!
  \ingroup PkgOrthtreeTraversal
  \brief walker for leaves-only traversal
 */
struct Leaves {

  /*!
   * \brief retrieve the first node of a tree in a leaves-only traversal, given the root
   *
   * \tparam Point_index
   * \param root
   * \return
   */
  template <typename Node>
  const Node* first(const Node* root) const {

    return deepest_first_child(root);
  }

  /*!
   * \brief retrieve the next node of a tree in a leaves-only traversal, given the current one
   *
   * \tparam Point_index
   * \param n
   * \return
   */
  template <typename Node>
  const Node* next(const Node* n) const {

    auto next = deepest_first_child(next_sibling(n));

    if (!next)
      next = deepest_first_child(next_sibling_up(n));

    return next;
  }
};

} // Traversal
} // Orthtree
} // CGAL

#endif //CGAL_ORTHTREE_TRAVERSAL_CRITERION_H
