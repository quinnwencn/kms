file(GLOB_RECURSE ROUTE_SOURCES CONFIGURE_DEPENDS *.cc)
add_library(route OBJECT ${ROUTE_SOURCES})
target_include_directories(route PUBLIC
    ${CMAKE_SOURCE_DIR}/include
    ${Boost_INCLUDE_DIRS}
)

set_target_properties(route PROPERTIES POSITION_INDEPENDENT_CODE ON)
