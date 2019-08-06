# GRASPA data

**GRASPA 1.0** provides the following useful material to enhance reproducibility:

### Object models
- The object models of the **YCB objects included in the benchmark** are available [in this folder](https://github.com/robotology/GRASPA-benchmark/tree/master/data/objects/YCB). These models are used to generate the printable layouts and to compute the grasp quality.

### 6D poses for the reachability tests
The poses to be reached within the reaching test are collected [in this folder](https://github.com/robotology/GRASPA-benchmark/tree/master/data/scenes/reachability). The sets of poses to test the robot reachability on are 3:

   <p align="center">
<img  src="https://github.com/robotology/GRASPA-benchmark/blob/master/media/reachability1.png" width=300>  <img title="Set of poses no. 1" src="https://github.com/robotology/GRASPA-benchmark/blob/master/media/reachability2.png" width=300> <img title="Set of poses no. 2" src="https://github.com/robotology/GRASPA-benchmark/blob/master/media/reachability3.png" width=300>
</p>

The poses are defined considering the hand reference frame attached on the robot hand as follows:

 <p align="center">
<img src="https://github.com/robotology-playground/GRASPA-test/blob/master/misc/icub-hand-frame.jpg" width=200>
</p>

We also provide the set of poses in case the hand reference frame is the following:

<p align="center">
<img src="https://github.com/robotology/GRASPA-benchmark/blob/master/media/icub-left.jpg" width=150>
</p>


In this case, only one set of poses changes (use [this xml file](https://github.com/robotology/GRASPA-benchmark/blob/master/data/scenes/reachability/reachability_set_poses_2l.xml) instead of [this one](https://github.com/robotology/GRASPA-benchmark/blob/master/data/scenes/reachability/reachability_set_poses_2.xml)).



**Note** If the end-effector of the test robot  has a **reference frame with a different orientation** w.r.t the ones shown in the previous pictures, the user is supposed to **transform the desired poses** so to make all the poses have the **approaching direction of the end-effector parallel to the _x-axes_ of the poses defined within GRASPA 1.0**. These ways the poses tested during the reachability tests are consistent with the movements executed by the test robot.

### 6D poses for the camera calibration tests
The poses to be reached within the camera calibration test are collected [in this file](https://github.com/robotology/GRASPA-benchmark/blob/master/data/scenes/camera_calibration/camera_calibration_poses.xml):

 <p align="center">
   <img title="Set of poses no. 1" src="https://github.com/robotology/GRASPA-benchmark/blob/master/media/reachability2.png" width=300>
</p>

The poses are defined considering the hand reference frame attached on the robot hand as follows:

<p align="center">
<img src="https://github.com/robotology-playground/GRASPA-test/blob/master/misc/icub-hand-frame.jpg" width=200>
</p>

We also provide the set of poses in case the hand reference frame is the following:

<p align="center">
<img src="https://github.com/robotology/GRASPA-benchmark/blob/master/media/icub-left.jpg" width=150>
</p>

In this case, use [this xml file](https://github.com/robotology/GRASPA-benchmark/blob/master/data/scenes/camera_calibration/camera_calibration_poses_l.xml) instead of [this one](https://github.com/robotology/GRASPA-benchmark/blob/master/data/scenes/camera_calibration/camera_calibration_poses.xml).



### Grasping scenarios
**GRASPA 1.0** comes with  [3 increasingly challenging layouts](https://github.com/robotology/GRASPA-benchmark/tree/master/data/scenes/grasping/3D_scenes), in terms of number of involved objects, their pose within the test area and their graspability.
In order for layouts to be reproducible in practice, we provide [printable layouts](https://github.com/robotology/GRASPA-benchmark/tree/master/data/scenes/grasping/printable_layouts) of dimensions _594x420 mm  (A2 format)_ that include markers and object footprints:

#### Layout 0
<p align="center">
<img src="https://github.com/robotology/GRASPA-benchmark/blob/master/media/scene1.png" width=300> <img src="https://github.com/robotology/GRASPA-benchmark/blob/master/media/layout_0_printout.png" width=300>
</p>

#### Layout 1
<p align="center">
<img src="https://github.com/robotology/GRASPA-benchmark/blob/master/media/scene2.png" width=300> <img src="https://github.com/robotology/GRASPA-benchmark/blob/master/media/layout_1_printout.png" width=300>
</p>

#### Layout 2
<p align="center">
<img src="https://github.com/robotology/GRASPA-benchmark/blob/master/media/scene3.png" width=300> <img src="https://github.com/robotology/GRASPA-benchmark/blob/master/media/layout_2_printout.png" width=300>
</p>

**Note** **GRASPA 1.0** features the printable layouts both in **A2 format** or in **4 combinable A4 files**.
Also, two different sets of printable layouts are available:
- those with the entire marker board:

<p align="center">
<img src="https://github.com/robotology/GRASPA-benchmark/blob/master/media/layout_0_printout.png" width=300>
</p>

- those with missing markers on a central cross:

<p align="center">
<img src="https://github.com/robotology/GRASPA-benchmark/blob/master/media/layout_0_no_central_printout.png" width=300>
</p>

The second option allows for properly printing the 4 combinable A4 layouts without white margins overlapped on markers (as sometimes happens with standard printers). 

### Template files
[This folder](https://github.com/robotology/GRASPA-benchmark/tree/master/data/template_files) collects some `xml` files that can be used as templates to collect the data required by the benchmark. An example of `xmls` filled with the data collected on the iCub is available [in this folder](https://github.com/robotology-playground/GRASPA-test/tree/master/experiment_data/right_arm).

More information on the structure of the template files is available [here](https://github.com/robotology/GRASPA-benchmark/tree/master/data/template_files).
