


 <h1 align="center">Welcome to <b>GRASPA</b>!</h1>

 <p align="center">
<img src="https://github.com/robotology/GRASPA-benchmark/blob/master/media/benchmark-setup2.jpg" width=400>
</p>

**GRASPA (GRASPA is a Robot Arm graSping Performance benchmArk)** is
 a benchmarking protocol and a set of metrics to evaluate the performance of grasp planners. It aims to **fairly compare methodologies tested on different robots** by measuring and accounting for **platform limitations** that might affect the performance of the algorithm itself.

 ## Version 1.0
 Version 1.0 features:
 -  **3 printable layouts** of predefined grasping scenarios (composed of a subset of YCB objects and localization markers) to enhance test reproducibility.
 - A protocol to **assess** the **robot reachability** and the **calibration of the vision system** within the defined grasping setup area.
 - Usage of a popular **grasp quality metric** to evaluate user-provided grasping poses.
 - A score to assess the **grasp stability during the practical execution** on the robot.
 - The possibility to **benchmark** the algorithm **either in isolation or in the clutter**, with the definition of a further metric to  evaluate the obstacle avoidance in the clutter modality.
 - A **composite score** to quantify the **overall performance** of the algorithm **accounting for** reachability and calibration **limits of the test platform**.

## Overview
- [How to install](https://github.com/robotology/GRASPA-benchmark#how-to-install)
- [How to run](https://github.com/robotology/GRASPA-benchmark#how-to-run)
- [Resources&Tools](https://github.com/robotology/GRASPA-benchmark#resourcestools)
- [How to collect data](https://github.com/robotology/GRASPA-benchmark#how-to-collect-the-data)
 ## How to install
  ### Using GitPod
 **TODO**
 ### Locally
 **TODO**

 ## How to run
 ### Using GitPod
 **TODO**

 ### Locally
 Once the data required by the benchmark are available, all the scores can be computed by:
 1. Properly pointing to the folders including the data in the [`src/script_launcher.sh`](https://github.com/robotology/GRASPA-benchmark/blob/master/src/script_launcher.sh). More information on how to fill correctly the script is available [here](https://github.com/robotology/GRASPA-benchmark/blob/master/src/README.md#graspa-score-computation).
 2. Launching the script:
     ```
     cd /path/to/GRASPA-benchmark
     $ ./src/script_launcher.sh
     ```

 ## Resources&Tools
GRASPA 1.0 comes with several resources and tools:
-  [`data/`](https://github.com/robotology/GRASPA-benchmark/tree/master/data) collects the:
   - [`YCB objects models`](https://github.com/robotology/GRASPA-benchmark/tree/master/data/objects) used in the grasping layouts;
   - the [`3D renders of the grasping layouts`](https://github.com/robotology/GRASPA-benchmark/tree/master/data/scenes/grasping/3D_scenes);
   - the [`printable versions`](https://github.com/robotology/GRASPA-benchmark/tree/master/data/scenes/grasping/printable_layouts);
   - the poses to be reached for the [`reachability test`](https://github.com/robotology/GRASPA-benchmark/tree/master/data/scenes/reachability) and the [`camera-calibration test`](https://github.com/robotology/GRASPA-benchmark/tree/master/data/scenes/camera_calibration).
   - [`template files`](https://github.com/robotology/GRASPA-benchmark/tree/master/data/template_files), the `xmls` to be filled with the data collected on your robot in order to compute the benchmark scores.
- [`src/`](https://github.com/robotology/GRASPA-benchmark/tree/master/src) includes several scripts and executables:
   - a [`launcher script`](https://github.com/robotology/GRASPA-benchmark/blob/master/src/script_launcher.sh) to automatically compute all the scores;
   - a [`C++ executable`](https://github.com/robotology/GRASPA-benchmark/tree/master/src/compute-grasp-quality-with-visu#grasp-quality-computation) to compute the **grasp quality** score using Simox [??];
   - a [`python script`](https://github.com/robotology/GRASPA-benchmark/blob/master/src/scores_evaluation.py) to combine all the information to get the **benchmark scores**;
   - a [`viewer`](https://github.com/robotology/GRASPA-benchmark/tree/master/src/scene-viewer#scene-viewer) to **visualize the data** provided within GRASPA and those collected by the user;
   - a [`python script`](https://github.com/robotology/GRASPA-benchmark/tree/master/src/layout-printer#layout-printer) to automatically **generate the printable layouts** of the grasping scenarios defined by GRASPA.

 ## How to collect the data

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
<img src="https://github.com/robotology-playground/GRASPA-test/blob/master/misc/icub-reach.jpg" width=300>
</p>

- Acquire the poses actually reached  by querying **forward kinematics**.
- Save the received poses using [this template](https://github.com/robotology/GRASPA-benchmark/tree/master/data/template_files/reached_poses) and following [this procedure](https://github.com/robotology/GRASPA-benchmark/tree/master/data/template_files#reachability-and-camera-calibration-data).

### Camera calibration data
- Put your robot **in the same pose used during the reachability**:

<p align="center"> and camera calibration tests
<img src="https://github.com/robotology/GRASPA-benchmark/blob/master/media/benchmark-setup1.jpg" width=200>
</p>

- Estimate the **pose of the board** reference frame:

<p align="center">
<img src="https://github.com/robotology-playground/GRASPA-test/blob/master/misc/board_pose.png" width=200> <img
</p>

- Ask the robot to reach all the [desired camera calibration poses](https://github.com/robotology/GRASPA-benchmark/blob/master/data/scenes/camera_calibration/camera_calibration_poses.xml):

<p align="center">
<img src="https://github.com/robotology-playground/GRASPA-test/blob/master/misc/icub-reach.jpg" width=300>
</p>

- Acquire the poses actually reached  by using **vision**:

<p align="center">
<img src="https://github.com/robotology-playground/GRASPA-test/blob/master/misc/camera-calib2.jpg" width=300>
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

 - If the robot grasps the object, make the robot **execute the trajectory for assessing the grasp stability** defined within GRASPA:
 <p align="center">
<img src="https://github.com/robotology-playground/GRASPA-test/blob/master/misc/grasp.jpg" width=200> <img src="https://github.com/robotology-playground/GRASPA-test/blob/master/misc/traj1.jpg" width=200> <img src="https://github.com/robotology-playground/GRASPA-test/blob/master/misc/grasp.jpg" width=200> <img src="https://github.com/robotology-playground/GRASPA-test/blob/master/misc/traj3.jpg" width=200>
</p>

- Store all the data using [these templates](https://github.com/robotology/GRASPA-benchmark/tree/master/data/template_files/grasps_data/layout_0) and following [this procedure](https://github.com/robotology/GRASPA-benchmark/tree/master/data/template_files#grasps-data).


Once all the data for one layout have been acquired, **the scores can be computed** following [these instructions](https://github.com/robotology/GRASPA-benchmark#how-to-run).
The **procedure needs to be repeated for each layout** defined by the benchmark.
