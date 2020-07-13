#####################
# Pre project setup #
#####################
set(CMAKE_OSX_DEPLOYMENT_TARGET "10.10" CACHE STRING "Minimum OS X deployment version" FORCE)

#################
# Project setup #
#################
cmake_minimum_required(VERSION 3.15)
project(ultralight-java CXX)

#####################
# Set CMake options #
#####################
set(CMAKE_CXX_STANDARD 17)

################################
# Set up 3rdparty dependencies #
################################
# add_subdirectory(3rdparty)

#####################
# Find dependencies #
#####################
find_package(JNI REQUIRED)

#####################
# Configure project #
#####################
if (NOT DEFINED GENERATED_JNI_HEADERS_DIR)
    message(FATAL_ERROR "Please run cmake with -DGENERATED_JNI_HEADERS_DIR=/path/to/generated/headers or simply use "
            "the gradle build task")
endif ()

##########
# Source #
##########
set(ULTRALIGHT_JAVA_INCLUDE_DIR "${CMAKE_CURRENT_LIST_DIR}/include")

set(ULTRALIGHT_JAVA_SOURCE
        src/ultralight_java.cpp)

##############
# Add target #
##############
add_library(ultralight-java SHARED ${ULTRALIGHT_JAVA_SOURCE})
target_link_libraries(ultralight-java PUBLIC ${JNI_LIBRARIES})
target_include_directories(ultralight-java PUBLIC ${ULTRALIGHT_JAVA_INCLUDE_DIR})
target_include_directories(ultralight-java PRIVATE ${JNI_INCLUDE_DIRS} ${GENERATED_JNI_HEADERS_DIR})

#######################
# Set compile options #
#######################
if (NOT MSVC)
    target_compile_options(ultralight-java PRIVATE -fPIC -fstack-protector)
else ()
    set_property(
            TARGET ultralight-java
            PROPERTY MSVC_RUNTIME_LIBRARY "MultiThreadedDebug"
    )
endif ()
