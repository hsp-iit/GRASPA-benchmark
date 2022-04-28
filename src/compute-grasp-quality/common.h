#if !defined(COMPUTE_GRASP_QUALITY_H)
#define COMPUTE_GRASP_QUALITY_H

#include <string>
#include <VirtualRobot/EndEffector/EndEffector.h>
#include <VirtualRobot/Grasping/Grasp.h>
#include <VirtualRobot/ManipulationObject.h>

namespace common {

struct GraspQualitySetEntry {
    float quality_collision_free;
    float quality_overall;

    GraspQualitySetEntry(float q_cfree, float q_overall):  quality_collision_free(q_cfree), quality_overall(q_overall) {}
};

typedef std::map <std::string, GraspQualitySetEntry> GraspSetQuality;

std::string fileBaseName(std::string const& path);

bool parseDataPath(std::string & data_path);

bool parseGraspPath(std::string& grasp_path);

bool parseSceneFilename(std::string& scene_filename);

bool parseRobotFilename(std::string& robot_filename);

bool parseObjectFilename(std::string& object_filename);

void listEndEffectors(const std::vector<VirtualRobot::EndEffectorPtr> robotEndEffectors);

void listGrasps(const std::vector<VirtualRobot::GraspPtr> grasp_vec);

int saveComputedQuality( const GraspSetQuality& set_quality, const std::string& xml_filename);

bool generateClosureTrajectory(const VirtualRobot::EndEffectorPtr& end_effector, const VirtualRobot::GraspPtr& grasp, VirtualRobot::TrajectoryPtr& closure_trajectory);

VirtualRobot::ManipulationObjectPtr getManipulationObjectFromGRASPAXML(const std::string& xml_path);

}

#endif
