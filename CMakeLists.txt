cmake_minimum_required(VERSION 3.21)
project(
        libherd
        VERSION 0.1
        DESCRIPTION "herd library"
        LANGUAGES CXX
)

# Warnings config
add_library(warnings_target INTERFACE)
if(MSVC)
    target_compile_options(
            warnings_target
            INTERFACE
            /W4
            /WX "/permissive-"
    )
else()
    target_compile_options(
            warnings_target
            INTERFACE
            -Wall
            -Wextra
            -Wshadow
            -Wnon-virtual-dtor
            -Wold-style-cast
            -Wcast-align
            -Wdouble-promotion
            -Wunused
            -Woverloaded-virtual
            -Wpedantic
            -Wconversion
            -Wsign-conversion
            -Wnull-dereference
            -Werror
    )
    if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
        target_compile_options(
                warnings_target
                INTERFACE
                -Waddress-of-packed-member
                -Wmisleading-indentation
                -Wduplicated-cond
                -Wduplicated-branches
                -Wlogical-op
                -Wuseless-cast
        )
    endif()
endif()

# Project options config

add_library(options_target INTERFACE)
target_compile_features(options_target INTERFACE cxx_std_20)

if(!MSVC)
    option(ENABLE_COVERAGE "Enable coverage reporting (gss/clang)" FALSE)

    if(ENABLE_COVERAGE)
        target_compile_options(options_target INTERFACE --coverage -O0 -g)
        target_link_libraries(options_targer INTERFACE --coverage)
    endif()
endif()

# Dependencies
find_package(PkgConfig REQUIRED)

# External
find_package(OpenFHE REQUIRED)

find_package(Threads REQUIRED)
find_package(OpenMP REQUIRED)

## UUID
pkg_search_module(UUID REQUIRED uuid)

add_subdirectory(third-party/spdlog EXCLUDE_FROM_ALL)
add_subdirectory(third-party/termcolor EXCLUDE_FROM_ALL)

# Targets

## Herd - proto
add_subdirectory(proto)

add_subdirectory(common)

## Herd
add_library(libherd)

target_include_directories(libherd PUBLIC include)

target_sources(
        libherd
        PRIVATE

        src/context/context.cpp
        src/context/context_builder.cpp
        src/session/session.cpp
        src/tree/circuit_graph.cpp
        src/tree/visitor.cpp

        src/backend/remote/remote_backend.cpp
        src/backend/remote/detail/remote_backend_connection_impl.cpp
        src/backend/remote/detail/mapper.cpp
        src/backend/remote/tree_dump.cpp

        src/crypto/crypto.cpp
        src/crypto/keyring.cpp
        src/crypto/binfhe_keyset.cpp

        src/data_storage/data_storage.cpp
        src/data_storage/data_table.cpp
        src/data_storage/i_data_row.cpp

        src/data_storage/local/local_data_storage.cpp
        src/data_storage/local/local_data_table.cpp
        src/data_storage/local/type_pool_factory.cpp

        src/utils/csv_reader.cpp
        src/utils/thread_pool.cpp
        src/utils/progress_bar.cpp

        src/uuid.cpp

        include/herd/operation.hpp

        include/herd/context/context.hpp
        include/herd/context/context_builder.hpp
        include/herd/session/session.hpp

        include/herd/backend/i_backend.hpp
        include/herd/backend/remote/remote_backend.hpp
        include/herd/backend/remote/detail/remote_backend_connection_impl.hpp
        include/herd/backend/remote/detail/mapper.hpp
        include/herd/backend/remote/tree_dump.hpp

        include/herd/tree/node/abstract_node.hpp
        include/herd/tree/node/input_node.hpp
        include/herd/tree/node/output_node.hpp
        include/herd/tree/node/operation_node.hpp
        include/herd/tree/circuit_graph.hpp
        include/herd/tree/visitor.hpp

        include/herd/data_storage/data_storage.hpp
        include/herd/data_storage/data_table.hpp
        include/herd/data_storage/i_data_row.hpp

        include/herd/data_storage/local/local_data_storage.hpp
        include/herd/data_storage/local/local_data_table.hpp
        include/herd/data_storage/local/type_pool.hpp
        include/herd/data_storage/local/type_pool_factory.hpp

        include/herd/crypto/schema.hpp
        include/herd/crypto/crypto.hpp
        include/herd/crypto/crypto_config.hpp
        include/herd/crypto/i_keyset.hpp
        include/herd/crypto/keyring.hpp
        include/herd/crypto/binfhe_keyset.hpp

        include/herd/type.hpp

        include/herd/utils/csv_reader.hpp
        include/herd/utils/csv_value_mappers.hpp

        include/herd/utils/thread_pool.hpp
        include/herd/utils/progress_future.hpp
        include/herd/utils/movable_task.hpp
        include/herd/utils/progress_bar.hpp

        include/herd/utils/pimpl.hpp

        include/herd/uuid.hpp

        include/herd/herd.hpp
)

target_compile_definitions(
        libherd
        PUBLIC
        MATHBACKEND=2
)

# Dependencies

target_link_libraries(
        libherd
        PRIVATE
        options_target
        warnings_target
)

target_link_libraries(
        libherd
        PRIVATE
        pthread

        OpenMP::OpenMP_CXX
        ${UUID_LIBRARIES}

        herd_proto
)

target_link_libraries(
        libherd
        PUBLIC
        OPENFHEcore
        OPENFHEbinfhe
        termcolor

        herd_common
)

target_include_directories(
        libherd
        SYSTEM PRIVATE
        ${OpenFHE_INCLUDE}
        ${OpenFHE_INCLUDE}/third-party/include
        ${OpenFHE_INCLUDE}/core
        ${OpenFHE_INCLUDE}/pke
        ${OpenFHE_INCLUDE}/binfhe
)

# Test

option(ENABLE_TESTS "Enable tests" FALSE)

if(ENABLE_TESTS)
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
            GTest::gtest GTest::gtest_main
            options_target
            warnings_target
    )

    target_sources(
            libherd_test
            PRIVATE
            test/utils/csv_reader_test.cpp
            test/utils/csv_value_mappers_test.cpp

            test/storage/local/local_data_storage_test.cpp
    )
endif()

option(ENABLE_EXAMPLES "Enable examples building" FALSE)

if(ENABLE_EXAMPLES)
    add_subdirectory(example)
endif()


#Docs
option(ENABLE_DOCS "Enable docs" FALSE)
if(ENABLE_DOCS)
    find_package(Doxygen REQUIRED)
    find_package(Python3 COMPONENTS Interpreter REQUIRED)

    set(DOXYGEN_OUT ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile.out)

    configure_file(docs/Doxyfile ${DOXYGEN_OUT})

    FetchContent_Declare(
            mcss
            GIT_REPOSITORY git://github.com/mosra/m.css
    )
    FetchContent_MakeAvailable(mcss)

    add_custom_target(
            docs
            COMMAND ${Python3_EXECUTABLE} ${mcss_SOURCE_DIR}/documentation/doxygen.py ${DOXYGEN_OUT} --debug
            WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
            COMMENT "Generating API documentation with Doxygen"
            VERBATIM
    )

endif()

