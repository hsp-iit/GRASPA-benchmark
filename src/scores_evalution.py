import numpy as np
import xml.etree.ElementTree as ET
from scipy.spatial.transform import Rotation as R
import os, re, sys
from os import listdir
from os.path import isfile, join


import argparse
parser = argparse.ArgumentParser()
parser.add_argument('--file_reachability_test', action='store', dest="reached_poses_file",
                    help='file containing the reached poses during reachability tests')
parser.add_argument('--grasps_folder', action='store', dest="grasping_folder",
                    help='folder containing the xml with the graspings tests for one layout')
parser.add_argument('--scenes_folder', action='store', dest="scenes_folder", default='data/scenes/grasping/3D_scenes/',
                    help='folder containing the xml with the graspings tests for one layout')

# Params useful for xml parsing
acceptable_keys = ['Reachable_frame00', 'Reachable_frame01', 'Reachable_frame02', 'Reachable_frame03',
                   'Reachable_frame10', 'Reachable_frame11', 'Reachable_frame12', 'Reachable_frame13',
                   'Reachable_frame20', 'Reachable_frame21', 'Reachable_frame22', 'Reachable_frame23',
                   'Reachable_frame30', 'Reachable_frame31', 'Reachable_frame32', 'Reachable_frame33']

testing_layout = None
# Scores computed (TODO)
# ------- Reachability -------
# s0 = Reachability score, for region 1,2 and 3
s0 = {}
# ------- Cam Calibration -------
# s1_1 = Reachability score, region 1
# s1_2 = Reachability score, region 2
# s1_3 = Reachability score, region 3
# ------- Grasp Quality -------
# s2 = Grasp Quality for each object
# ------- (Binary) Grasp Success -------
# s3 = Grasp success for each object (dictionary)
s3 = {}
# ------- Object graspability -------
# s4 = Object graspable/not graspable (dictionary)
s4 = {}
# -------Grasp robustness along trajectory -------
# s5 = Grasp robustness along trajectory (dictionary)
s5 = {}
# ------- Final score -------
# s6 = Final score

# Regions dimensions
region_1 = np.array([[0.0, 0.0], [0.0, 123.33], [-544, 0.0], [-544, 123.33]])
region_2 = np.array([[-0.0, 123.33], [0.0, 246.66], [-544, 123.33], [-544, 246.66]])
region_3 = np.array([[-0.0, 246.66], [-0.0, 370], [-544, 246.66], [-544, 370]])

def not_consistent(file, acceptable_object_names):

    tree = ET.parse(file)
    root = tree.getroot()

    file_name = os.path.splitext(os.path.basename(file))[0]
    file_name = file_name[:-6]

    if file_name in acceptable_object_names[root[2].attrib['name']]:
        return False
    else:
        return True

def parse_grasping_files(files, s3, s4, s5):

    for file in files:
        file_name = os.path.splitext(os.path.basename(file))[0]
        file_name = file_name[:-6]

        tree = ET.parse(file)
        root = tree.getroot()

        s3[file_name] = float(root[3].attrib['quality'])
        s4[file_name] = float(root[4].attrib['quality'])
        s5[file_name] = float(root[5].attrib['quality'])


def parse_scenes_files(dict_store, root):

    objects_list = []
    # Parse scene files
    for pose in root.iter('ManipulationObject'):
        objects_list.append(pose.attrib['name'])

    dict_store[root.attrib['name']] = objects_list


def parse_reachability_files(dict_store, root):

    # Parse reachability files
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

    # Compute position error
    position_error = np.linalg.norm(desired_pose['position'] - reached_pose['position'])

    # Compute orientation error
    R_tmp = desired_pose['orientation'].as_dcm() * reached_pose['orientation'].as_dcm().transpose()
    R_error = R.from_dcm(R_tmp)
    orientation_error = np.sin(np.linalg.norm(R_error.as_rotvec()))

    # Return position and orientation error
    return position_error + orientation_error

def compute_reachability_score(args):

    # Parse the file provided by the user including the poses reached by the robot
    tree = ET.parse(args.reached_poses_file)
    root = tree.getroot()
    reached_poses = {}
    parse_reachability_files(reached_poses,root)

    # Parse the file provided by the benchmark for the reachibility test
    # consistent with the tag provided by the user
    desired_poses = {}
    testing_layout = root.attrib['name']

    if (testing_layout == 'Benchmark_Layout_0'):
        pose_to_reach_file = "data/scenes/reachability/reachability_scene_1.xml"
    if (testing_layout == 'Benchmark_Layout_1'):
        pose_to_reach_file = "data/scenes/reachability/reachability_scene_2.xml"
    if (testing_layout == 'Benchmark_Layout_2'):
        pose_to_reach_file = "data/scenes/reachability/reachability_scene_2.xml"

    tree = ET.parse(pose_to_reach_file)
    root = tree.getroot()
    parse_reachability_files(desired_poses, root)

    s0_1 = 0.0
    s0_2 = 0.0
    s0_3 = 0.0
    n_poses_1 = 0
    n_poses_2 = 0
    n_poses_3 = 0

    # Compute reachability error for each region of the scene
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
    s0['s0_1'] = s0_1
    s0['s0_2'] = s0_2
    s0['s0_3'] = s0_3

    print('Reachability scores:')
    print( "|| s0_1    :", s0['s0_1'])
    print( "|| s0_2    :", s0['s0_2'])
    print( "|| s0_3    :", s0['s0_3'])


def read_scores(args):

    # Parse the scenes to load object labels
    scenes_files = [f for f in listdir(args.scenes_folder) if isfile(join(args.scenes_folder,f))]

    # acceptable_object_names lists all the objects name included in a scene
    acceptable_object_names = {}
    for file in scenes_files:
        tree = ET.parse(join(args.scenes_folder,file))
        root = tree.getroot()
        parse_scenes_files(acceptable_object_names,root)


    # Collect file in the folder_grasping_test
    grasp_files = [join(args.grasping_folder,f) for f in listdir(args.grasping_folder) if isfile(join(args.grasping_folder,f))]

    # Check consistency between object and layout
    for file in grasp_files:
        if (not_consistent(file, acceptable_object_names)):
            sys.exit("Inconsistency between provided object names and benchmark layout!")

    # Read Binary scores and save in s2 as a dictionary, with tag the names
    # of the objects as defined in the xmls of the benchmark
    # Read Graspable scores and save in s3 as a dictionary, with tag the names
    # of the objects as defined in the xmls of the benchmark
    # Read Stability scores and save in s4 as a dictionary, with tag the names
    # of the objects as defined in the xmls of the benchmark
    parse_grasping_files(grasp_files, s3, s4, s5)


if __name__ == '__main__':

    # Compute scores s0_1, s0_2, s0_3
    # representing the reachibility of the robot in the scene
    compute_reachability_score(parser.parse_args())

    # Compute grasp Quality
    # TODO To launch fabrizio's code

    # Read scores s3, s4, s5 from user provided files
    read_scores(parser.parse_args())
    import IPython
    IPython.embed()
