


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
 
 [![Preview](media/preview.jpg)](https://youtu.be/wUBHQQddJr0)

## Overview
- [How to install](https://github.com/robotology/GRASPA-benchmark#how-to-install)
- [How to run](https://github.com/robotology/GRASPA-benchmark#how-to-run)
- [Resources&Tools](https://github.com/robotology/GRASPA-benchmark#resourcestools)
- [How to collect data](https://github.com/robotology/GRASPA-benchmark#how-to-collect-the-data)
 ## How to install
  ### Using GitPod
  We have packaged a GitPod environment with every dependency compiled and already installed. Click on the tempting blue button to open the cloud environment in your browser. 
 [![Gitpod](https://gitpod.io/button/open-in-gitpod.svg)](https://gitpod.io/#https://github.com/robotology/GRASPA-benchmark)
 ### Locally
 **TODO**

 ## How to run
 ### Using GitPod
 **TODO**

 ### Locally
 Once the data required by the benchmark are available (see [here](https://github.com/robotology/GRASPA-benchmark#how-to-collect-the-data) instructions on how to collect them), all the scores can be computed by:
 1. Properly pointing to the folders including the data in the [`src/script_launcher.sh`](https://github.com/robotology/GRASPA-benchmark/blob/master/src/script_launcher.sh). More information on how to fill correctly the script is available [here](https://github.com/robotology/GRASPA-benchmark/blob/master/src/README.md#graspa-score-computation).
 2. Launching the script:
     ```
     cd /path/to/GRASPA-benchmark
     $ ./src/script_launcher.sh
     ```
    An example of the benchmark output is available [here](https://github.com/robotology/GRASPA-benchmark/blob/master/src/README.md#output-example).

 ## Resources&Tools
**GRASPA 1.0** comes with several resources and tools:
-  [`data/`](https://github.com/robotology/GRASPA-benchmark/tree/master/data) collects the:
   - [`YCB objects models`](https://github.com/robotology/GRASPA-benchmark/tree/master/data/objects/YCB) used in the grasping layouts;
   - the [`3D renders of the grasping layouts`](https://github.com/robotology/GRASPA-benchmark/tree/master/data/scenes/grasping/3D_scenes);
   - the [`printable versions`](https://github.com/robotology/GRASPA-benchmark/tree/master/data/scenes/grasping/printable_layouts);
   - the poses to be reached for the [`reachability test`](https://github.com/robotology/GRASPA-benchmark/tree/master/data/scenes/reachability) and the [`camera-calibration test`](https://github.com/robotology/GRASPA-benchmark/tree/master/data/scenes/camera_calibration).
   - [`template files`](https://github.com/robotology/GRASPA-benchmark/tree/master/data/template_files), the `xmls` to be filled with the data collected on your robot in order to compute the benchmark scores.
- [`src/`](https://github.com/robotology/GRASPA-benchmark/tree/master/src) includes several scripts and executables:
   - a [`launcher script`](https://github.com/robotology/GRASPA-benchmark/blob/master/src/script_launcher.sh) to automatically compute all the scores;
   - a [`C++ executable`](https://github.com/robotology/GRASPA-benchmark/tree/master/src/compute-grasp-quality#grasp-quality-computation) to compute the **grasp quality** score using [Simox](https://gitlab.com/Simox/simox);
   - a [`python script`](https://github.com/robotology/GRASPA-benchmark/blob/master/src/scores_evaluation.py) to combine all the information to get the **benchmark scores**;
   - a [`viewer`](https://github.com/robotology/GRASPA-benchmark/tree/master/src/scene-viewer#scene-viewer) to **visualize the data** provided within GRASPA and those collected by the user;
   - a [`python script`](https://github.com/robotology/GRASPA-benchmark/tree/master/src/layout-printer#layout-printer) to automatically **generate the printable layouts** of the grasping scenarios defined by **GRASPA 1.0**.

 ## [How to collect the data](/data_collection)
