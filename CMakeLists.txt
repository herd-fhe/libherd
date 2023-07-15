cmake_minimum_required(VERSION 3.21)
project(
        libherd
        VERSION 0.1
        DESCRIPTION "herd library"
        LANGUAGES CXX C
)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

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

if(NOT MSVC)
    option(ENABLE_COVERAGE "Enable coverage reporting (gss/clang)" FALSE)

    if(ENABLE_COVERAGE)
        target_compile_options(options_target INTERFACE -fprofile-arcs -ftest-coverage -O1 -g)
        target_link_libraries(options_target INTERFACE -fprofile-arcs -ftest-coverage)
    endif()
endif()

# Dependencies
find_package(PkgConfig REQUIRED)

# External
find_package(OpenFHE REQUIRED)
find_package(libclang REQUIRED)
find_package(LLVM 14 REQUIRED CONFIG)
find_package(Threads REQUIRED)
find_package(OpenMP REQUIRED)

add_subdirectory(third-party/spdlog EXCLUDE_FROM_ALL)
add_subdirectory(third-party/termcolor EXCLUDE_FROM_ALL)

include(FetchContent)

FetchContent_Declare(
        xlscc
        URL https://github.com/herd-fhe/xlscc-builder/releases/download/xlscc-d9c688df88/xlscc-release.tar.gz

        SOURCE_DIR ${CMAKE_BINARY_DIR}/xls
)
FetchContent_MakeAvailable(xlscc)

# Targets

## Herd - proto
add_subdirectory(proto)

add_subdirectory(common)
add_subdirectory(mappers)

## Herd
add_library(libherd)

target_include_directories(libherd PUBLIC include)

target_sources(
        libherd
        PRIVATE

        src/context/context.cpp
        src/context/context_builder.cpp
        src/session/session.cpp

        src/backend/remote/remote_backend.cpp
        src/backend/remote/detail/remote_backend_connection_impl.cpp

        src/crypto/crypto_factory.cpp
        src/crypto/crypto.cpp
        src/crypto/keyring.cpp
        src/crypto/keyset.cpp

        src/crypto/binfhe/binfhe.cpp
        src/crypto/binfhe/detail/crypto.cpp
        src/crypto/binfhe/detail/keyset.cpp
        src/crypto/binfhe/detail/ciphertext.cpp

        src/translator/xlscc/detail/source_analyzer.cpp
        src/translator/xlscc/detail/ir_parser.cpp
        src/translator/xlscc/detail/ir_transpiler.cpp
        src/translator/xlscc/detail/translator.cpp

        src/storage/data_storage.cpp
        src/storage/data_frame.cpp
        src/storage/i_data_row.cpp
        src/storage/import_options.cpp

        src/storage/local/detail/data_storage.cpp
        src/storage/local/detail/data_frame.cpp
        src/storage/local/type_pool_factory.cpp

        src/storage/remote/detail/data_storage.cpp
        src/storage/remote/detail/data_frame.cpp

        src/executor/job.cpp

        src/executor/remote/detail/executor.cpp
        src/executor/remote/detail/job.cpp

        src/utils/csv_reader.cpp
        src/utils/progress_bar.cpp
        src/utils/thread_pool.cpp
        src/utils/temp_storage.cpp
        src/utils/path.cpp
        src/utils/string_utils.cpp

        include/herd/context/context.hpp
        include/herd/context/context_builder.hpp
        include/herd/session/session.hpp

        include/herd/backend/i_backend.hpp
        include/herd/backend/remote/remote_backend.hpp
        include/herd/backend/remote/detail/remote_backend_connection_impl.hpp

        include/herd/translator/i_translator.hpp
        include/herd/translator/exception.hpp

        include/herd/translator/xlscc/detail/source_analyzer.hpp
        include/herd/translator/xlscc/detail/ir_parser.hpp
        include/herd/translator/xlscc/detail/ir_transpiler.hpp
        include/herd/translator/xlscc/detail/compiler.hpp

        include/herd/storage/data_storage.hpp
        include/herd/storage/data_frame.hpp
        include/herd/storage/i_data_row.hpp
        include/herd/storage/import_options.hpp

        include/herd/storage/local/detail/data_storage.hpp
        include/herd/storage/local/detail/data_frame.hpp
        include/herd/storage/local/type_pool.hpp
        include/herd/storage/local/type_pool_factory.hpp

        include/herd/storage/remote/detail/data_storage.hpp
        include/herd/storage/remote/detail/data_frame.hpp

        include/herd/executor/i_executor.hpp
        include/herd/executor/job.hpp

        include/herd/executor/remote/detail/executor.hpp
        include/herd/executor/remote/detail/job.hpp

        include/herd/crypto/i_crypto.hpp
        include/herd/crypto/crypto_config.hpp
        include/herd/crypto/keyset.hpp
        include/herd/crypto/keyring.hpp
        include/herd/crypto/ciphertext.hpp
        include/herd/crypto/crypto_factory.hpp

        include/herd/crypto/binfhe/binfhe.hpp
        include/herd/crypto/binfhe/detail/crypto.hpp
        include/herd/crypto/binfhe/detail/keyset.hpp
        include/herd/crypto/binfhe/detail/ciphertext.hpp

        include/herd/utils/csv_reader.hpp
        include/herd/utils/csv_value_mappers.hpp

        include/herd/utils/thread_pool.hpp
        include/herd/utils/progress_future.hpp
        include/herd/utils/movable_task.hpp
        include/herd/utils/movable_function.hpp
        include/herd/utils/progress_bar.hpp
        include/herd/utils/temp_storage.hpp
        include/herd/utils/path.hpp
        include/herd/utils/string_utils.hpp

        include/herd/utils/pimpl.hpp

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

        herd_proto
)

llvm_map_components_to_libnames(LLVM_LIBS
        coverage
        irreader
        mcparser
        objcarcopts
        option
        passes
        profiledata
        support
)

target_link_libraries(
        libherd
        PUBLIC
        OPENFHEcore
        OPENFHEbinfhe
        termcolor

        herd_common
        herd_dto_mappers

        LLVM-14
        clang-cpp
)


target_include_directories(
        libherd
        SYSTEM PRIVATE
        ${OpenFHE_INCLUDE}
        ${OpenFHE_INCLUDE}/third-party/include
        ${OpenFHE_INCLUDE}/core
        ${OpenFHE_INCLUDE}/pke
        ${OpenFHE_INCLUDE}/binfhe

        ${LLVM_INCLUDE_DIRS}
)

# Test

option(ENABLE_TESTS "Enable tests" FALSE)

if(ENABLE_TESTS)
    add_subdirectory(test)
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

