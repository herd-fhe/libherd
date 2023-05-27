find_program(libclang_LLVM_CONFIG_EXECUTABLE NAMES llvm-config-15)

execute_process(COMMAND ${libclang_LLVM_CONFIG_EXECUTABLE} --includedir OUTPUT_VARIABLE libclang_INCLUDE_DIR OUTPUT_STRIP_TRAILING_WHITESPACE)
execute_process(COMMAND ${libclang_LLVM_CONFIG_EXECUTABLE} --libdir OUTPUT_VARIABLE libclang_LIB_DIR OUTPUT_STRIP_TRAILING_WHITESPACE)

find_library(libclang_LIBRARY NAMES clang libclang HINTS ${libclang_LIB_DIR})


include(FindPackageHandleStandardArgs)

find_package_handle_standard_args(
        libclang
        DEFAULT_MSG
        libclang_INCLUDE_DIR
        libclang_LIBRARY
)

if(libclang_FOUND)
    set(libclang_INCLUDE_DIRS ${libclang_INCLUDE_DIR})
    set(libclang_LIBRARIES ${libclang_LIBRARY})
    if(NOT TARGET libclang::libclang)
        add_library(libclang::libclang UNKNOWN IMPORTED)
        set_target_properties(
                libclang::libclang
                PROPERTIES
                INTERFACE_INCLUDE_DIRECTORIES ${libclang_INCLUDE_DIR}
                IMPORTED_LOCATION ${libclang_LIBRARY}
        )
    endif()
endif()