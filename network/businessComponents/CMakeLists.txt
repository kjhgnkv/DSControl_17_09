cmake_minimum_required(VERSION 3.13)
project(BusinessComponents)

set(CMAKE_CXX_STANDARD 98)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

if(CMAKE_COMPILER_IS_GNUCC)
    add_definitions(-Wno-sign-compare)
endif(CMAKE_COMPILER_IS_GNUCC)

if(NOT CMAKE_DEBUG_POSTFIX)
    set(CMAKE_DEBUG_POSTFIX d)
endif()

include(../cmake/macchina.cmake)
include(../cmake/build.cmake)

set(Macchina_LIBRARIES_RELEASE PocoFoundation PocoUtil)
set(Macchina_LIBRARIES_DEBUG PocoFoundationd PocoUtild)

add_library(DSHostLib INTERFACE)
target_include_directories(DSHostLib INTERFACE
    libraries
    ../DSHost/include
    "${Macchina_INCLUDE_DIRS}"
    )
target_link_directories(DSHostLib INTERFACE
    "${Macchina_LIBRARY_DIRS}"
    )
target_link_libraries(DSHostLib INTERFACE
    optimized "${Macchina_LIBRARIES_RELEASE}"
    debug "${Macchina_LIBRARIES_DEBUG}"
    )
target_compile_options(DSHostLib INTERFACE
    "-std=c++03"
    )
# target_compile_definitions(DSHostLib INTERFACE -DUNICODE)

add_library(BusinessComponentLib INTERFACE)
target_link_libraries(BusinessComponentLib INTERFACE
    DSHostLib
    )

add_custom_target(BusinessComponentsDirectory ALL
    COMMAND ${CMAKE_COMMAND} -E make_directory "${CMAKE_BINARY_DIR}/components"
    VERBATIM
    )

macro(add_bundle_content name)
    add_custom_target(${name}BundleContent ALL
        COMMAND ${CMAKE_COMMAND} -E copy "${CMAKE_CURRENT_SOURCE_DIR}/Bundle.bndlspec" "."
        COMMAND ${CMAKE_COMMAND} -E make_directory "lib"
        COMMAND ${CMAKE_COMMAND} -E copy "$<TARGET_FILE:${name}>" "lib/"
        DEPENDS ${name}
        WORKING_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}"
        VERBATIM
        )
endmacro()

if(WIN32)
    set(Macchina_BUNDLE_UTIL_ARG "/output-dir=${CMAKE_BINARY_DIR}\\components")
else()
    set(Macchina_BUNDLE_UTIL_ARG "--output-dir=${CMAKE_BINARY_DIR}/components")
endif()

macro(add_bundle_file name namedep)
    add_custom_target(${name}Bundle ALL
        COMMAND ${Macchina_BUNDLE_UTIL} ${Macchina_BUNDLE_UTIL_ARG} "Bundle.bndlspec"
        DEPENDS ${namedep}
        WORKING_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}"
        VERBATIM
        )
endmacro()

macro(add_bundle name)
    add_bundle_content(${name})
    add_bundle_file(${name} ${name}BundleContent)
endmacro()

add_subdirectory(DSMonitor)
