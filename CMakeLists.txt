cmake_minimum_required(VERSION 3.16)

project(mtkLogger
    VERSION   0.1.0
    LANGUAGES CXX
    DESCRIPTION "High-performance, low-latency, zero-allocation C++ logging library"
)

# ── C++ Standard ──────────────────────────────────────────────────────────────
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# ── Qt dependency ─────────────────────────────────────────────────────────────
find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Core)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Core)

# ── Collect sources ───────────────────────────────────────────────────────────

set(MTKLOGGER_PUBLIC_HEADERS
    mtkLoggerNamespaceMacro.h
    mtkLoggerLevel.h
    mtkLoggerMessage.h
    mtkNullMessageLogger.h
    mtkAbstractAppender.h
    mtkAbstractLogger.h
    mtkLoggerManager.h
    Appender/mtkConsoleAppender.h
    Appender/mtkFileAppender.h
    Appender/mtkRollingFileAppender.h
)

set(MTKLOGGER_SOURCES
    mtkLoggerMessage.cpp
    mtkAbstractAppender.cpp
    mtkAbstractLogger.cpp
    mtkLoggerManager.cpp
    Appender/mtkConsoleAppender.cpp
    Appender/mtkFileAppender.cpp
    Appender/mtkRollingFileAppender.cpp
)

# ── Library target ────────────────────────────────────────────────────────────

add_library(mtkLogger ${MTKLOGGER_SOURCES} ${MTKLOGGER_PUBLIC_HEADERS})

# Allow #include "mtkXxx.h" from both root and Appender/ subdirectories
target_include_directories(mtkLogger
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/Appender>
        $<INSTALL_INTERFACE:include/mtkLogger>
        $<INSTALL_INTERFACE:include/mtkLogger/Appender>
)

target_link_libraries(mtkLogger
    PUBLIC
        Qt${QT_VERSION_MAJOR}::Core
)

# ── Build type defaults ───────────────────────────────────────────────────────

if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
    set(CMAKE_BUILD_TYPE Release CACHE STRING "Build type" FORCE)
    set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS
        "Debug" "Release" "MinSizeRel" "RelWithDebInfo")
endif()

# ── Example executable (optional) ─────────────────────────────────────────────

option(MTKLOGGER_BUILD_EXAMPLE "Build the example application" ON)

if(MTKLOGGER_BUILD_EXAMPLE)
    add_executable(mtkLogger_example example_main.cpp)
    target_link_libraries(mtkLogger_example PRIVATE mtkLogger)
endif()

# ── Install rules ─────────────────────────────────────────────────────────────

include(GNUInstallDirs)

install(TARGETS mtkLogger
    EXPORT  mtkLoggerTargets
    LIBRARY  DESTINATION ${CMAKE_INSTALL_LIBDIR}
    ARCHIVE  DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME  DESTINATION ${CMAKE_INSTALL_BINDIR}
)

# Install root-level public headers
install(FILES
    mtkLoggerNamespaceMacro.h
    mtkLoggerLevel.h
    mtkLoggerMessage.h
    mtkNullMessageLogger.h
    mtkAbstractAppender.h
    mtkAbstractLogger.h
    mtkLoggerManager.h
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/mtkLogger
)

# Install Appender headers
install(FILES
    Appender/mtkConsoleAppender.h
    Appender/mtkFileAppender.h
    Appender/mtkRollingFileAppender.h
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/mtkLogger/Appender
)

# ── CMake package config (for find_package) ───────────────────────────────────

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
