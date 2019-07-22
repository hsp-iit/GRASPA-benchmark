#===============================================================================
#
# Copyright (C) 2019 Istituto Italiano di Tecnologia (IIT)
#
# This software may be modified and distributed under the terms of the
# GPL-2+ license. See the accompanying LICENSE file for details.
#
#===============================================================================

# Add option to enable build of different modules
option(BUILD_COMPUTE_GRASP_QUALITY "Build executable to compute grasp quality" ON)
if (BUILD_COMPUTE_GRASP_QUALITY)
    add_subdirectory(compute-grasp-quality)
    add_subdirectory(compute-grasp-quality-with-visu)
endif()

option(BUILD_SCENE_VIEWER "Build executable to view scenes" ON)
if (BUILD_SCENE_VIEWER)
    add_subdirectory(scene-viewer)
endif()
