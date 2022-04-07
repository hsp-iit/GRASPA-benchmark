#include "common.h"
#include <iostream>
#include <string>

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

int common::saveComputedQuality(const common::GraspSetQuality& set_quality, const std::string& xml_filename)
{
    /*  Save the computed quality to the xml file adding a node as child of <grasp_data>
    *   replacing any existing node with same name
    *   <ComputedQuality>
    *       <Grasp name="Grasp 0" quality_collision_free=0.264 quality_overall=0.125/>
    *       <Grasp name="Grasp 1" quality_collision_free=0.493 quality_overall=0.345/>
    *       ...
    *   </ComputedQuality>
    */

    // Check for file existence
    std::ifstream xml_file_in(xml_filename.c_str());
    if (xml_file_in.fail())
    {
        std::cout << "Error: could not open file " << xml_filename << std::endl;
        return 1;
    }

    // Read the XML as a 0-terminated char string
    std::string line, xml_string;
    while (std::getline(xml_file_in, line))
        xml_string += line;
    std::vector<char> xml_string_copy(xml_string.begin(), xml_string.end());
    xml_string_copy.push_back('\0');
    xml_file_in.close();

    // Parse the XML string
    rapidxml::xml_document<char> xml_doc;
    xml_doc.parse<0>(&xml_string_copy[0]);

    // Add a ComputedQuality child to the root node. Delete any existing ones with the same name
    rapidxml::xml_node<char> *root_node = xml_doc.first_node("grasp_data");
    rapidxml::xml_node<char> *existing_computed_quality_node = root_node -> first_node("ComputedQuality");
    if (existing_computed_quality_node != 0)
    {
        root_node -> remove_node(existing_computed_quality_node);
        std::cout << "Removed existing Computed Quality information" << std::endl;
    }
    rapidxml::xml_node<char> *computed_quality_node = xml_doc.allocate_node(rapidxml::node_element, "ComputedQuality", 0);
    root_node -> append_node(computed_quality_node);

    // Add each grasp quality scores
    for (const auto& g_quality : set_quality)
    {
        rapidxml::xml_node<char> *grasp_node = xml_doc.allocate_node(rapidxml::node_element, "Grasp");
        const char* quality_collision_free_string = std::to_string(g_quality.second.quality_collision_free).c_str();
        const char* quality_overall_string = std::to_string(g_quality.second.quality_overall).c_str();
        rapidxml::xml_attribute<char> *grasp_attribute = xml_doc.allocate_attribute("name", g_quality.first.c_str());
        rapidxml::xml_attribute<char> *q_cf_attribute = xml_doc.allocate_attribute("quality_collision_free", xml_doc.allocate_string(quality_collision_free_string));
        rapidxml::xml_attribute<char> *q_ov_attribute = xml_doc.allocate_attribute("quality_overall", xml_doc.allocate_string(quality_overall_string));
        grasp_node -> append_attribute(grasp_attribute);
        grasp_node -> append_attribute(q_cf_attribute);
        grasp_node -> append_attribute(q_ov_attribute);
        computed_quality_node -> append_node(grasp_node);
    }

    // Save the modified DOM
    std::ofstream xml_file_out(xml_filename.c_str());
    xml_file_out << xml_doc;
    xml_file_out.close();

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

    // Find the xml file directory
    std::string xml_dir = xml_path.substr(0, xml_path.find_last_of("/\\"));

    // Call the Simox function to parse the ManipulationObject
    VirtualRobot::ManipulationObjectPtr manipulation_object;
    try
    {
        manipulation_object = VirtualRobot::ObjectIO::processManipulationObject(manipulation_object_node, xml_dir);
    }
    catch (VirtualRobot::VirtualRobotException& exc)
    {
        std::cout << "Could not parse manipulation object from " << xml_path << std::endl;
        throw;
    }

    return manipulation_object;
}


