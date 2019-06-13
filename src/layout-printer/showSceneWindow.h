
#pragma once

#include <VirtualRobot/VirtualRobot.h>
#include <VirtualRobot/Robot.h>
#include <VirtualRobot/VirtualRobotException.h>
#include <VirtualRobot/Nodes/RobotNode.h>
#include <VirtualRobot/XML/SceneIO.h>
#include <VirtualRobot/Visualization/VisualizationFactory.h>
#include <VirtualRobot/Visualization/CoinVisualization/CoinVisualizationNode.h>
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
    showSceneWindow(std::string& sSceneFile);
    ~showSceneWindow() override;

    /*!< Executes the SoQt mainLoop. You need to call this in order to execute the application. */
    int main();

    std::string scene_name;

public slots:

    //SoOffscreenRenderer* cam2Renderer;
    /*! Closes the window and exits SoQt runloop. */
    void quit();

    /*!< Overriding the close event, so we know when the window was closed by the user. */
    void closeEvent(QCloseEvent* event) override;

    void resetSceneryAll();
    void loadScene();
    void selectScene();

    // void selectRobot(int nr);
    // void selectObject(int nr);
    // void selectGrasp(int nr);
    // void selectEEF(int nr);
    // void selectRobotConfig(int nr);
    // void selectTrajectory(int nr);
    // void sliderMoved(int pos);

    //void closeHand();
    //void openHand();
    //void colModel();
    //void showRoot();

    void exportPDF();

    SoQtExaminerViewer* getExaminerViewer()
    {
        return viewer;
    };

protected:

    void updateGui();
    //void updateGrasps();
    //void updateGraspVisu();
    void setupUI();
    QString formatString(const char* s, float f);
    void buildVisu();

    Ui::MainWindowShowScene UI;
    SoQtExaminerViewer* viewer; /*!< Viewer to display the 3D model of the robot and the environment. */

    SoSeparator* sceneSep;
    SoSeparator* sceneVisuSep;
    SoSeparator* graspVisu;
    SoSeparator* coordVisu;
    SoOffscreenRenderer* cam2Renderer;
    VirtualRobot::GraspPtr currentGrasp;
    VirtualRobot::GraspSetPtr currentGraspSet;
    VirtualRobot::SceneObjectPtr currentObject;
    VirtualRobot::RobotPtr currentRobot;
    VirtualRobot::TrajectoryPtr currentTrajectory;
    VirtualRobot::EndEffectorPtr currentEEF;
    VirtualRobot::RobotNodePtr cam2;

    VirtualRobot::ScenePtr scene;
    std::string sceneFile;


    boost::shared_ptr<VirtualRobot::CoinVisualization> visualization;
};
