add_subdirectory(handler)
add_subdirectory(server)
add_subdirectory(route)

add_library(kms SHARED
    $<TARGET_OBJECTS:handlers>
    $<TARGET_OBJECTS:server>
    $<TARGET_OBJECTS:route>
)

set_target_properties(kms PROPERTIES POSITION_INDEPENDENT_CODE ON)
