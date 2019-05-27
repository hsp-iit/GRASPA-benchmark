
#===============================================================================
#
# Copyright (C) 2019 Istituto Italiano di Tecnologia (IIT)
#
# This software may be modified and distributed under the terms of the
# GPL-2+ license. See the accompanying LICENSE file for details.
#
#===============================================================================

set(EXEC_TARGET_NAME compute-grasp-quality)

# Application headers
set(${EXEC_TARGET_NAME}_HEADER
    cgq.h
)

# Application source
set(${EXEC_TARGET_NAME}_SOURCE
    cgq.cpp
)

# The VirtualRobotApplication call is a Simox macro that declares executable targets, sets include directories and links libraries. We do that explicitly
#VirtualRobotApplication( ${EXEC_TARGET_NAME} ${${EXEC_TARGET_NAME}_SOURCE} ${${EXEC_TARGET_NAME}_HEADER})

SimoxApplication( ${EXEC_TARGET_NAME} ${${EXEC_TARGET_NAME}_SOURCE} ${${EXEC_TARGET_NAME}_HEADER})

# Application target calls
#add_executable(${EXEC_TARGET_NAME} ${${EXEC_TARGET_NAME}_HEADER} ${${EXEC_TARGET_NAME}_SOURCE})

# Set up Simox libraries
#setupSimoxExternalLibraries()

# Link external libraries
#target_link_libraries(${EXEC_TARGET_NAME} PRIVATE VirtualRobot ${Simox_EXTERNAL_LIBRARIES})
