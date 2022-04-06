#include "common.h"
#include <iostream>

#include <VirtualRobot/VirtualRobot.h>
#include <VirtualRobot/VirtualRobotException.h>
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

#include <rapidxml/rapidxml.hpp>
#include <rapidxml/rapidxml_utils.hpp>
#include <rapidxml/rapidxml_print.hpp>


std::string common::fileBaseName(std::string const& path)
{
    //  return basename of a file, without extension

    std::string basename_with_extension = path.substr(path.find_last_of("/\\") + 1);

    return basename_with_extension.substr(0, basename_with_extension.find_last_of("."));
}

bool common::parseDataPath(std::string& data_path)
{
    //  add datapath to search path

    std::string data_path_cmd_line = VirtualRobot::RuntimeEnvironment::getValue("datapath");
    if (!data_path_cmd_line.empty())
    {
        VirtualRobot::RuntimeEnvironment::addDataPath(data_path_cmd_line);
        return true;
    }
    else
    {
        VirtualRobot::RuntimeEnvironment::addDataPath(data_path);
    }

    return false;
}

bool common::parseGraspPath(std::string& grasp_path)
{
    //  this function doesn't serve much purpose, only exists for consistency

    std::string grasp_path_cmd_line = VirtualRobot::RuntimeEnvironment::getValue("graspsdir");
    if (!grasp_path_cmd_line.empty())
    {
        grasp_path = grasp_path_cmd_line;
        return true;
    }
    else
    {
        return false;
    }
}

bool common::parseSceneFilename(std::string& scene_filename)
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

bool common::parseRobotFilename(std::string& robot_filename)
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

bool common::parseObjectFilename(std::string& object_filename)
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

void common::listEndEffectors(const std::vector<VirtualRobot::EndEffectorPtr> robotEndEffectors)
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

void common::listGrasps(const std::vector<VirtualRobot::GraspPtr> grasp_vec)
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

bool common::generateClosureTrajectory(const VirtualRobot::EndEffectorPtr& end_effector, const VirtualRobot::GraspPtr& grasp, VirtualRobot::TrajectoryPtr& closure_trajectory)
{

    //  if the grasp specifies a final configuration, generate a joint space trajectory that
    //  goes from the initial config to that

    end_effector->openActors();

    //  store the initial configuration as a Eigen vector

    VirtualRobot::RobotConfigPtr eef_initial_config = end_effector->getConfiguration();
    eef_initial_config->print();


    return true;

}

int common::saveComputedQuality( const common::GraspSetQuality& set_quality, const std::string& xml_filename)
{

    /*  append to the xml file a node containing
    *   <ComputedQuality>
    *       <Grasp name="Grasp 0" quality_collision_free=0.264 quality_overall=0.125/>
    *       <Grasp name="Grasp 1" quality_collision_free=0.493 quality_overall=0.345/>
    *       ...
    *   </ComputedQuality>
    */


    //  check if the field exists first
    //  if it does, delete any ComputedQuality field

    std::string tmp_filename = xml_filename + ".tmp";

    std::ifstream file;
    std::ofstream tmp;

    file.open(xml_filename);
    tmp.open(tmp_filename);

    if (file.is_open())
    {
        std::string line;

        while(!file.eof())
        {
            std::getline(file, line);

            if (line.find("<ComputedQuality>", 0) != std::string::npos)
            {

                while(line.find("</ComputedQuality>",0 ) == std::string::npos)
                {
                    std::getline(file, line);
                }

                continue;
            }

            if (file.eof())
                tmp << line;
            else
                tmp << line << std::endl;

        }
    }

    file.close();
    tmp.close();

    std::remove(xml_filename.c_str());
    std::rename(tmp_filename.c_str(), xml_filename.c_str());

    std::ofstream xml_file(xml_filename, std::ios_base::app);
    xml_file << "<ComputedQuality>" << "\n";

    for (const auto& g_quality : set_quality)
    {
        xml_file << "    <Grasp name=\"" << g_quality.first << "\"";
        xml_file << " quality_collision_free=\"" << g_quality.second.quality_collision_free << "\"";
        xml_file << " quality_overall=\"" << g_quality.second.quality_overall << "\"";
        xml_file << "/>" << "\n";
    }

    xml_file << "</ComputedQuality>" << "\n";

    xml_file.close();
    return 0;

}

VirtualRobot::ManipulationObjectPtr common::getManipulationObjectFromGRASPAXML(const std::string& xml_path)
{
    // Return a Simox Manipulation Object being parsed from a GRASPA XML with a different xml root

    // Load the XML
    rapidxml::file<char> xml_file(xml_path.c_str());
    rapidxml::xml_document<char> doc;
    doc.parse<0>(xml_file.data());

    // Get the ManipulationObject node
    rapidxml::xml_node<char> *root_node = doc.first_node("grasp_data");
    rapidxml::xml_node<char> *manipulation_object_node = root_node->first_node("ManipulationObject");

    // Print the ManipulationObject node on a xml string
    // std::string manipulation_object_xml_string;
    // rapidxml::print(back_insterter(manipulation_object_xml_string), *manipulation_object_node, 0);
    // std::ifstream manipulation_object_ifstream(manipulation_object_xml_string.c_str());

    // Find the xml file directory
    std::string xml_dir = xml_path.substr(0, xml_path.find_last_of("/\\"));

    // Call the Simox function to parse the ManipulationObject
    VirtualRobot::ManipulationObjectPtr manipulation_object;
    try
    {
        // manipulation_object = VirtualRobot::ObjectIO::LoadManipulationObjectFromString(manipulation_object_xml_string, xml_dir);
        manipulation_object = VirtualRobot::ObjectIO::processManipulationObject(manipulation_object_node, xml_dir);
    }
    catch (VirtualRobot::VirtualRobotException& exc)
    {
        std::cout << "Could not parse manipulation object from " << xml_path << std::endl;
        throw;
    }

    return manipulation_object;
}


