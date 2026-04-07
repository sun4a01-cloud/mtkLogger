cmake_minimum_required(VERSION 3.16)

project(mtkLogger
    VERSION   0.1.0
    LANGUAGES CXX
    DESCRIPTION "High-performance, low-latency, zero-allocation C++ logging library"
)

# ──────────────────────────────────────────────────────────────────────────────
# C++ Standard
# ──────────────────────────────────────────────────────────────────────────────
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# ──────────────────────────────────────────────────────────────────────────────
# UTF-8 Source Encoding (Windows MSVC)
# ──────────────────────────────────────────────────────────────────────────────
if(MSVC)
    # Set source file encoding to UTF-8 to avoid C4819 warnings
    add_compile_options(/utf-8)
endif()

# ──────────────────────────────────────────────────────────────────────────────
# Build type defaults
# ──────────────────────────────────────────────────────────────────────────────
if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
    set(CMAKE_BUILD_TYPE Release CACHE STRING "Build type" FORCE)
    set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS
        "Debug" "Release" "MinSizeRel" "RelWithDebInfo")
endif()

# ──────────────────────────────────────────────────────────────────────────────
# Qt dependency
# ──────────────────────────────────────────────────────────────────────────────
# Allow user to specify Qt directory via CMAKE_PREFIX_PATH or QT_DIR env variable
if(DEFINED ENV{QT_DIR})
    list(APPEND CMAKE_PREFIX_PATH "$ENV{QT_DIR}")
endif()

find_package(Qt5 5.12 REQUIRED COMPONENTS Core)

# ──────────────────────────────────────────────────────────────────────────────
# Collect sources
# ──────────────────────────────────────────────────────────────────────────────
set(MTKLOGGER_PUBLIC_HEADERS
    include/mtkLoggerNamespaceMacro.h
    include/mtkLoggerMacro.h
    include/mtkLoggerLevel.h
    include/mtkLoggerMessage.h
    include/mtkLoggerExportMacro.h
    include/mtkNullMessageLogger.h
    include/mtkAbstractAppender.h
    include/mtkAbstractLogger.h
    include/mtkLoggerManager.h
    include/Appender/mtkFileAppender.h
    include/Appender/mtkConsoleAppender.h
    include/Appender/mtkRollingFileAppender.h
)

set(MTKLOGGER_SOURCES
    src/mtkLoggerMessage.cpp
    src/mtkAbstractAppender.cpp
    src/mtkAbstractLogger.cpp
    src/mtkLoggerManager.cpp
    src/Appender/mtkFileAppender.cpp
    src/Appender/mtkConsoleAppender.cpp
    src/Appender/mtkRollingFileAppender.cpp
)

# ──────────────────────────────────────────────────────────────────────────────
# Library target
# ──────────────────────────────────────────────────────────────────────────────
add_library(mtkLogger ${MTKLOGGER_SOURCES} ${MTKLOGGER_PUBLIC_HEADERS})

# Set output name and properties
set_target_properties(mtkLogger PROPERTIES
    OUTPUT_NAME mtkLogger
    VERSION ${PROJECT_VERSION}
    SOVERSION ${PROJECT_VERSION_MAJOR}
    POSITION_INDEPENDENT_CODE ON
)

# Add compile definitions for Windows DLL export/import
if(WIN32)
    target_compile_definitions(mtkLogger PRIVATE
        MTK_LOGGER_EXPORTS
    )
endif()

# Include directories
target_include_directories(mtkLogger
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/Appender>
        $<INSTALL_INTERFACE:include/mtkLogger>
        $<INSTALL_INTERFACE:include/mtkLogger/Appender>
)

# Link Qt
target_link_libraries(mtkLogger
    PUBLIC
        Qt5::Core
)

# ──────────────────────────────────────────────────────────────────────────────
# Example executable (optional)
# ──────────────────────────────────────────────────────────────────────────────
option(MTKLOGGER_BUILD_EXAMPLE "Build the example application" ON)

if(MTKLOGGER_BUILD_EXAMPLE)
    add_executable(mtkLogger_example example/main.cpp)
    target_link_libraries(mtkLogger_example PRIVATE mtkLogger)
endif()

# ──────────────────────────────────────────────────────────────────────────────
# Install rules
# ──────────────────────────────────────────────────────────────────────────────
include(GNUInstallDirs)

install(TARGETS mtkLogger
    EXPORT  mtkLoggerTargets
    LIBRARY  DESTINATION ${CMAKE_INSTALL_LIBDIR}
    ARCHIVE  DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME  DESTINATION ${CMAKE_INSTALL_BINDIR}
)

# Install public headers
install(FILES
    include/mtkLoggerNamespaceMacro.h
    include/mtkLoggerLevel.h
    include/mtkLoggerMessage.h
    include/mtkLoggerExportMacro.h
    include/mtkNullMessageLogger.h
    include/mtkAbstractAppender.h
    include/mtkAbstractLogger.h
    include/mtkLoggerManager.h
    DESTINATION include/mtkLogger
)

# Install Appender headers
install(FILES
    include/Appender/mtkFileAppender.h
    include/Appender/mtkConsoleAppender.h
    include/Appender/mtkRollingFileAppender.h
    DESTINATION include/mtkLogger/Appender
)

# ──────────────────────────────────────────────────────────────────────────────
# CMake package config (for find_package)
# ──────────────────────────────────────────────────────────────────────────────
install(EXPORT mtkLoggerTargets
    FILE        mtkLoggerTargets.cmake
    NAMESPACE   mtkLogger::
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/mtkLogger
)

include(CMakePackageConfigHelpers)

configure_package_config_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/cmake/mtkLoggerConfig.cmake.in"
    "${CMAKE_CURRENT_BINARY_DIR}/mtkLoggerConfig.cmake"
    INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/mtkLogger
)

write_basic_package_version_file(
    "${CMAKE_CURRENT_BINARY_DIR}/mtkLoggerConfigVersion.cmake"
    VERSION ${PROJECT_VERSION}
    COMPATIBILITY SameMajorVersion
)

install(FILES
    "${CMAKE_CURRENT_BINARY_DIR}/mtkLoggerConfig.cmake"
    "${CMAKE_CURRENT_BINARY_DIR}/mtkLoggerConfigVersion.cmake"
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/mtkLogger
)

# ──────────────────────────────────────────────────────────────────────────────
# CPack configuration for creating installers
# ──────────────────────────────────────────────────────────────────────────────
set(CPACK_PACKAGE_NAME "mtkLogger")
set(CPACK_PACKAGE_VENDOR "Sun4a01")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "High-performance C++ logging library")
set(CPACK_PACKAGE_VERSION_MAJOR ${PROJECT_VERSION_MAJOR})
set(CPACK_PACKAGE_VERSION_MINOR ${PROJECT_VERSION_MINOR})
set(CPACK_PACKAGE_VERSION_PATCH ${PROJECT_VERSION_PATCH})
set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_SOURCE_DIR}/LICENSE")
set(CPACK_RESOURCE_FILE_README "${CMAKE_CURRENT_SOURCE_DIR}/README.md")

if(WIN32)
    set(CPACK_GENERATOR "ZIP;NSIS")
else()
    set(CPACK_GENERATOR "TGZ")
endif()

include(CPack)