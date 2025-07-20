cmake_minimum_required(VERSION 3.21)
project(throwEngine)

# Auto-detect or download vcpkg
if(NOT DEFINED CMAKE_TOOLCHAIN_FILE AND NOT DEFINED ENV{CMAKE_TOOLCHAIN_FILE})
    if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/vcpkg/scripts/buildsystems/vcpkg.cmake")
        set(CMAKE_TOOLCHAIN_FILE "${CMAKE_CURRENT_SOURCE_DIR}/vcpkg/scripts/buildsystems/vcpkg.cmake")
    elseif(DEFINED ENV{VCPKG_ROOT})
        set(CMAKE_TOOLCHAIN_FILE "$ENV{VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake")
    else()
        message(STATUS "Downloading vcpkg...")
        include(FetchContent)
        FetchContent_Declare(
            vcpkg
            GIT_REPOSITORY https://github.com/microsoft/vcpkg.git
            GIT_TAG master
        )
        FetchContent_MakeAvailable(vcpkg)
        set(CMAKE_TOOLCHAIN_FILE "${vcpkg_SOURCE_DIR}/scripts/buildsystems/vcpkg.cmake")
    endif()
endif()

set(VCPKG_MANIFEST_MODE ON)

# Engine configuration
add_subdirectory(engine)
add_subdirectory(game)
