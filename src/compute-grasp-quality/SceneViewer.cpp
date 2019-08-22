#include <VirtualRobot/Robot.h>
#include <VirtualRobot/VirtualRobotException.h>
#include <VirtualRobot/Nodes/RobotNode.h>
#include <VirtualRobot/XML/RobotIO.h>
#include <VirtualRobot/Visualization/VisualizationFactory.h>
#include <VirtualRobot/Visualization/CoinVisualization/CoinVisualization.h>
#include <VirtualRobot/RuntimeEnvironment.h>

#include <Inventor/Qt/viewers/SoQtExaminerViewer.h>
#include <Inventor/nodes/SoSeparator.h>
#include <Inventor/Qt/SoQt.h>

#include <string>
#include <iostream>

using namespace VirtualRobot;

#include <Eigen/Core>
#include <Eigen/Geometry>

#include "showSceneWindow.h"
#include "common.h"

int main(int argc, char* argv[])
{
    VirtualRobot::init(argc, argv, "Compute grasp quality, with visualization");

    cout << " --- START --- " << endl;

    VirtualRobot::RuntimeEnvironment::considerKey("scene");                 //  pointing to the layout xml
    VirtualRobot::RuntimeEnvironment::considerKey("datapath");              //  benchmark data path (default: repo root)
    VirtualRobot::RuntimeEnvironment::considerKey("robot");                 //  robot xml, from datapath folder
    VirtualRobot::RuntimeEnvironment::considerKey("graspsdir");              //  path to user-defined grasps
    VirtualRobot::RuntimeEnvironment::considerKey("approach");              //  if approach movement is to consider
    VirtualRobot::RuntimeEnvironment::processCommandLine(argc, argv);
    VirtualRobot::RuntimeEnvironment::print();

    //  Add scene/objects data path

    std::string data_path("../../");
    common::parseDataPath(data_path);

    //  Find the scene file; if not found, use a default

    std::string scene_filename("data/scenes/grasping/3D_scenes/layout_0.xml");
    common::parseSceneFilename(scene_filename);

    //  Find the robot; if not found, use a default

    std::string robot_filename("robots/iCub/iCub.xml");
    common::parseRobotFilename(robot_filename);

    //  Find the user-defined grasps path
    //  this assumes there is a directory with the same name as the layout
    //  under grasps_path
    //  grasps_path
    //      |
    //      layout_0
    //          |
    //          *.xml
    //      |
    //      layout_1
    //          |
    //      ...

    std::string grasps_path;
    common::parseGraspPath(grasps_path);
    grasps_path = grasps_path + "/" + common::fileBaseName(scene_filename);
    VirtualRobot::RuntimeEnvironment::addDataPath(grasps_path);

    //  Print some debug statements

    std::cout << std::endl << "Parsing complete!" << std::endl;
    std::cout << "Using scene: \t\t" << scene_filename << std::endl;
    std::cout << "Using robot: \t\t" << robot_filename << std::endl;
    std::cout << "Parsing grasps from: \t" << grasps_path << std::endl;

    showSceneWindow rw(scene_filename, robot_filename, grasps_path);

    rw.main();

    return 0;

}
