cmake_minimum_required(VERSION 3.20)

project(Reflection VERSION 0.1.0 LANGUAGES CXX)

include(GNUInstallDirs)

option(REFLECTION_BUILD_TESTS "Build test executable" ON)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if(MSVC)
    # Match xmake default on Windows: /MD
    cmake_policy(SET CMP0091 NEW)
    set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreadedDLL")
endif()

# Header-only library target
add_library(reflection INTERFACE)
add_library(reflection::reflection ALIAS reflection)

target_include_directories(reflection
    INTERFACE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)

target_compile_features(reflection INTERFACE cxx_std_23)

if(WIN32)
    target_compile_definitions(reflection INTERFACE NOMINMAX UNICODE)
endif()

if(MSVC)
    target_compile_options(reflection INTERFACE /utf-8 /W4)
elseif(CMAKE_CXX_COMPILER_ID MATCHES "Clang|GNU")
    target_compile_options(reflection INTERFACE -Wall -Wextra)
endif()

if(NOT MSVC)
    target_compile_options(reflection INTERFACE -fvisibility=hidden)
endif()

include(FetchContent)
# Disable JSONC C API in this project; only C++ API is needed.
set(JSONC_BUILD_C_API OFF CACHE BOOL "Disable JSONC C API" FORCE)
FetchContent_Declare(
    jsonc
    GIT_REPOSITORY https://github.com/SculkCatalystMC/JSONC.git
    GIT_TAG v1.4.0
)
FetchContent_Declare(
    boost_pfr
    GIT_REPOSITORY https://github.com/boostorg/pfr.git
    GIT_TAG 2.2.0
)
FetchContent_Declare(
    magic_enum
    GIT_REPOSITORY https://github.com/Neargye/magic_enum.git
    GIT_TAG v0.9.7
)
FetchContent_MakeAvailable(jsonc boost_pfr magic_enum)

if(TARGET magic_enum::magic_enum)
    target_link_libraries(reflection INTERFACE $<BUILD_INTERFACE:magic_enum::magic_enum>)
endif()

if(TARGET boost_pfr)
    target_link_libraries(reflection INTERFACE $<BUILD_INTERFACE:boost_pfr>)
elseif(TARGET Boost::pfr)
    target_link_libraries(reflection INTERFACE $<BUILD_INTERFACE:Boost::pfr>)
endif()

if(TARGET jsonc::jsonc)
    target_link_libraries(reflection INTERFACE $<BUILD_INTERFACE:jsonc::jsonc>)
elseif(TARGET jsonc)
    target_link_libraries(reflection INTERFACE $<BUILD_INTERFACE:jsonc>)
endif()

if(NOT TARGET magic_enum::magic_enum)
    message(FATAL_ERROR "magic_enum fetch failed or target is unavailable.")
endif()

if(NOT TARGET boost_pfr AND NOT TARGET Boost::pfr)
    message(FATAL_ERROR "boost_pfr fetch failed or target is unavailable.")
endif()

if(NOT TARGET jsonc::jsonc AND NOT TARGET jsonc)
    message(FATAL_ERROR "jsonc fetch failed or target is unavailable.")
endif()

if(REFLECTION_BUILD_TESTS)
    add_executable(test
        test/main.cpp
        test/test_jsonc.cpp
    )

    target_include_directories(test PRIVATE test)
    target_link_libraries(test PRIVATE reflection::reflection)

    if(MSVC)
        target_compile_options(test PRIVATE /EHsc)
    endif()

    if(MSVC)
        target_compile_options(test PRIVATE $<$<CONFIG:Release>:/O2>)
    else()
        target_compile_options(test PRIVATE $<$<CONFIG:Release>:-O3>)
    endif()
endif()

install(TARGETS reflection EXPORT reflectionTargets)

install(DIRECTORY include/
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

install(EXPORT reflectionTargets
    FILE reflectionTargets.cmake
    NAMESPACE reflection::
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/reflection
)

