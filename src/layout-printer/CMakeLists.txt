#===============================================================================
#
# Copyright (C) 2019 Istituto Italiano di Tecnologia (IIT)
#
# This software may be modified and distributed under the terms of the
# GPL-2+ license. See the accompanying LICENSE file for details.
#
#===============================================================================

PROJECT ( layout-printer )

#INCLUDE(${Simox_DIR}/CMakeModules/SimoxMacros.cmake)

#  If using QT5
# find_package(Qt5 REQUIRED
#     Core
#     PrintSupport
#     Gui
#     Widgets
# )

IF(Simox_VISUALIZATION AND Simox_USE_COIN_VISUALIZATION)

	# the variable "demo_SRCS" contains all .cpp files of this project
	FILE(GLOB demo_SRCS ${PROJECT_SOURCE_DIR}/SceneViewer.cpp ${PROJECT_SOURCE_DIR}/showSceneWindow.cpp)
	FILE(GLOB demo_INCS ${PROJECT_SOURCE_DIR}/showSceneWindow.h)

  ################################## moc'ing ##############################
  set(GUI_MOC_HDRS
      ${PROJECT_SOURCE_DIR}/showSceneWindow.h
  )

  set(GUI_UIS
      ${PROJECT_SOURCE_DIR}/SceneViewer.ui
  )

 	# create the executable
  VirtualRobotQtApplication(${PROJECT_NAME} "${demo_SRCS}" "${demo_INCS}" "${GUI_MOC_HDRS}" "${GUI_UIS}")
  SET_TARGET_PROPERTIES(${PROJECT_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
  SET_TARGET_PROPERTIES(${PROJECT_NAME} PROPERTIES FOLDER ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})

  #  If using QT5
  # target_link_libraries(${PROJECT_NAME} Qt5::Widgets Qt5::PrintSupport)

  #######################################################################################
  ############################ Setup for installation ###################################
  #######################################################################################

  # install(TARGETS ${PROJECT_NAME}
  #   # IMPORTANT: Add the library to the "export-set"
  #   EXPORT SimoxTargets
  #   RUNTIME DESTINATION "${INSTALL_BIN_DIR}" COMPONENT bin
  #   COMPONENT dev)

  MESSAGE( STATUS " ** Simox application ${PROJECT_NAME} will be placed into " ${Simox_BIN_DIR})
  MESSAGE( STATUS " ** Simox application ${PROJECT_NAME} will be installed into " ${INSTALL_BIN_DIR})



ENDIF()
