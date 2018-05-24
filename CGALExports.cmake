# Generated by CMake 3.5.1

if("${CMAKE_MAJOR_VERSION}.${CMAKE_MINOR_VERSION}" LESS 2.5)
   message(FATAL_ERROR "CMake >= 2.6.0 required")
endif()
cmake_policy(PUSH)
cmake_policy(VERSION 2.6)
#----------------------------------------------------------------
# Generated CMake target import file.
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Protect against multiple inclusion, which would fail when already imported targets are added once more.
set(_targetsDefined)
set(_targetsNotDefined)
set(_expectedTargets)
foreach(_expectedTarget CGAL::CGAL CGAL::CGAL_Core CGAL::CGAL_ImageIO CGAL::CGAL_Qt5)
  list(APPEND _expectedTargets ${_expectedTarget})
  if(NOT TARGET ${_expectedTarget})
    list(APPEND _targetsNotDefined ${_expectedTarget})
  endif()
  if(TARGET ${_expectedTarget})
    list(APPEND _targetsDefined ${_expectedTarget})
  endif()
endforeach()
if("${_targetsDefined}" STREQUAL "${_expectedTargets}")
  set(CMAKE_IMPORT_FILE_VERSION)
  cmake_policy(POP)
  return()
endif()
if(NOT "${_targetsDefined}" STREQUAL "")
  message(FATAL_ERROR "Some (but not all) targets in this export set were already defined.\nTargets Defined: ${_targetsDefined}\nTargets not yet defined: ${_targetsNotDefined}\n")
endif()
unset(_targetsDefined)
unset(_targetsNotDefined)
unset(_expectedTargets)


# Create imported target CGAL::CGAL
add_library(CGAL::CGAL SHARED IMPORTED)

set_target_properties(CGAL::CGAL PROPERTIES
  INTERFACE_COMPILE_OPTIONS "\$<\$<COMPILE_LANGUAGE:CXX>:-frounding-math>"
  INTERFACE_INCLUDE_DIRECTORIES "/usr/include/x86_64-linux-gnu;/usr/include;/usr/include;/home/apurva/Desktop/to_send/cgal-public-dev/include;/home/apurva/Desktop/to_send/cgal-public-dev/AABB_tree/include;/home/apurva/Desktop/to_send/cgal-public-dev/Advancing_front_surface_reconstruction/include;/home/apurva/Desktop/to_send/cgal-public-dev/Algebraic_foundations/include;/home/apurva/Desktop/to_send/cgal-public-dev/Algebraic_kernel_d/include;/home/apurva/Desktop/to_send/cgal-public-dev/Algebraic_kernel_for_circles/include;/home/apurva/Desktop/to_send/cgal-public-dev/Algebraic_kernel_for_spheres/include;/home/apurva/Desktop/to_send/cgal-public-dev/Alpha_shapes_2/include;/home/apurva/Desktop/to_send/cgal-public-dev/Alpha_shapes_3/include;/home/apurva/Desktop/to_send/cgal-public-dev/Apollonius_graph_2/include;/home/apurva/Desktop/to_send/cgal-public-dev/Arithmetic_kernel/include;/home/apurva/Desktop/to_send/cgal-public-dev/Arrangement_on_surface_2/include;/home/apurva/Desktop/to_send/cgal-public-dev/BGL/include;/home/apurva/Desktop/to_send/cgal-public-dev/Barycentric_coordinates_2/include;/home/apurva/Desktop/to_send/cgal-public-dev/Boolean_set_operations_2/include;/home/apurva/Desktop/to_send/cgal-public-dev/Bounding_volumes/include;/home/apurva/Desktop/to_send/cgal-public-dev/Box_intersection_d/include;/home/apurva/Desktop/to_send/cgal-public-dev/CGAL_Core/include;/home/apurva/Desktop/to_send/cgal-public-dev/CGAL_ImageIO/include;/home/apurva/Desktop/to_send/cgal-public-dev/CGAL_ipelets/include;/home/apurva/Desktop/to_send/cgal-public-dev/Cartesian_kernel/include;/home/apurva/Desktop/to_send/cgal-public-dev/Circular_kernel_2/include;/home/apurva/Desktop/to_send/cgal-public-dev/Circular_kernel_3/include;/home/apurva/Desktop/to_send/cgal-public-dev/Circulator/include;/home/apurva/Desktop/to_send/cgal-public-dev/Classification/include;/home/apurva/Desktop/to_send/cgal-public-dev/Combinatorial_map/include;/home/apurva/Desktop/to_send/cgal-public-dev/Cone_spanners_2/include;/home/apurva/Desktop/to_send/cgal-public-dev/Convex_decomposition_3/include;/home/apurva/Desktop/to_send/cgal-public-dev/Convex_hull_2/include;/home/apurva/Desktop/to_send/cgal-public-dev/Convex_hull_3/include;/home/apurva/Desktop/to_send/cgal-public-dev/Convex_hull_d/include;/home/apurva/Desktop/to_send/cgal-public-dev/Distance_2/include;/home/apurva/Desktop/to_send/cgal-public-dev/Distance_3/include;/home/apurva/Desktop/to_send/cgal-public-dev/Envelope_2/include;/home/apurva/Desktop/to_send/cgal-public-dev/Envelope_3/include;/home/apurva/Desktop/to_send/cgal-public-dev/Filtered_kernel/include;/home/apurva/Desktop/to_send/cgal-public-dev/Generalized_map/include;/home/apurva/Desktop/to_send/cgal-public-dev/Generator/include;/home/apurva/Desktop/to_send/cgal-public-dev/Geomview/include;/home/apurva/Desktop/to_send/cgal-public-dev/GraphicsView/include;/home/apurva/Desktop/to_send/cgal-public-dev/HalfedgeDS/include;/home/apurva/Desktop/to_send/cgal-public-dev/Hash_map/include;/home/apurva/Desktop/to_send/cgal-public-dev/Homogeneous_kernel/include;/home/apurva/Desktop/to_send/cgal-public-dev/Inscribed_areas/include;/home/apurva/Desktop/to_send/cgal-public-dev/Installation/include;/home/apurva/Desktop/to_send/cgal-public-dev/Interpolation/include;/home/apurva/Desktop/to_send/cgal-public-dev/Intersections_2/include;/home/apurva/Desktop/to_send/cgal-public-dev/Intersections_3/include;/home/apurva/Desktop/to_send/cgal-public-dev/Interval_skip_list/include;/home/apurva/Desktop/to_send/cgal-public-dev/Interval_support/include;/home/apurva/Desktop/to_send/cgal-public-dev/Inventor/include;/home/apurva/Desktop/to_send/cgal-public-dev/Jet_fitting_3/include;/home/apurva/Desktop/to_send/cgal-public-dev/Kernel_23/include;/home/apurva/Desktop/to_send/cgal-public-dev/Kernel_d/include;/home/apurva/Desktop/to_send/cgal-public-dev/LEDA/include;/home/apurva/Desktop/to_send/cgal-public-dev/Linear_cell_complex/include;/home/apurva/Desktop/to_send/cgal-public-dev/Matrix_search/include;/home/apurva/Desktop/to_send/cgal-public-dev/Mesh_2/include;/home/apurva/Desktop/to_send/cgal-public-dev/Mesh_3/include;/home/apurva/Desktop/to_send/cgal-public-dev/Mesher_level/include;/home/apurva/Desktop/to_send/cgal-public-dev/Minkowski_sum_2/include;/home/apurva/Desktop/to_send/cgal-public-dev/Minkowski_sum_3/include;/home/apurva/Desktop/to_send/cgal-public-dev/Modifier/include;/home/apurva/Desktop/to_send/cgal-public-dev/Modular_arithmetic/include;/home/apurva/Desktop/to_send/cgal-public-dev/Nef_2/include;/home/apurva/Desktop/to_send/cgal-public-dev/Nef_3/include;/home/apurva/Desktop/to_send/cgal-public-dev/Nef_S2/include;/home/apurva/Desktop/to_send/cgal-public-dev/NewKernel_d/include;/home/apurva/Desktop/to_send/cgal-public-dev/Number_types/include;/home/apurva/Desktop/to_send/cgal-public-dev/OpenNL/include;/home/apurva/Desktop/to_send/cgal-public-dev/Operations_on_polyhedra/include;/home/apurva/Desktop/to_send/cgal-public-dev/Optimal_transportation_reconstruction_2/include;/home/apurva/Desktop/to_send/cgal-public-dev/Optimisation_basic/include;/home/apurva/Desktop/to_send/cgal-public-dev/Partition_2/include;/home/apurva/Desktop/to_send/cgal-public-dev/Periodic_2_triangulation_2/include;/home/apurva/Desktop/to_send/cgal-public-dev/Periodic_3_triangulation_3/include;/home/apurva/Desktop/to_send/cgal-public-dev/Point_set_2/include;/home/apurva/Desktop/to_send/cgal-public-dev/Point_set_3/include;/home/apurva/Desktop/to_send/cgal-public-dev/Point_set_processing_3/include;/home/apurva/Desktop/to_send/cgal-public-dev/Point_set_shape_detection_3/include;/home/apurva/Desktop/to_send/cgal-public-dev/Poisson_surface_reconstruction_3/include;/home/apurva/Desktop/to_send/cgal-public-dev/Polygon/include;/home/apurva/Desktop/to_send/cgal-public-dev/Polygon_mesh_processing/include;/home/apurva/Desktop/to_send/cgal-public-dev/Polyhedron/include;/home/apurva/Desktop/to_send/cgal-public-dev/Polyhedron_IO/include;/home/apurva/Desktop/to_send/cgal-public-dev/Polyline_simplification_2/include;/home/apurva/Desktop/to_send/cgal-public-dev/Polynomial/include;/home/apurva/Desktop/to_send/cgal-public-dev/Polytope_distance_d/include;/home/apurva/Desktop/to_send/cgal-public-dev/Principal_component_analysis/include;/home/apurva/Desktop/to_send/cgal-public-dev/Principal_component_analysis_LGPL/include;/home/apurva/Desktop/to_send/cgal-public-dev/Profiling_tools/include;/home/apurva/Desktop/to_send/cgal-public-dev/Property_map/include;/home/apurva/Desktop/to_send/cgal-public-dev/QP_solver/include;/home/apurva/Desktop/to_send/cgal-public-dev/Random_numbers/include;/home/apurva/Desktop/to_send/cgal-public-dev/Ridges_3/include;/home/apurva/Desktop/to_send/cgal-public-dev/STL_Extension/include;/home/apurva/Desktop/to_send/cgal-public-dev/Scale_space_reconstruction_3/include;/home/apurva/Desktop/to_send/cgal-public-dev/SearchStructures/include;/home/apurva/Desktop/to_send/cgal-public-dev/Segment_Delaunay_graph_2/include;/home/apurva/Desktop/to_send/cgal-public-dev/Segment_Delaunay_graph_Linf_2/include;/home/apurva/Desktop/to_send/cgal-public-dev/Set_movable_separability_2/include;/home/apurva/Desktop/to_send/cgal-public-dev/Skin_surface_3/include;/home/apurva/Desktop/to_send/cgal-public-dev/Snap_rounding_2/include;/home/apurva/Desktop/to_send/cgal-public-dev/Solver_interface/include;/home/apurva/Desktop/to_send/cgal-public-dev/Spatial_searching/include;/home/apurva/Desktop/to_send/cgal-public-dev/Spatial_sorting/include;/home/apurva/Desktop/to_send/cgal-public-dev/Straight_skeleton_2/include;/home/apurva/Desktop/to_send/cgal-public-dev/Stream_lines_2/include;/home/apurva/Desktop/to_send/cgal-public-dev/Stream_support/include;/home/apurva/Desktop/to_send/cgal-public-dev/Subdivision_method_3/include;/home/apurva/Desktop/to_send/cgal-public-dev/Surface_mesh/include;/home/apurva/Desktop/to_send/cgal-public-dev/Surface_mesh_deformation/include;/home/apurva/Desktop/to_send/cgal-public-dev/Surface_mesh_parameterization/include;/home/apurva/Desktop/to_send/cgal-public-dev/Surface_mesh_segmentation/include;/home/apurva/Desktop/to_send/cgal-public-dev/Surface_mesh_shortest_path/include;/home/apurva/Desktop/to_send/cgal-public-dev/Surface_mesh_simplification/include;/home/apurva/Desktop/to_send/cgal-public-dev/Surface_mesh_skeletonization/include;/home/apurva/Desktop/to_send/cgal-public-dev/Surface_mesher/include;/home/apurva/Desktop/to_send/cgal-public-dev/Surface_sweep_2/include;/home/apurva/Desktop/to_send/cgal-public-dev/TDS_2/include;/home/apurva/Desktop/to_send/cgal-public-dev/TDS_3/include;/home/apurva/Desktop/to_send/cgal-public-dev/Testsuite/include;/home/apurva/Desktop/to_send/cgal-public-dev/Three/include;/home/apurva/Desktop/to_send/cgal-public-dev/Triangulation/include;/home/apurva/Desktop/to_send/cgal-public-dev/Triangulation_2/include;/home/apurva/Desktop/to_send/cgal-public-dev/Triangulation_3/include;/home/apurva/Desktop/to_send/cgal-public-dev/Union_find/include;/home/apurva/Desktop/to_send/cgal-public-dev/Visibility_2/include;/home/apurva/Desktop/to_send/cgal-public-dev/Voronoi_diagram_2/include"
  INTERFACE_LINK_LIBRARIES "/usr/lib/x86_64-linux-gnu/libgmp.so;/usr/lib/x86_64-linux-gnu/libmpfr.so;/usr/lib/x86_64-linux-gnu/libboost_thread.so;/usr/lib/x86_64-linux-gnu/libboost_system.so;/usr/lib/x86_64-linux-gnu/libboost_chrono.so;/usr/lib/x86_64-linux-gnu/libboost_date_time.so;/usr/lib/x86_64-linux-gnu/libboost_atomic.so;/usr/lib/x86_64-linux-gnu/libpthread.so"
)

# Create imported target CGAL::CGAL_Core
add_library(CGAL::CGAL_Core SHARED IMPORTED)

set_target_properties(CGAL::CGAL_Core PROPERTIES
  INTERFACE_COMPILE_DEFINITIONS "CGAL_USE_CORE=1"
  INTERFACE_INCLUDE_DIRECTORIES "/usr/include/x86_64-linux-gnu;/usr/include"
  INTERFACE_LINK_LIBRARIES "/usr/lib/x86_64-linux-gnu/libgmp.so;/usr/lib/x86_64-linux-gnu/libmpfr.so;CGAL::CGAL;Boost::thread"
)

# Create imported target CGAL::CGAL_ImageIO
add_library(CGAL::CGAL_ImageIO SHARED IMPORTED)

set_target_properties(CGAL::CGAL_ImageIO PROPERTIES
  INTERFACE_COMPILE_DEFINITIONS "CGAL_USE_ZLIB=1"
  INTERFACE_INCLUDE_DIRECTORIES "/usr/include"
  INTERFACE_LINK_LIBRARIES "CGAL::CGAL;/usr/lib/x86_64-linux-gnu/libz.so"
)

# Create imported target CGAL::CGAL_Qt5
add_library(CGAL::CGAL_Qt5 SHARED IMPORTED)

set_target_properties(CGAL::CGAL_Qt5 PROPERTIES
  INTERFACE_LINK_LIBRARIES "CGAL::CGAL;Qt5::OpenGL;Qt5::Svg"
)

# Import target "CGAL::CGAL" for configuration "RELEASEWITH_demos=ON"
set_property(TARGET CGAL::CGAL APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASEWITH_DEMOS=ON)
set_target_properties(CGAL::CGAL PROPERTIES
  IMPORTED_LOCATION_RELEASEWITH_DEMOS=ON "/home/apurva/Desktop/to_send/cgal-public-dev/lib/libCGAL.so.13.0.2"
  IMPORTED_SONAME_RELEASEWITH_DEMOS=ON "libCGAL.so.13"
  )

# Import target "CGAL::CGAL_Core" for configuration "RELEASEWITH_demos=ON"
set_property(TARGET CGAL::CGAL_Core APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASEWITH_DEMOS=ON)
set_target_properties(CGAL::CGAL_Core PROPERTIES
  IMPORTED_LOCATION_RELEASEWITH_DEMOS=ON "/home/apurva/Desktop/to_send/cgal-public-dev/lib/libCGAL_Core.so.13.0.2"
  IMPORTED_SONAME_RELEASEWITH_DEMOS=ON "libCGAL_Core.so.13"
  )

# Import target "CGAL::CGAL_ImageIO" for configuration "RELEASEWITH_demos=ON"
set_property(TARGET CGAL::CGAL_ImageIO APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASEWITH_DEMOS=ON)
set_target_properties(CGAL::CGAL_ImageIO PROPERTIES
  IMPORTED_LOCATION_RELEASEWITH_DEMOS=ON "/home/apurva/Desktop/to_send/cgal-public-dev/lib/libCGAL_ImageIO.so.13.0.2"
  IMPORTED_SONAME_RELEASEWITH_DEMOS=ON "libCGAL_ImageIO.so.13"
  )

# Import target "CGAL::CGAL_Qt5" for configuration "RELEASEWITH_demos=ON"
set_property(TARGET CGAL::CGAL_Qt5 APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASEWITH_DEMOS=ON)
set_target_properties(CGAL::CGAL_Qt5 PROPERTIES
  IMPORTED_LOCATION_RELEASEWITH_DEMOS=ON "/home/apurva/Desktop/to_send/cgal-public-dev/lib/libCGAL_Qt5.so.13.0.2"
  IMPORTED_SONAME_RELEASEWITH_DEMOS=ON "libCGAL_Qt5.so.13"
  )

# This file does not depend on other imported targets which have
# been exported from the same project but in a separate export set.

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
cmake_policy(POP)
