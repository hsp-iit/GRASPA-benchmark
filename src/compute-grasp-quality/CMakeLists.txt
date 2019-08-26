PROJECT ( compute-grasp-quality )

IF(Simox_VISUALIZATION AND Simox_USE_COIN_VISUALIZATION AND BUILD_COMPUTE_GRASP_QUALITY)

	# the variable "demo_SRCS" contains all .cpp files of this project
        FILE(GLOB demo_SRCS ${PROJECT_SOURCE_DIR}/SceneViewer.cpp ${PROJECT_SOURCE_DIR}/showSceneWindow.cpp ${PROJECT_SOURCE_DIR}/common.cpp)
        FILE(GLOB demo_INCS ${PROJECT_SOURCE_DIR}/showSceneWindow.h ${PROJECT_SOURCE_DIR}/common.h)

  ################################## moc'ing ##############################
  set(GUI_MOC_HDRS
      ${PROJECT_SOURCE_DIR}/showSceneWindow.h
  )

  set(GUI_UIS
      ${PROJECT_SOURCE_DIR}/SceneViewer.ui
  )

  # create the executable
  SimoxQtApplication(${PROJECT_NAME} "${demo_SRCS}" "${demo_INCS}" "${GUI_MOC_HDRS}" "${GUI_UIS}")
  SET_TARGET_PROPERTIES(${PROJECT_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
  SET_TARGET_PROPERTIES(${PROJECT_NAME} PROPERTIES FOLDER ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})

  #######################################################################################
  ############################ Setup for installation ###################################
  #######################################################################################

  # add the install targets
  install( TARGETS ${PROJECT_NAME} RUNTIME DESTINATION bin)

  MESSAGE( STATUS " ** Simox application ${PROJECT_NAME} will be placed into " ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
  MESSAGE( STATUS " ** Simox application ${PROJECT_NAME} will be installed into " ${CMAKE_INSTALL_PREFIX}/bin)

ENDIF()
