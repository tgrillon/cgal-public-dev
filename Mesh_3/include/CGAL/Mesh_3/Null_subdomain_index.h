// Copyright (c) 2015 GeometryFactory
// All rights reserved.
//
// This file is part of CGAL (www.cgal.org).
//
// $URL$
// $Id$
// SPDX-License-Identifier: GPL-3.0-or-later OR LicenseRef-Commercial
//
//
// Author(s)     : Laurent Rineau

#ifndef CGAL_INTERNAL_MESH_3_INTERNAL_NULL_SUBDOMAIN_INDEX
#define CGAL_INTERNAL_MESH_3_INTERNAL_NULL_SUBDOMAIN_INDEX

#include <CGAL/license/Mesh_3.h>

namespace CGAL {
  struct Null_subdomain_index {
    template <typename T>
    bool operator()(const T& x) const { return 0 == x; }
  };
}

#endif //CGAL_INTERNAL_MESH_3_INTERNAL_NULL_SUBDOMAIN_INDEX
