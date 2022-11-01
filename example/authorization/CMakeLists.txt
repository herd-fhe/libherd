add_executable(authorization)
set_target_properties(authorization PROPERTIES FOLDER example)

target_sources(
        authorization
        PRIVATE
        main.cpp
)
target_link_libraries(authorization PRIVATE libherd)