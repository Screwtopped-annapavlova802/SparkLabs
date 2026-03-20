cmake_minimum_required(VERSION 3.15)
project(SparkLabs VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(DEFAULT_BUILD_TYPE "Release")

if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
    set(CMAKE_BUILD_TYPE "${DEFAULT_BUILD_TYPE}")
endif()

string(TOUPPER "${CMAKE_BUILD_TYPE}" CMAKE_BUILD_TYPE_UPPER)

if(CMAKE_BUILD_TYPE_UPPER MATCHES DEBUG)
    add_compile_options(-Wall -Wextra -Wpedantic -g -O0)
else()
    add_compile_options(-Wall -Wextra -Wpedantic -O3)
endif()

if(MSVC)
    add_compile_options(/W4 /WX)
    add_compile_definitions(_CRT_SECURE_NO_WARNINGS SPARKLABS_PLATFORM_WINDOWS)
elseif(CMAKE_CXX_COMPILER_ID MATCHES GNU|Clang)
    add_compile_options(-Wall -Wextra -Wpedantic)
    add_compile_definitions(SPARKLABS_PLATFORM_UNIX)
endif()

option(SPARKLABS_ORT_ENABLED "Enable ONNX Runtime support" ON)
option(SPARKLABS_GPU_SUPPORT "Enable GPU acceleration" ON)

find_package(Threads REQUIRED)

add_subdirectory(core)
add_subdirectory(engine)
add_subdirectory(sparkai)
add_subdirectory(render)
add_subdirectory(platform)
add_subdirectory(scripts)
add_subdirectory(tests)

set_target_properties(${PROJECT_NAME} PROPERTIES
    DEBUG_OUTPUT_NAME "${PROJECT_NAME}_d"
    RELEASE_OUTPUT_NAME "${PROJECT_NAME}"
)

add_executable(${PROJECT_NAME} main.cpp)

target_link_libraries(${PROJECT_NAME} PRIVATE
    SparkLabs::Core
    SparkLabs::Engine
    SparkLabs::sparkai
    SparkLabs::Render
    SparkLabs::Platform
    Threads::Threads
)

if(SPARKLABS_ORT_ENABLED)
    target_compile_definitions(${PROJECT_NAME} PRIVATE SPARKLABS_ORT_ENABLED)
endif()

enable_testing()
add_subdirectory(tests)
