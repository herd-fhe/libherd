add_library(example_target_options INTERFACE)
target_link_libraries(
        example_target_options
        INTERFACE
        options_target
        warnings_target
)

add_subdirectory(authorization)
add_subdirectory(session_manipulation)
add_subdirectory(key_upload)
add_subdirectory(data_frame_upload)
add_subdirectory(task_scheduling)