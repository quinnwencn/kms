file(GLOB_RECURSE KMS_SOURCES CONFIGURE_DEPENDS *.cc)
add_library(kmslib OBJECT ${KMS_SOURCES})
target_include_directories(kmslib PUBLIC
        ${CMAKE_SOURCE_DIR}/include
        ${CMAKE_SOURCE_DIR}/src
)
target_link_libraries(kmslib PUBLIC logger tomlplusplus::tomlplusplus)
set_target_properties(kmslib PROPERTIES POSITION_INDEPENDENT_CODE ON)
