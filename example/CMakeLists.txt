add_library(example_target_options INTERFACE)
target_link_libraries(
        example_target_options
        INTERFACE
        options_target
        warnings_target
)

add_subdirectory(authorization)
add_subdirectory(session_manipulation)