#===============================================================================
#
# Copyright (C) 2019 Istituto Italiano di Tecnologia (IIT)
#
# This software may be modified and distributed under the terms of the
# GPL-2+ license. See the accompanying LICENSE file for details.
#
#===============================================================================

cmake_minimum_required(VERSION 3.5.1)
project(benchmark-manipulation)


# set reference c++ standard
set(CMAKE_CXX_STANDARD 11)

# Default build type to Release
if(NOT CMAKE_CONFIGURATION_TYPES)
    if(NOT CMAKE_BUILD_TYPE)
        message(STATUS "Setting build type to 'Release' as none was specified.")
        set_property(CACHE CMAKE_BUILD_TYPE PROPERTY VALUE "Release")
    endif()
endif()

include(GNUInstallDirs)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/${CMAKE_INSTALL_BINDIR}")

# add Simox to the project.
# Environment variable Simox_DIR must be set to the build root directory of Simox in order for FIND_PACKAGE to work
find_package(Simox REQUIRED)


# Unsure about the following if we don't use visualization
# IF(Simox_USE_COIN_VISUALIZATION)
# 	  FILE(GLOB SRCS ${PROJECT_SOURCE_DIR}/myDemo.cpp ${PROJECT_SOURCE_DIR}/myWindow.cpp)
# 	  FILE(GLOB INCS ${PROJECT_SOURCE_DIR}/myWindow.h)
# 	  set(GUI_MOC_HDRS ${PROJECT_SOURCE_DIR}/myWindow.h)
# 	  set(GUI_UIS ${PROJECT_SOURCE_DIR}/myWindow.ui)
# 	  SimoxQtApplication(${PROJECT_NAME} "${SRCS}" "${INCS}" "${GUI_MOC_HDRS}" "${GUI_UIS}")
# ENDIF()

add_subdirectory(src)
