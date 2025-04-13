file(GLOB_RECURSE SERVER_SOURCES CONFIGURE_DEPENDS *.cc)
add_library(server OBJECT ${SERVER_SOURCES})
target_include_directories(server PUBLIC
    ${CMAKE_SOURCE_DIR}/include
    ${Boost_INCLUDE_DIRS}
)
target_link_libraries(server PUBLIC ${Boost_LIBRARIES})