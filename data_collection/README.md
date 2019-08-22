 # How to collect the data

**GRASPA 1.0** features several scores in order to fairly evaluate a grasp planner algorithm regardless of the possible limitations of test platform.
The computation of each score requires specific data to be collected on the robot.  Hereafter, we summarize the procedure to follow. A sample code for data collection on the iCub is available in [`GRASPA-test`](https://github.com/robotology-playground/GRASPA-test).

### Reachability data
- Put your robot in front of the printed layout:

<p align="center">
<img src="https://github.com/robotology/GRASPA-benchmark/blob/master/media/benchmark-setup1.jpg" width=200>
</p>

- Estimate the **pose of the board** reference frame:

<p align="center">
<img src="https://github.com/robotology-playground/GRASPA-test/blob/master/misc/board_pose.png" width=200> <img
</p>

- Ask the robot to reach all the [desired reachability poses](https://github.com/robotology/GRASPA-benchmark/tree/master/data/scenes/reachability):

<p align="center">
<img src="https://github.com/robotology-playground/GRASPA-test/blob/master/misc/icub-reach.jpg" width=200>
</p>

- Acquire the poses actually reached  by querying **forward kinematics**.
- Save the received poses using [this template](https://github.com/robotology/GRASPA-benchmark/tree/master/data/template_files/reached_poses) and following [this procedure](https://github.com/robotology/GRASPA-benchmark/tree/master/data/template_files#reachability-and-camera-calibration-data).

### Camera calibration data
- Put your robot **in the same pose used during the reachability test**:

<p align="center">
<img src="https://github.com/robotology/GRASPA-benchmark/blob/master/media/benchmark-setup1.jpg" width=200>
</p>

- Estimate the **pose of the board** reference frame:

<p align="center">
<img src="https://github.com/robotology-playground/GRASPA-test/blob/master/misc/board_pose.png" width=200> <img
</p>

- Ask the robot to reach all the [desired camera calibration poses](https://github.com/robotology/GRASPA-benchmark/blob/master/data/scenes/camera_calibration/camera_calibration_poses.xml):

<p align="center">
<img src="https://github.com/robotology-playground/GRASPA-test/blob/master/misc/icub-reach.jpg" width=200>
</p>

- Acquire the poses actually reached  by using **vision**:

<p align="center">
<img src="https://github.com/robotology-playground/GRASPA-test/blob/master/misc/camera-calib2.jpg" width=200>
</p>

- Save the received poses using [this template](https://github.com/robotology/GRASPA-benchmark/blob/master/data/template_files/camera_calibration/camera_calibration_output.xml) and following [this procedure](https://github.com/robotology/GRASPA-benchmark/tree/master/data/template_files#reachability-and-camera-calibration-data).

### Grasps data:
- Put your robot in the **same pose used during the reachability and camera calibration tests**:

<p align="center">
<img src="https://github.com/robotology/GRASPA-benchmark/blob/master/media/benchmark-setup1.jpg" width=200>
</p>

- Estimate the **pose of the board** reference frame:

<p align="center">
<img src="https://github.com/robotology-playground/GRASPA-test/blob/master/misc/board_pose.png" width=200> <img
</p>

- Using your grasp planner, compute and make the robot execute **5 grasps for each object** in the layout. You can benchmark your algorithm *in isolation*:

  <p align="center">
  <img src="https://github.com/robotology-playground/GRASPA-test/blob/master/misc/grasp.jpg" width=200>
  </p>

  Or *in the clutter* :

  <p align="center">
  <img src="https://github.com/robotology/GRASPA-benchmark/blob/master/media/benchmark-setup2.jpg" width=200>
  </p>

  on condition that **the same modality is kept for each object and layout**.

- If the robot grasps the object, make the robot **execute the trajectory for assessing the grasp stability** defined within **GRASPA 1.0**:
<p align="center">
<img src="https://github.com/robotology-playground/GRASPA-test/blob/master/misc/grasp.jpg" width=200> <img src="https://github.com/robotology-playground/GRASPA-test/blob/master/misc/traj1.jpg" width=200> <img src="https://github.com/robotology-playground/GRASPA-test/blob/master/misc/grasp.jpg" width=200> <img src="https://github.com/robotology-playground/GRASPA-test/blob/master/misc/traj3.jpg" width=200>
</p>

- Store all the data using [these templates](https://github.com/robotology/GRASPA-benchmark/tree/master/data/template_files/grasps_data/layout_0) and following [this procedure](https://github.com/robotology/GRASPA-benchmark/tree/master/data/template_files#grasps-data).


Once all the data for one layout have been acquired, **the scores can be computed** following [these instructions](https://github.com/robotology/GRASPA-benchmark#how-to-run).
The **procedure needs to be repeated for each layout** defined by the benchmark.
