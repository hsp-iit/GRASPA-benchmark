#!/bin/bash

REACHED_POSES_FOLDER=/home/gvezzani/giulia_code/RAL-benchmark-test/experiment_data/right_arm/reaching_test/
FILE_CAMERA_CALIBRATION=/home/gvezzani/giulia_code/RAL-benchmark-test/experiment_data/right_arm/camera_calibration/cam_calibration_test_output.xml
GRASPS_FOLDER=/home/gvezzani/giulia_code/RAL-benchmark-test/experiment_data/right_arm/grasps_data/layout_0
LAYOUT_NAME=Benchmark_Layout_0
MODALITY=isolation


## To launch pose quality evaluation


## To launch the script to compute all the scores
python src/scores_evaluation.py --verbose=True --reached_poses_folder="${REACHED_POSES_FOLDER}" --file_camera_calibration_test="${FILE_CAMERA_CALIBRATION}" --grasps_folder="${GRASPS_FOLDER}" --layout="${LAYOUT_NAME}" --modality="${MODALITY}"
