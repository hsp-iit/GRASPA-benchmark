#if !defined(COMPUTE_GRASP_QUALITY_H)
#define COMPUTE_GRASP_QUALITY_H

#include <string>
#include <VirtualRobot/EndEffector/EndEffector.h>
#include <VirtualRobot/Grasping/Grasp.h>

void parseDataPath();

bool parseRobotFilename(std::string& robot_filename);

bool parseObjectFilename(std::string& object_filename);

void listEndEffectors(const std::vector<VirtualRobot::EndEffectorPtr> robotEndEffectors);

void listGrasps(const std::vector<VirtualRobot::GraspPtr> grasp_vec);

#endif
