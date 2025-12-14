file(GLOB_RECURSE HTTP_SOURCES CONFIGURE_DEPENDS *.cc)
add_library(https OBJECT ${HTTP_SOURCES})
target_include_directories(https PUBLIC
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/src
    ${Boost_INCLUDE_DIRS}
)
target_link_libraries(https public logger)

set_target_properties(https PROPERTIES POSITION_INDEPENDENT_CODE ON)
