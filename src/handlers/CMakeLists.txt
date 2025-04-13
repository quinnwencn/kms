file(GLOB_RECURSE HANDLER_SOURCES CONFIGURE_DEPENDS *.cc)
add_library(handlers OBJECT ${HANDLER_SOURCES})
target_include_directories(handlers PUBLIC
    ${CMAKE_SOURCE_DIR}/include
    ${Boost_INCLUDE_DIRS}
)
