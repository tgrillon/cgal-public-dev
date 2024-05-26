set(CGAL_MAJOR_VERSION 6)
set(CGAL_MINOR_VERSION 0)
set(CGAL_BUGFIX_VERSION 0)
include(${CMAKE_CURRENT_LIST_DIR}/CGALConfigBuildVersion.cmake)
set(CGAL_VERSION_PUBLIC_RELEASE_VERSION "6.0-beta1")
set(CGAL_VERSION_PUBLIC_RELEASE_NAME "CGAL-${CGAL_VERSION_PUBLIC_RELEASE_VERSION}")

if (CGAL_BUGFIX_VERSION AND CGAL_BUGFIX_VERSION GREATER 0)
  set(CGAL_CREATED_VERSION_NUM "${CGAL_MAJOR_VERSION}.${CGAL_MINOR_VERSION}.${CGAL_BUGFIX_VERSION}")
else()
  set(CGAL_CREATED_VERSION_NUM "${CGAL_MAJOR_VERSION}.${CGAL_MINOR_VERSION}")
endif()

set(PACKAGE_VERSION ${CGAL_CREATED_VERSION_NUM})

if(PACKAGE_VERSION VERSION_LESS PACKAGE_FIND_VERSION)
  set(PACKAGE_VERSION_COMPATIBLE FALSE)
else()
  set(PACKAGE_VERSION_COMPATIBLE TRUE)
  if(PACKAGE_FIND_VERSION STREQUAL PACKAGE_VERSION)
    set(PACKAGE_VERSION_EXACT TRUE)
  endif()
endif()
