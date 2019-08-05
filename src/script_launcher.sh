#!/bin/bash

REACHED_POSES_FOLDER=/home/gvezzani/giulia_code/GRASPA-test/experiment_data/right_arm/reaching_test/
FILE_CAMERA_CALIBRATION=/home/gvezzani/giulia_code/GRASPA-test/experiment_data/right_arm/camera_calibration/cam_calibration_test_output.xml
GRASPS_FOLDER=/home/gvezzani/giulia_code/GRASPA-test/experiment_data/right_arm/grasps_data/layout_0
LAYOUT_NAME=Benchmark_Layout_0
MODALITY=isolation
THRES_POS_REACH=0.02
THRES_ORIE_REACH=0.5
THRES_REACH=0.5
THRES_POS_CAM=0.045
THRES_ORIE_CAM=0.8
THRES_CAM=0.5


## To launch pose quality evaluation


## To launch the script to compute all the scores
python src/scores_evaluation.py --reached_poses_folder="${REACHED_POSES_FOLDER}" --file_camera_calibration_test="${FILE_CAMERA_CALIBRATION}" --grasps_folder="${GRASPS_FOLDER}" --layout="${LAYOUT_NAME}" --modality="${MODALITY}" --threshold_pos=${THRES_POS_REACH} --threshold_orie=${THRES_ORIE_REACH} --reaching_threshold=${THRES_REACH} --threshold_camera_pos=${THRES_POS_CAM} --threshold_camera_orie=${THRES_ORIE_CAM} --camera_threshold=${THRES_CAM}
