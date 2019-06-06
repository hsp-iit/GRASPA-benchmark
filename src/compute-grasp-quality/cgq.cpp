#include "cgq.h"
#include <iostream>

#include <VirtualRobot/VirtualRobot.h>
#include <VirtualRobot/Robot.h>
#include <VirtualRobot/ManipulationObject.h>
#include <VirtualRobot/RuntimeEnvironment.h>
#include <VirtualRobot/XML/RobotIO.h>
#include <VirtualRobot/XML/ObjectIO.h>
#include <VirtualRobot/Grasping/Grasp.h>
#include <VirtualRobot/Grasping/GraspSet.h>
#include <VirtualRobot/EndEffector/EndEffector.h>
#include <GraspPlanning/GraspStudio.h>
#include <GraspPlanning/GraspQuality/GraspQualityMeasureWrenchSpace.h>
#include <GraspPlanning/ApproachMovementSurfaceNormal.h>
#include <GraspPlanning/GraspQuality/GraspEvaluationPoseUncertainty.h>

void parseDataPath()
{
    //  add datapath to search path

    std::string data_path = VirtualRobot::RuntimeEnvironment::getValue("datapath");
    if (!data_path.empty())
    {
        VirtualRobot::RuntimeEnvironment::addDataPath(data_path);
    }

    return;
}

bool parseSceneFilename(std::string& scene_filename)
{
    //  replace the scene filename if another is found in the search directory

    std::string scene_cmd_line_filename = VirtualRobot::RuntimeEnvironment::getValue("scene");
    if (!scene_cmd_line_filename.empty() && VirtualRobot::RuntimeEnvironment::getDataFileAbsolute(scene_cmd_line_filename))
    {
        scene_filename = scene_cmd_line_filename;
        return true;
    }
    else
    {
        return false;
    }

}

bool parseRobotFilename(std::string& robot_filename)
{
    //  replace robot filename if another is found in the search directory

    std::string robot_cmd_line_filename = VirtualRobot::RuntimeEnvironment::getValue("robot");
    if (!robot_cmd_line_filename.empty() && VirtualRobot::RuntimeEnvironment::getDataFileAbsolute(robot_cmd_line_filename))
    {
        robot_filename = robot_cmd_line_filename;
        return true;
    }
    else
    {
        return false;
    }

}

bool parseObjectFilename(std::string& object_filename)
{
    //  replace object filename if another is found in the search directory

    std::string object_cmd_line_filename = VirtualRobot::RuntimeEnvironment::getValue("object");
    if (!object_cmd_line_filename.empty() && VirtualRobot::RuntimeEnvironment::getDataFileAbsolute(object_cmd_line_filename))
    {
        object_filename = object_cmd_line_filename;
        return true;
    }
    else
    {
        return false;
    }
}

void listEndEffectors(const std::vector<VirtualRobot::EndEffectorPtr> robotEndEffectors)
{
    //  list any end effector in the provided vector

    std::cout << "Robot end effectors found: " << std::endl;

    for (auto eef_ptr : robotEndEffectors)
    {
        std::cout << "\t" << eef_ptr->getName() << std::endl;
        std::cout << "\tRegistered preshapes: " << std::endl;

        std::vector<std::string> eef_preshapes = eef_ptr->getPreshapes();

        for (auto pre_name : eef_preshapes)
        {
            std::cout << "\t\t" << pre_name << std::endl;
        }
    }

    return;
}

void listGrasps(const std::vector<VirtualRobot::GraspPtr> grasp_vec)
{
    //  display info about grasps

    std::cout << "Retrieved grasps: " << grasp_vec.size() << std::endl;

    for (auto grasp_ptr : grasp_vec)
    {
        std::cout << "\tEnd Effector: \t"   << grasp_ptr->getEefName() << std::endl;
        std::cout << "\tPreshape: \t\t"       << grasp_ptr->getPreshapeName() << std::endl;
        std::cout << "\tQuality: \t\t"        << grasp_ptr->getQuality() << std::endl;
    }

    return;

}

int main(int argc, char* argv[])
{

    VirtualRobot::init(argc, argv, "Compute Grasp Quality");
    std::cout << " --- START --- " << std::endl;

    //  parse robot/object filenames if flags exist as arguments

    VirtualRobot::RuntimeEnvironment::considerKey("scene");
    VirtualRobot::RuntimeEnvironment::considerKey("robot");
    VirtualRobot::RuntimeEnvironment::considerKey("object");
    VirtualRobot::RuntimeEnvironment::considerKey("eef");
    //VirtualRobot::RuntimeEnvironment::considerKey("preshape");
    VirtualRobot::RuntimeEnvironment::considerKey("datapath");
    VirtualRobot::RuntimeEnvironment::processCommandLine(argc, argv);
    VirtualRobot::RuntimeEnvironment::print();

    //  add command line data path

    parseDataPath();

    //  load default scene
    //  relative to data directory

    std::string scene_filename("scenes/grasping/scene1.xml");
    VirtualRobot::RuntimeEnvironment::getDataFileAbsolute(scene_filename);

    parseSceneFilename(scene_filename);

    std::cout << "Using scene: \t" << scene_filename << std::endl;

    //  load default setup
    //  robot filename is relative to the data directory

    std::string robot_filename("robots/ArmarIII/ArmarIII.xml");
    VirtualRobot::RuntimeEnvironment::getDataFileAbsolute(robot_filename);

    //  parse robot path and assign it

    parseRobotFilename(robot_filename);

    std::cout << "Using robot: \t" << robot_filename << std::endl;

    //  load default object
    //  object filename is relative to the data directory

    std::string object_filename("grasps/WaterBottleWithGrasps.xml");
    VirtualRobot::RuntimeEnvironment::getDataFileAbsolute(object_filename);

    //  parse manipulation object

    parseObjectFilename(object_filename);

    std::cout << "Using object: \t" << object_filename << std::endl;

    //  try loading the scene

    VirtualRobot::ScenePtr scene;

    try
    {
        scene = VirtualRobot::SceneIO::loadScene(scene_filename);
    }
    catch (VirtualRobot::VirtualRobotException &exc)
    {
        std::cout << "Error: " << exc.what() << std::endl;
    }


    //  DEBUG: list stuff in the scene
    //  TODO: there should be a grasp set or a grasp named after the scene,
    //  so we open a scene -> look for ManipulationObjects in the scene -> parse grasps for that scene and leave others.


    std::vector< VirtualRobot::ManipulationObjectPtr > objects_in_scene = scene->getManipulationObjects();

    std::vector< VirtualRobot::ManipulationObjectPtr > objects_in_scene_with_grasps;

    for (auto & obj : objects_in_scene)
    {
        std::cout << "Object: " << obj->getName() << std::endl;
        std::cout << obj->getGlobalPose() << std::endl;
        //  look for a filename in data/grasps with a specific name
        std::string man_obj_filename = "grasps/" + obj->getName() + "_grasp.xml";
        VirtualRobot::RuntimeEnvironment::getDataFileAbsolute(man_obj_filename);
        VirtualRobot::ManipulationObjectPtr man_obj;
        try
        {
            man_obj = VirtualRobot::ObjectIO::loadManipulationObject(man_obj_filename);
        }
        catch (VirtualRobot::VirtualRobotException &exc)
        {
            std::cout << "Error: " << exc.what() << std::endl;
            continue;
        }
        objects_in_scene_with_grasps.push_back(man_obj);
        //man_obj->print();
        std::cout << man_obj->getName() << std::endl;
    }


    //  try loading the robot

    VirtualRobot::RobotPtr robot;

    try
    {
        robot = VirtualRobot::RobotIO::loadRobot(robot_filename);
    }
    catch (VirtualRobot::VirtualRobotException &exc)
    {
        std::cout << "Error: " << exc.what() << std::endl;
    }

    //  try loading the object

    VirtualRobot::ManipulationObjectPtr object;

    try
    {
        object = VirtualRobot::ObjectIO::loadManipulationObject(object_filename);
    }
    catch (VirtualRobot::VirtualRobotException &exc)
    {
        std::cout << "Error: " << exc.what() << std::endl;
    }

    //  if no object and robot are valid, quit the program

    if (!robot || !object)
    {
        std::cout << "Object or robot invalid or not found. Quitting..." << std::endl;
        return 1;
    }

    //  retrieve and display robot end effector list

    std::vector<VirtualRobot::EndEffectorPtr> robotEndEffectors = robot->getEndEffectors();

    listEndEffectors(robotEndEffectors);

    //  retrieve robot end effector by name and load it

    std::string eef_name = VirtualRobot::RuntimeEnvironment::getValue("eef");
    VirtualRobot::EndEffectorPtr eef = robot->getEndEffector(eef_name);

    if (eef_name.empty() || !eef)
    {
        std::cout << "End effector not found. Evaluation will consider grasps with any end effector" << std::endl;
    }
    else
    {
        std::cout << "Using end effector: " << eef_name << std::endl;
    }

    //  extract grasps

    std::vector<VirtualRobot::GraspSetPtr> all_grasp_sets = object->getAllGraspSets();


    for (VirtualRobot::GraspSetPtr grasp_set : all_grasp_sets)
    {

        //  if eef was specified, skip any grasp not using it

        if(eef && eef_name != grasp_set->getEndEffector())
        {
            continue;
        }

        //VirtualRobot::GraspSetPtr grasp_set = object->getGraspSet(eef);
        std::vector < VirtualRobot::GraspPtr > grasp_vec = grasp_set->getGrasps();

        listGrasps(grasp_vec);

        //  create quality measure and calculate OWS

        GraspStudio::GraspQualityMeasureWrenchSpacePtr qualityMeasure( new GraspStudio::GraspQualityMeasureWrenchSpace(object));

        qualityMeasure->calculateObjectProperties();

        for ( VirtualRobot::GraspPtr grasp : grasp_vec)
        {
            //  get eef for each grasp

            std::string eef_name = grasp->getEefName();

            //  assume eef is present in robot config, otherwise will crash

            eef = robot->getEndEffector(eef_name);

            //  extract preshape and assign it

            std::string preshape_name = grasp->getPreshapeName();
            eef->setPreshape(preshape_name);

            //  get the pose of the eef TCP

            Eigen::Matrix4Xf mGrasp = grasp->getTcpPoseGlobal(object->getGlobalPose());

            //  not sure what this one does

            GraspStudio::ApproachMovementSurfaceNormalPtr approach(new GraspStudio::ApproachMovementSurfaceNormal(object,eef));

            //  clone the eef and place it according to the TCP in the grasp pose

            VirtualRobot::RobotPtr eefCloned = approach->getEEFRobotClone();
            eefCloned->setGlobalPoseForRobotNode(eefCloned->getEndEffector(eef_name)->getTcp(), mGrasp);

            //  obtain contact information

            eefCloned->getEndEffector(eef_name)->openActors();

            //  do some funky stuff to get the eef as robot
            //VirtualRobot::RobotPtr eef_as_robot = eefCloned->createEefRobot("HAND", "hand");
            VirtualRobot::RobotConfigPtr eef_robot_config = eefCloned->getConfig();
            std::cout << "EEF config before actor closing: " << std::endl;
            eef_robot_config->print();

            std::map<std::string, float> config_as_map = eef_robot_config->getRobotNodeJointValueMap();

            //  add joint config before closing to the trajectory

            VirtualRobot::TrajectoryPtr hand_joint_traj( new VirtualRobot::Trajectory(robot->getRobotNodeSet(eef_name)));

              //  std::map is always ordered by key, so iterating two times will return the keys in the same order!
//            for (const auto& pair : config_as_map)
//            {
//                std::cout << "Key " << pair.first << " \t\tvalue: " << pair.second << std::endl;        //  we can transform a robot joint config in a Eigen vector and the same joint will always have the same index. The trajectory only cares about the number of joints and the consistency if the index, not about the actual meaning of entries!
//            }

            VirtualRobot::EndEffector::ContactInfoVector contacts = eefCloned->getEndEffector(eef_name)->closeActors(object);

//            eef_robot_config = eefCloned->getConfig();
//            std::cout << "EEF config after actor closing: " << std::endl;
//            eef_robot_config->print();

//            config_as_map = eef_robot_config->getRobotNodeJointValueMap();
//            for (const auto& pair : config_as_map)
//            {
//                std::cout << "Key " << pair.first << " \t\tvalue: " << pair.second << std::endl;
//            }


            //  compute pose quality

            qualityMeasure->setVerbose(false);
            qualityMeasure->setContactPoints(contacts);

            float grasp_quality = qualityMeasure->getGraspQuality();

            bool is_grasp_fc = qualityMeasure->isGraspForceClosure();

            std::cout << "Grasp " << grasp->getName() << (is_grasp_fc ? " IS " : " IS NOT ") << "in force closure" << std::endl;
            std::cout << "Quality: " << grasp_quality << std::endl;

            //  evaluate robustness for some default parameters

            int num_samples = 500;
            float max_pos_delta = 10.0;
            float max_ori_delta = 10.0;

            GraspStudio::GraspEvaluationPoseUncertainty::PoseUncertaintyConfig uncertainty_config;
            uncertainty_config.init(max_pos_delta, max_ori_delta);

            GraspStudio::GraspEvaluationPoseUncertaintyPtr uncertainty_eval(new GraspStudio::GraspEvaluationPoseUncertainty(uncertainty_config));

            std::vector<Eigen::Matrix4f> poses_eval = uncertainty_eval->generatePoses(object->getGlobalPose(), contacts, num_samples);

            GraspStudio::GraspEvaluationPoseUncertainty::PoseEvalResults results = uncertainty_eval->evaluatePoses(eefCloned->getEndEffector(eef_name), object, poses_eval, qualityMeasure);

            std::cout << "Robustness: " << std::endl;

            results.print();

            //  open the actors after grasp

            eefCloned->getEndEffector(eef_name)->openActors();

        }
    }

    return 0;
}
