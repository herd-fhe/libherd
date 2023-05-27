enable_testing()
find_package(GTest QUIET)

if (NOT GTest_FOUND)
    include(FetchContent)
    FetchContent_Declare(
            googletest
            GIT_REPOSITORY https://github.com/google/googletest.git
            GIT_TAG        release-1.12.1
    )

    FetchContent_GetProperties(googletest)
    if(NOT googletest_POPULATED)
        FetchContent_Populate(googletest)
        set(CMAKE_SUPPRESS_DEVELOPER_WARNINGS 1 CACHE BOOL "")
        add_subdirectory(${googletest_SOURCE_DIR} ${googletest_BINARY_DIR} EXCLUDE_FROM_ALL)
        unset(CMAKE_SUPPRESS_DEVELOPER_WARNINGS)
    endif()
endif()

add_executable(libherd_test "")
target_link_libraries(
        libherd_test
        libherd
        GTest::gtest GTest::gmock GTest::gtest_main
        options_target
        warnings_target
)

target_sources(
        libherd_test
        PRIVATE
        utils/csv_reader_test.cpp
        utils/csv_value_mappers_test.cpp
        utils/progress_bar_test.cpp

        translator/xlscc/translator_test.cpp
        translator/xlscc/ir_parser_test.cpp
        translator/xlscc/ir_transpiler_test.cpp

        storage/local/local_data_storage_test.cpp
)
