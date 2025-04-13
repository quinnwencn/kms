add_subdirectory(handlers)
add_subdirectory(server)
add_subdirectory(route)

add_library(kms SHARED
    $<TARGET_OBJECTS:handlers>
    $<TARGET_OBJECTS:server>
    $<TARGET_OBJECTS:route>
)
