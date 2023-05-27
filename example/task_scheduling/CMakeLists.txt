add_executable(task_scheduling)
set_target_properties(task_scheduling PROPERTIES FOLDER example)

target_sources(
        task_scheduling
        PRIVATE
        main.cpp
)
target_link_libraries(task_scheduling PRIVATE example_target_options)
target_link_libraries(
        task_scheduling
        PRIVATE
        spdlog::spdlog_header_only
        libherd
)

target_include_directories(task_scheduling PRIVATE ../utils)