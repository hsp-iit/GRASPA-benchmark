#!/bin/bash

## Layout -- Change me
LAYOUT=0

## Algorithm to test -- Change me
ALGORITHM=superquadrics

## Env variables
GRASPA_TEST_DIR=/workspace/GRASPA-test/experiment_data/icub/icub-$ALGORITHM
REACHED_POSES_FOLDER=$GRASPA_TEST_DIR/reaching_test/
FILE_CAMERA_CALIBRATION=$GRASPA_TEST_DIR/camera_calibration/cam_calibration_test_output.xml
GRASPS_FOLDER=$GRASPA_TEST_DIR/grasps_data/layout_${LAYOUT}
LAYOUT_NAME=Benchmark_Layout_${LAYOUT}
MODALITY=isolation

## Thresholds
THRES_POS_REACH=0.02
THRES_ORIE_REACH=0.5
THRES_REACH=0.5
THRES_POS_CAM=0.045
THRES_ORIE_CAM=0.8
THRES_CAM=0.5

## To launch pose quality evaluation
compute-grasp-quality --scene $GRASPA_DIR/data/scenes/grasping/3D_scenes/layout_${LAYOUT}.xml --graspsdir $GRASPA_TEST_DIR/grasps_data --datapath $GRASPA_DIR --robot /usr/local/data/robots/iCub/iCub.xml


## To launch the script to compute all the scores
python3 src/scores_evaluation.py --reached_poses_folder="${REACHED_POSES_FOLDER}" --file_camera_calibration_test="${FILE_CAMERA_CALIBRATION}" --grasps_folder="${GRASPS_FOLDER}" --layout="${LAYOUT_NAME}" --modality="${MODALITY}" --threshold_pos=${THRES_POS_REACH} --threshold_orie=${THRES_ORIE_REACH} --reaching_threshold=${THRES_REACH} --threshold_camera_pos=${THRES_POS_CAM} --threshold_camera_orie=${THRES_ORIE_CAM} --camera_threshold=${THRES_CAM}
