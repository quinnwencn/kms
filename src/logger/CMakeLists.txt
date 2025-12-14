file(GLOB_RECURSE LOGGER_SOURCES CONFIGURE_DEPENDS *.cc)
add_library(logger STATIC ${LOGGER_SOURCES})
target_include_directories(logger PUBLIC
    ${CMAKE_SOURCE_DIR}/include
    ${Boost_INCLUDE_DIRS}
    ${tomlplusplus_INCLUDE_DIRS}
    ${fmt_INCLUDE_DIRS}
)

target_link_libraries(logger PUBLIC
    fmt::fmt
    Boost::log
    Boost::log_setup)
set_target_properties(logger PROPERTIES POSITION_INDEPENDENT_CODE ON)
