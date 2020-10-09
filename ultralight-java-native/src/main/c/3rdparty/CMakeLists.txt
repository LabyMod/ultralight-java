################################
# Set up 3rdparty dependencies #
################################

# Set the architecture information for ultralight
if (CMAKE_SIZEOF_VOID_P EQUAL 8)
    set(ULTRALIGHT_ARCH "x64")
elseif (CMAKE_SIZEOF_VOID_P EQUAL 4)
    set(ULTRALIGHT_ARCH "x86")
else ()
    message(FATAL_ERROR "Unsupported pointer size ${CMAKE_SIZEOF_VOID_P}")
endif ()

# Set the OS information for ultralight
if (WIN32)
    set(ULTRALIGHT_OS_NAME "win")
    set(ULTRALIGHT_LINK_DIRECTORY "lib")
elseif (APPLE)
    set(ULTRALIGHT_OS_NAME "mac")
    set(ULTRALIGHT_LINK_DIRECTORY "bin")
elseif (UNIX)
    set(ULTRALIGHT_OS_NAME "linux")
    set(ULTRALIGHT_LINK_DIRECTORY "bin")
else ()
    message(FATAL_ERROR "Unsupported operating system")
endif ()

# Ultralight
set(ULTRALIGHT_DIR "${CMAKE_CURRENT_BINARY_DIR}/ultralight-${ULTRALIGHT_OS_NAME}-${ULTRALIGHT_ARCH}")
set(ULTRALIGHT_ARCHIVE "${ULTRALIGHT_DIR}/ultralight.7z")
# d1b164895859be1fa3dadf4caf0ef017ef21e5a5
# Fix bug with Bitmap::WritePNG ignoring internal row bytes configuration.
#
# https://github.com/ultralight-ux/Ultralight/commit/d1b164895859be1fa3dadf4caf0ef017ef21e5a5
set(ULTRALIGHT_VERSION "d1b1648")

# Check the version of ultralight
set(ULTRALIGHT_VERSION_FILE "${ULTRALIGHT_DIR}/.version")
if (EXISTS "${ULTRALIGHT_VERSION_FILE}")
    # The version file exists, read it
    file(READ "${ULTRALIGHT_VERSION_FILE}" ULTRALIGHT_INSTALLED_VERSION)
endif ()

# Check if the installed version matches the target version
if (NOT "${ULTRALIGHT_VERSION}" STREQUAL "${ULTRALIGHT_INSTALLED_VERSION}")
    # Versions do not match, erase the installed version
    file(REMOVE_RECURSE "${ULTRALIGHT_DIR}")
endif ()

# Check if the ultralight directory exists
if (NOT EXISTS "${ULTRALIGHT_DIR}")
    # It does not, download ultralight and extract it
    file(DOWNLOAD
            "https://ultralight-sdk.sfo2.cdn.digitaloceanspaces.com/ultralight-sdk-${ULTRALIGHT_VERSION}-${ULTRALIGHT_OS_NAME}-${ULTRALIGHT_ARCH}.7z"
            "${ULTRALIGHT_ARCHIVE}"
            SHOW_PROGRESS
            STATUS ULTRALIGHT_DOWNLOAD_STATUS
            LOG ULTRALIGHT_DOWNLOAD_LOG
            )

    # Extract the status from the download return value
    list(GET ULTRALIGHT_DOWNLOAD_STATUS 0 ULTRALIGHT_DOWNLOAD_ERROR_CODE)
    list(GET ULTRALIGHT_DOWNLOAD_STATUS 1 ULTRALIGHT_DOWNLOAD_ERROR_MESSAGE)

    # Check the download status
    if(NOT ULTRALIGHT_DOWNLOAD_ERROR_CODE EQUAL 0)
        # Download failed
        message(SEND_ERROR "Failed to download Ultralight: ${ULTRALIGHT_DOWNLOAD_ERROR_MESSAGE}\n\n${ULTRALIGHT_DOWNLOAD_LOG}")
    endif()

    # Extract it
    execute_process(
            COMMAND "${CMAKE_COMMAND}" -E tar xvf "${ULTRALIGHT_ARCHIVE}"
            WORKING_DIRECTORY "${ULTRALIGHT_DIR}"
            RESULT_VARIABLE EXTRACT_ERROR
    )

    if(EXTRACT_ERROR)
        message(FATAL_ERROR "Failed to extract the ultralight zip archive")
    endif()

    # Delete the archive file
    file(REMOVE "${ULTRALIGHT_ARCHIVE}")

    # Write the version file
    file(WRITE "${ULTRALIGHT_VERSION_FILE}" "${ULTRALIGHT_VERSION}")
endif ()

# Add the ultralight target
add_library(ultralight INTERFACE)
target_include_directories(ultralight INTERFACE "${ULTRALIGHT_DIR}/include")
target_link_directories(ultralight INTERFACE "${ULTRALIGHT_DIR}/${ULTRALIGHT_LINK_DIRECTORY}")
target_link_libraries(ultralight INTERFACE AppCore Ultralight UltralightCore WebCore)
