add_executable(key_upload)
set_target_properties(key_upload PROPERTIES FOLDER example)

target_sources(
        key_upload
        PRIVATE
        main.cpp
)
target_link_libraries(key_upload PRIVATE example_target_options)
target_link_libraries(
        key_upload
        PRIVATE
        spdlog::spdlog_header_only
        libherd
)