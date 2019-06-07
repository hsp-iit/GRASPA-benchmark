import numpy as np
import xml.etree.ElementTree as ET
from scipy.spatial.transform import Rotation as R

import argparse
parser = argparse.ArgumentParser()
parser.add_argument('--file_reachability_test', action='store', dest="reached_poses_file",
                    help='file containing the reached poses during reachability tests')

# Params useful for xml parsing
acceptable_keys = ['Reachable_frame00', 'Reachable_frame01', 'Reachable_frame02', 'Reachable_frame03',
                   'Reachable_frame10', 'Reachable_frame11', 'Reachable_frame12', 'Reachable_frame13',
                   'Reachable_frame20', 'Reachable_frame21', 'Reachable_frame22', 'Reachable_frame23',
                   'Reachable_frame30', 'Reachable_frame31', 'Reachable_frame32', 'Reachable_frame33']

# Scores computed (TODO)
# ------- Reachability -------
# s0_1 = Reachability score, region 1
s0_1 = 0.0
# s0_2 = Reachability score, region 2
s0_2 = 0.0
# s0_3 = Reachability score, region 3
s0_3 = 0.0
# ------- Cam Calibration -------
# s1_1 = Reachability score, region 1
# s1_2 = Reachability score, region 2
# s1_3 = Reachability score, region 3
# ------- Grasp Quality -------
# s2_i = Grasp Quality for each object
# ------- (Binary) Grasp Success -------
# s2_i = Grasp success for each object
# ------- Object graspability -------
# s3_i = Object graspable/not graspable
# -------Grasp robustness along trajectory -------
# s4_i = Object graspable/not graspable
# ------- Final score -------
# s6 = Final score

# Regions dimensions
region_1 = np.array([[0.0, 0.0], [0.0, 123.33], [-544, 0.0], [-544, 123.33]])
region_2 = np.array([[-0.0, 123.33], [0.0, 246.66], [-544, 123.33], [-544, 246.66]])
region_3 = np.array([[-0.0, 246.66], [-0.0, 370], [-544, 246.66], [-544, 370]])

def parse_reachability_files(dict_store, root):

    for pose in root.iter('ManipulationObject'):
        if (pose.attrib['name'] in acceptable_keys):
            print(pose.attrib['name'])
            R_matrix = np.zeros((3,3))
            position = np.zeros(3)
            for r in pose.iter('row1'):
                R_matrix[0,0] =  r.attrib['c1']
                R_matrix[0,1] =  r.attrib['c2']
                R_matrix[0,2] =  r.attrib['c3']
                position[0] = r.attrib['c4']
            for r in pose.iter('row2'):
                R_matrix[1,0] =  r.attrib['c1']
                R_matrix[1,1] =  r.attrib['c2']
                R_matrix[1,2] =  r.attrib['c3']
                position[1] = r.attrib['c4']
            for r in pose.iter('row3'):
                R_matrix[2,0] =  r.attrib['c1']
                R_matrix[2,1] =  r.attrib['c2']
                R_matrix[2,2] =  r.attrib['c3']
                position[2] = r.attrib['c4']
            print('Rotation_matrix ')
            r_rot = R.from_dcm(R_matrix)
            print(r_rot.as_dcm())
            print('Position ')
            print("    ",position)

            dict_store[pose.attrib['name']] = {'position': position, 'orientation': r_rot}
            print('---------------------------------------')

def computeReachingError(desired_pose, reached_pose):
    #TODO Check this/Debug
    position_error = np.linalg.norm(desired_pose['position'] - reached_pose['position'])
    R_tmp = reached_pose['orientation'].as_dcm() * desired_pose['orientation'].as_dcm().transpose()
    R_error = R.from_dcm(R_tmp)
    error = np.linalg.norm(R_error.as_rotvec())

    print('desired ', desired_pose['orientation'].as_dcm())
    print('reached ', reached_pose['orientation'].as_dcm())
    return error

def compute_reachability_score(args):

    tree = ET.parse(args.reached_poses_file)
    root = tree.getroot()

    reached_poses = {}
    parse_reachability_files(reached_poses,root)

    desired_poses = {}
    if (root.attrib['name'] == 'Benchmark_Layout_0'):
        pose_to_reach_file = "data/scenes/reachability/reachability_scene_1.xml"
    if (root.attrib['name'] == 'Benchmark_Layout_1'):
        pose_to_reach_file = "data/scenes/reachability/reachability_scene_2.xml"
    if (root.attrib['name'] == 'Benchmark_Layout_2'):
        pose_to_reach_file = "data/scenes/reachability/reachability_scene_2.xml"

    tree = ET.parse(pose_to_reach_file)
    root = tree.getroot()

    parse_reachability_files(desired_poses, root)

    #TODO
    # Compute score s0
    s0_1 = 0.0
    s0_2 = 0.0
    s0_3 = 0.0
    n_poses_1 = 0
    n_poses_2 = 0
    n_poses_3 = 0
    for name_pose in desired_poses:
        if( desired_poses[name_pose]['position'][1] >= region_1[2,1] and
            desired_poses[name_pose]['position'][1] <= region_1[1,1]):
            s0_1 = s0_1 + computeReachingError(desired_poses[name_pose], reached_poses[name_pose])
            n_poses_1 += 1

        if( desired_poses[name_pose]['position'][1] >= region_2[2,1] and
            desired_poses[name_pose]['position'][1] <= region_2[1,1]):
            s0_2 = s0_2 + computeReachingError(desired_poses[name_pose], reached_poses[name_pose])
            n_poses_2 += 1

        if( desired_poses[name_pose]['position'][1] >= region_3[2,1] and
            desired_poses[name_pose]['position'][1] <= region_3[1,1]):
            s0_3 += computeReachingError(desired_poses[name_pose], reached_poses[name_pose])
            n_poses_3 += 1

    s0_1 = s0_1 / n_poses_1
    s0_2 = s0_2 / n_poses_2
    s0_3 = s0_3 / n_poses_3

    import IPython
    IPython.embed()


if __name__ == '__main__':
    compute_reachability_score(parser.parse_args())
