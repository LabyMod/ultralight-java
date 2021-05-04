#####################
# Pre project setup #
#####################
set(CMAKE_OSX_DEPLOYMENT_TARGET "10.10" CACHE STRING "Minimum OS X deployment version" FORCE)

#################
# Project setup #
#################
cmake_minimum_required(VERSION 3.15)
project(ultralight-java-gpu CXX C)
include(CheckSymbolExists)

#####################
# Set CMake options #
#####################
set(CMAKE_CXX_STANDARD 17)

################################
# Set up 3rdparty dependencies #
################################
add_subdirectory(3rdparty)

#####################
# Find dependencies #
#####################
find_package(JNI REQUIRED)

# Check for JNI_VERSION_1_8
# Apple ships an older Java framework, so compilation fails when cross compiling
# Since it doesn't matter what version the library is linked against, only what version is present at runtime,
# we manually define JNI_VERSION_1_8 if it is not defined already
set(CMAKE_REQUIRED_INCLUDES "${JNI_INCLUDE_DIRS}")
check_symbol_exists(JNI_VERSION_1_8 "jni.h" HAS_JNI_VERSION_1_8)

if(NOT HAS_JNI_VERSION_1_8 AND APPLE)
    message(WARNING "JNI_VERSION_1_8 not defined, working around package error in Apple Java SDK")
    add_compile_definitions(JNI_VERSION_1_8=0x00010008)
elseif(NOT HAS_JNI_VERSION_1_8)
    message(SEND_ERROR "JNI version 1.8 not supported")
endif()

##########
# Source #
##########
set(ULTRALIGHT_JAVA_INCLUDE_DIR "${CMAKE_CURRENT_LIST_DIR}/include")

set(ULTRALIGHT_JAVA_SOURCE
        src/gpudriver/gl/glad.c
        src/gpudriver/common/GPUDriverImpl.cpp
        src/gpudriver/gl/GPUContextGL.cpp
        src/gpudriver/gl/GPUDriverGL.cpp

        src/gpudriver/ultralight_java_gpu.cpp)

##############
# Add target #
##############
add_library(ultralight-java-gpu SHARED ${ULTRALIGHT_JAVA_SOURCE})

target_link_libraries(ultralight-java-gpu PUBLIC ${JNI_LIBRARIES} ultralight)
target_include_directories(ultralight-java-gpu PUBLIC ${ULTRALIGHT_JAVA_INCLUDE_DIR})
target_include_directories(ultralight-java-gpu PRIVATE ${JNI_INCLUDE_DIRS} ${GENERATED_JNI_HEADERS_DIR})


#######################
# Set compile options #
#######################
if (NOT MSVC)
    target_compile_options(ultralight-java-gpu PRIVATE -fPIC -fstack-protector)
else ()
    set_property(
            TARGET ultralight-java-gpu
            PROPERTY MSVC_RUNTIME_LIBRARY "MultiThreadedDebug"
    )
endif ()
