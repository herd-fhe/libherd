add_executable(data_frame_upload)
set_target_properties(data_frame_upload PROPERTIES FOLDER example)

target_sources(
        data_frame_upload
        PRIVATE
        main.cpp
)
target_link_libraries(data_frame_upload PRIVATE example_target_options)
target_link_libraries(
        data_frame_upload
        PRIVATE
        spdlog::spdlog_header_only
        libherd
)

target_include_directories(data_frame_upload PRIVATE ../utils)