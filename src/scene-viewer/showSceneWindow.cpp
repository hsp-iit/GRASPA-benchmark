
#include "showSceneWindow.h"
#include "VirtualRobot/EndEffector/EndEffector.h"
#include "VirtualRobot/Workspace/Reachability.h"
#include "VirtualRobot/ManipulationObject.h"
#include "VirtualRobot/XML/ObjectIO.h"
#include "VirtualRobot/Grasping/GraspSet.h"
#include "VirtualRobot/Visualization/CoinVisualization/CoinVisualizationFactory.h"

#include <QFileDialog>
#include <Eigen/Geometry>

#include <string>
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

float TIMER_MS = 30.0f;

showSceneWindow::showSceneWindow(std::string& sSceneFile)
    : QMainWindow(nullptr)
{
    VR_INFO << " start " << endl;
    sceneFile = sSceneFile;
    sceneSep = new SoSeparator;
    sceneSep->ref();
    sceneVisuSep = new SoSeparator;
    sceneSep->addChild(sceneVisuSep);
    graspVisu = new SoSeparator;
    sceneSep->addChild(graspVisu);
    coordVisu = new SoSeparator;
    sceneSep->addChild(coordVisu);
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

    viewer->setBackgroundColor(SbColor(1.0f, 1.0f, 1.0f));
    viewer->setAccumulationBuffer(true);
    viewer->setAntialiasing(true, 4);

    viewer->setGLRenderAction(new SoLineHighlightRenderAction);
    viewer->setTransparencyType(SoGLRenderAction::SORTED_OBJECT_BLEND);
    viewer->setFeedbackVisibility(true);
    viewer->setSceneGraph(sceneSep);
    viewer->viewAll();
    viewer->setAntialiasing(true, 4);
    connect(UI.pushButtonLoad, SIGNAL(clicked()), this, SLOT(selectScene()));
}

QString showSceneWindow::formatString(const char* s, float f)
{
    QString str1(s);

    if (f >= 0)
    {
        str1 += " ";
    }

    if (fabs(f) < 1000)
    {
        str1 += " ";
    }

    if (fabs(f) < 100)
    {
        str1 += " ";
    }

    if (fabs(f) < 10)
    {
        str1 += " ";
    }

    QString str1n;
    str1n.setNum(f, 'f', 3);
    str1 = str1 + str1n;
    return str1;
}


void showSceneWindow::resetSceneryAll()
{
    updateGui();
    buildVisu();
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

    coordVisu->removeAllChildren();

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

void showSceneWindow::selectScene()
{
    QString fi = QFileDialog::getOpenFileName(this, tr("Open Scene File"), QString(), tr("XML Files (*.xml)"));
    sceneFile = std::string(fi.toLatin1());

    viewer = new SoQtExaminerViewer(UI.frameViewer, "", TRUE, SoQtExaminerViewer::BUILD_POPUP);

    viewer->setBackgroundColor(SbColor(1.0f, 1.0f, 1.0f));
    viewer->setAccumulationBuffer(true);
    viewer->setAntialiasing(true, 4);

    viewer->setGLRenderAction(new SoLineHighlightRenderAction);
    viewer->setTransparencyType(SoGLRenderAction::SORTED_OBJECT_BLEND);
    viewer->setFeedbackVisibility(true);
    viewer->setSceneGraph(sceneSep);
    viewer->viewAll();
    viewer->setAntialiasing(true, 4);

    loadScene();

    viewer->viewAll();
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
    }

    scene_name =scene->getName();

    if (!scene)
    {
        try
        {

            ManipulationObjectPtr mo = ObjectIO::loadManipulationObject(sceneFile);

            if (mo)
            {
                VR_INFO << "Loaded Manipulation object:" << endl;
                mo->print();
                scene.reset(new Scene(mo->getName()));
                scene->registerManipulationObject(mo);
            }
        }
        catch (VirtualRobotException& e)
        {
            cout << "Could not find valid manipulation object in file " << sceneFile << endl;
        }
    }

    if (!scene)
    {
        try
        {

            ObstaclePtr mo = ObjectIO::loadObstacle(sceneFile);

            if (mo)
            {
                VR_INFO << "Loaded obstacle:" << endl;
                mo->print();
                scene.reset(new Scene(mo->getName()));
                scene->registerObstacle(mo);
            }
        }
        catch (VirtualRobotException& e)
        {
            cout << "Could not find valid obstacle in file " << sceneFile << endl;
        }
    }

    if (!scene)
    {
        cout << " ERROR while creating scene" << endl;
        return;
    }
    updateGui();
    buildVisu();
    viewer->viewAll();
}

void showSceneWindow::updateGui()
{
    currentRobot.reset();

    if (!scene)
    {
        return;
    }
}
