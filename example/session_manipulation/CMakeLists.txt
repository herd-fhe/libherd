add_executable(session_manipulation)
set_target_properties(session_manipulation PROPERTIES FOLDER example)

target_sources(
        session_manipulation
        PRIVATE
        main.cpp
)
target_link_libraries(session_manipulation PRIVATE example_target_options)
target_link_libraries(
        session_manipulation
        PRIVATE
        spdlog::spdlog_header_only
        libherd
)