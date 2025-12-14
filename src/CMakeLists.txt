add_subdirectory(http)
add_subdirectory(config)
add_subdirectory(libkms)
add_subdirectory(logger)

add_library(kms SHARED
    $<TARGET_OBJECTS:https>
    $<TARGET_OBJECTS:config>
    $<TARGET_OBJECTS:kmslib>
)

target_link_libraries(kms PUBLIC
        fmt::fmt
        logger)

set_target_properties(kms PROPERTIES POSITION_INDEPENDENT_CODE ON)
