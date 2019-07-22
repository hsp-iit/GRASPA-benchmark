
#include "showSceneWindow.h"
#include "VirtualRobot/EndEffector/EndEffector.h"
#include "VirtualRobot/Workspace/Reachability.h"
#include "VirtualRobot/ManipulationObject.h"
#include "VirtualRobot/XML/ObjectIO.h"
#include "VirtualRobot/Grasping/GraspSet.h"
#include "VirtualRobot/Visualization/CoinVisualization/CoinVisualizationFactory.h"

#include <QFileDialog>
#include <Eigen/Geometry>

#include <ctime>
#include <vector>
#include <iostream>
#include <cmath>

#include "Inventor/actions/SoLineHighlightRenderAction.h"
#include <Inventor/nodes/SoShapeHints.h>
#include <Inventor/nodes/SoLightModel.h>

#include <sstream>
using namespace std;
using namespace VirtualRobot;

showSceneWindow::showSceneWindow(std::string& scene_filename, std::string& robot_filename, std::string& grasps_path)
    : QMainWindow(nullptr)
{
    VR_INFO << " start " << endl;

    sceneFile = scene_filename;
    sceneSep = new SoSeparator;
    sceneSep->ref();
    sceneVisuSep = new SoSeparator;
    sceneSep->addChild(sceneVisuSep);

    setupUI();

    loadScene();

    viewer->viewAll();
}

showSceneWindow::~showSceneWindow()
{
    sceneSep->unref();
    delete viewer;
}

void showSceneWindow::setupUI()
{
    UI.setupUi(this);
    viewer = new SoQtExaminerViewer(UI.frameViewer, "", TRUE, SoQtExaminerViewer::BUILD_POPUP);

    // setup
    viewer->setBackgroundColor(SbColor(1.0f, 1.0f, 1.0f));
    viewer->setAccumulationBuffer(true);
    viewer->setAntialiasing(true, 4);

    viewer->setGLRenderAction(new SoLineHighlightRenderAction);
    viewer->setTransparencyType(SoGLRenderAction::SORTED_OBJECT_BLEND);
    viewer->setFeedbackVisibility(true);
    viewer->setSceneGraph(sceneSep);
    viewer->viewAll();
    viewer->setAntialiasing(true, 4);

}

void showSceneWindow::closeEvent(QCloseEvent* event)
{
    quit();
    QMainWindow::closeEvent(event);
}

void showSceneWindow::buildVisu()
{
    if (!scene)
    {
        return;
    }

    sceneVisuSep->removeAllChildren();
    SceneObject::VisualizationType visuType = SceneObject::Full;

    visualization = scene->getVisualization<CoinVisualization>(visuType);
    SoNode* visualisationNode = nullptr;

    if (visualization)
    {
        visualisationNode = visualization->getCoinVisualization();
    }

    if (visualisationNode)
    {
        sceneVisuSep->addChild(visualisationNode);
    }

}

int showSceneWindow::main()
{
    SoQt::show(this);
    SoQt::mainLoop();
    return 0;
}

void showSceneWindow::quit()
{
    std::cout << "showSceneWindow: Closing" << std::endl;
    this->close();
    SoQt::exitMainLoop();
}

void showSceneWindow::loadScene()
{
    sceneVisuSep->removeAllChildren();

    cout << "Loading Scene from " << sceneFile << endl;

    scene.reset();

    try
    {
        scene = SceneIO::loadScene(sceneFile);
    }
    catch (VirtualRobotException& e)
    {
        cout << "Could not find valid scene in file " << sceneFile << endl;
        return;
    }

    buildVisu();
    viewer->viewAll();
}
