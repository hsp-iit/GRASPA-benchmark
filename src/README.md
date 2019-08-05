

# GRASPA score computation
We provide a [bash script](https://github.com/robotology/GRASPA-benchmark/blob/master/src/script_launcher.sh) to automatically compute all the scores defined within GRASPA 1.0.

In order to properly run the script, the user is required to fill some information in:
- [`REACHED_POSES_FOLDER`](https://github.com/robotology/GRASPA-benchmark/blob/master/src/script_launcher.sh#L3), the folder including the data collected during the **reachability test**. An example of reachability folder is given in  [`GRASPA-test`](https://github.com/robotology-playground/GRASPA-test/tree/master/experiment_data/right_arm/reaching_test), that collects the data acquired on the iCub humanoid robot.

- [`REACHED_POSES_FOLDER`](https://github.com/robotology/GRASPA-benchmark/blob/master/src/script_launcher.sh#L4), the file including the data collected during the **camera calibration test**. An example of reachability folder is given in  [`GRASPA-test`](https://github.com/robotology-playground/GRASPA-test/tree/master/experiment_data/right_arm/camera_calibration), that collects the data acquired on the iCub humanoid robot.

- [`GRASPS_FOLDER`](https://github.com/robotology/GRASPA-benchmark/blob/master/src/script_launcher.sh#L5), the folder including the data collected during the **grasp execution on a specific layout**. An example of reachability folder is given in  [`GRASPA-test`](https://github.com/robotology-playground/GRASPA-test/tree/master/experiment_data/right_arm/grasps_data/layout_0), that collects the data acquired on the iCub humanoid robot.

- [`LAYOUT_NAME`](https://github.com/robotology/GRASPA-benchmark/blob/master/src/script_launcher.sh#L6), the **name of the layout** on which the grasping data have been collected. Accepted labels: `Benchmark_Layout_0`, `Benchmark_Layout_1`, `Benchmark_Layout_2`.


- [`MODALITY`](https://github.com/robotology/GRASPA-benchmark/blob/master/src/script_launcher.sh#L7) to specify **if the benchmarking is executed in isolation** (label: `isolation`) or **in the clutter** (label: `clutter`).

**TODO** Add thresholds

At first the grasp quality for each grasp in the specific layout is evaluated using [`compute-grasp-quality-with-visu`](https://github.com/robotology/GRASPA-benchmark/tree/master/src/compute-grasp-quality-with-visu). The grasp qualities are added in the grasps data and, finally, each score of the benchmark is computed by [`scores_evaluation.py`](https://github.com/robotology/GRASPA-benchmark/blob/master/src/scores_evaluation.py).

## Output example

### Grasp quality evaluation
**TODO**: Add output example of grasp quality

### Benchmark scores computation
- The benchmark scores are computed for **one layout per time**:

 <p align="center">
<img src="https://github.com/robotology/GRASPA-benchmark/blob/master/media/output1-layout.png" width=400>
</p>

- The **reachability and camera calibration scores** are computed by comparing the
poses defined within the benchmark with those acquired by the user:

 <p align="center">
<img src="https://github.com/robotology/GRASPA-benchmark/blob/master/media/output1-reachability.png" width=400> <img src="https://github.com/robotology/GRASPA-benchmark/blob/master/media/output1-camera.png" width=400>
</p>

- The **graspability, binary success and grasp stability** are just read from the files provided by the user:

 <p align="center">
<img src="https://github.com/robotology/GRASPA-benchmark/blob/master/media/output1-graspability.png" width=400> <img src="https://github.com/robotology/GRASPA-benchmark/blob/master/media/output2-binary.png" width=400> <img src="https://github.com/robotology/GRASPA-benchmark/blob/master/media/output2-stability.png" width=400>
</p>


- The **grasp quality** are read from the files properly filled by `compute-grasp-quality-with-visu`:

<p align="center">
<img src="https://github.com/robotology/GRASPA-benchmark/blob/master/media/output2-quality.png" width=400>
</p>

- In the clutter modality, also the **obstacle avoidance scores** are read from the files provided by the user:

<p align="center">
<img src="https://github.com/robotology/GRASPA-benchmark/blob/master/media/output2-obstacle.png" width=400>
</p>

- Then, the final score for the layout under test is provided:

<p align="center">
<img src="https://github.com/robotology/GRASPA-benchmark/blob/master/media/output3.png" width=400>
</p>
