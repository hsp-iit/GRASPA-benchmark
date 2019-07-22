
#pragma once

#include <VirtualRobot/VirtualRobot.h>
#include <VirtualRobot/Robot.h>
#include <VirtualRobot/VirtualRobotException.h>
#include <VirtualRobot/Nodes/RobotNode.h>
#include <VirtualRobot/XML/SceneIO.h>
#include <VirtualRobot/Visualization/VisualizationFactory.h>
#include <VirtualRobot/Visualization/CoinVisualization/CoinVisualization.h>
#include <VirtualRobot/Obstacle.h>
#include <string.h>
#include <QtCore/QtGlobal>
#include <QtGui/QtGui>
#include <QtCore/QtCore>

#include <Inventor/sensors/SoTimerSensor.h>
#include <Inventor/nodes/SoEventCallback.h>
#include <Inventor/Qt/viewers/SoQtExaminerViewer.h>
#include <Inventor/Qt/SoQt.h>
#include <Inventor/nodes/SoSeparator.h>


#include <vector>

#include "ui_SceneViewer.h"

class showSceneWindow : public QMainWindow
{
    Q_OBJECT
public:
    showSceneWindow(std::string& scene_filename, std::string& robot_filename, std::string& grasps_path);
    ~showSceneWindow() override;

    /*!< Executes the SoQt mainLoop. You need to call this in order to execute the application. */
    int main();

public slots:
    /*! Closes the window and exits SoQt runloop. */
    void quit();

    /*!< Overriding the close event, so we know when the window was closed by the user. */
    void closeEvent(QCloseEvent* event) override;

    void loadScene();

    void loadRobot();

    void loadObjectsGrasps();

    SoQtExaminerViewer* getExaminerViewer()
    {
        return viewer;
    }

protected:

    void updateGui();
    void updateGrasps();
    void updateGraspVisu();
    void setupUI();
    QString formatString(const char* s, float f);
    void buildVisu();

    Ui::MainWindowShowScene UI;
    SoQtExaminerViewer* viewer; /*!< Viewer to display the 3D model of the robot and the environment. */

    SoSeparator* sceneSep;
    SoSeparator* sceneVisuSep;
    SoSeparator* robotSep;
    SoSeparator* objectSep;

    SoSeparator* eefVisu;


    VirtualRobot::GraspPtr currentGrasp;
    VirtualRobot::GraspSetPtr currentGraspSet;
    VirtualRobot::RobotPtr robot;
    VirtualRobot::EndEffectorPtr currentEEF;
    VirtualRobot::RobotPtr eefCloned;

    VirtualRobot::ScenePtr scene;

    std::vector< VirtualRobot::ManipulationObjectPtr > objects_in_scene;
    std::vector< VirtualRobot::ManipulationObjectPtr > objects_in_scene_with_grasps;

    std::string sceneFile;
    std::string robotFile;
    std::string graspsPath;


    boost::shared_ptr<VirtualRobot::CoinVisualization> visualization;
    boost::shared_ptr<VirtualRobot::CoinVisualization> visualizationRobot;
};

