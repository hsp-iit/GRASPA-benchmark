# Template files

The files collected in this folder are templates to be filled in order to evaluate the benchmark scores on the data collected on your robot.
Hereafter, we briefly explain the meaning of the data contained in this file. This information is useful to understand how to fill them
with the data collected on your robot.

## Reachability and camera calibration data
When filling the [reachability](https://github.com/robotology/GRASPA-benchmark/tree/master/data/template_files/reached_poses) or [camera calibration file](https://github.com/robotology/GRASPA-benchmark/tree/master/data/template_files/camera_calibration) is important to:
- Specify the name of the set of poses (e.g. `Set of Poses 1` in this case). GRASPA will use this tag to compare the data 
you collected with the proper corresponding desired poses.
    ```html
    <Scene name="Set_Poses_1">
    ```
- Store each pose in a `ManipulationObject` node with the proper name (e.g. `Reachable_frame00` in this case). This name is used by the benchmark
to compare the reached pose with the corresponding desired pose.
  ```html
      <ManipulationObject name='Reachable_frame00'>
          <File>objects/frame.xml</File>
          <GlobalPose>
              <Transform>
                  <Matrix4x4>
                      <row1 c1='0' c2='0' c3='-1' c4='0'/>
                      <row2 c1='1' c2='0' c3='0' c4='0'/>
                      <row3 c1='0' c2='-1' c3='0' c4='150'/>
                      <row4 c1='0' c2='0' c3='0' c4='1'/>
                  </Matrix4x4>
             </Transform>
          </GlobalPose>
      </ManipulationObject>
  ```
- (Optional) Add the `Table` and `Frame` entities as `Obstacle` nodes to visualize them in the viewer:
    ```html
        <Obstacle name='Table'>
            <Visualization>
    		<File type='inventor'>objects/iv/layout_base_noboard.iv</File>
    		<UseAsCollisionModel/>
    	    </Visualization>
            <GlobalPose>
                <Transform>
                    <Matrix4x4>
                        <row1 c1='1' c2='0' c3='0' c4='-267'/>
                        <row2 c1='0' c2='1' c3='0' c4='178'/>
                        <row3 c1='0' c2='0' c3='1' c4='-2'/>
                        <row4 c1='0' c2='0' c3='0' c4='1'/>
                    </Matrix4x4>
                </Transform>
             </GlobalPose>
        </Obstacle>

        <Obstacle name='Frame'>
            <Visualization>
                <File>data/objects/misc/frame.wrl</File>
	        </Visualization>
             <GlobalPose>
                 <Transform>
                     <Matrix4x4>
                         <row1 c1='1' c2='0' c3='0' c4='-0'/>
                         <row2 c1='0=' c2='1' c3='0' c4='-0'/>
                         <row3 c1='0' c2='0' c3='1' c4='0'/>
                         <row4 c1='0' c2='0' c3='0' c4='1'/>
                     </Matrix4x4>
                 </Transform>
             </GlobalPose>
         </Obstacle>
  ```

- Close the root node:

  ```html
    </Scene>
  ```

## Grasps data
The grasps data must be collected separately for **each layout** and for **each object** in the layout. Missing data will lead to altered benchmark scores.
An example of complete grasps data collected on the iCub humanoid robot is available [here](https://github.com/robotology-playground/GRASPA-test/tree/master/experiment_data/right_arm/grasps_data).

When filling the grasps file, you need to:
- **Rename the grasp file** as `Ycb`+ name of the object in camel case + `_grasp.xml` (see [the file names used in these folders](https://github.com/robotology-playground/GRASPA-test/tree/master/experiment_data/right_arm/grasps_data) for clarity.

- Add a `ManipulationObject` node with the name of the object under test. Accepted object names are listed [here](https://github.com/robotology/GRASPA-benchmark/tree/master/data/objects/YCB), as folder names. 
  ```html
   <ManipulationObject name="banana">
	    <Visualization>
		     <File type="inventor">../../../GRASP-benchmark/data/objects/YCB/banana/./nontextured.stl</File>
	    </Visualization>
	    <CollisionModel>
		     <File type="inventor">../../../GRASP-benchmark/data/objects/YCB/banana/./nontextured.stl</File>
	   </CollisionModel>
   ```
 - Specify the test **layout, the robot and the end-effector** used:
 
   ```html
    <GraspSet name="Benchmark_Layout_0" RobotType="iCub" EndEffector="Right Hand">
   ```
   
- For each grasp executed, specify the **grasp name and  6D pose**:

  ```html
    <Grasp name="Grasp 0" quality="0" Creation="Simox - GraspStudio - GraspWrenchSpace" Preshape="Grasp Preshape">
			<Transform>
				<Matrix4x4>
					<row1 c1="0.027" c2="0.994" c3="-0.110" c4="-242.660" />
					<row2 c1="0.816" c2="-0.086" c3="-0.572" c4="122.864" />
					<row3 c1="-0.578" c2="-0.074" c3="-0.813" c4="52.416" />
					<row4 c1="0" c2="0" c3="0" c4="1" />
				</Matrix4x4>
			</Transform>
		</Grasp>
  ```
  
- Close the `ManipulationObject` node:

    ```html
    </ManipulationObject>   
    ```
    
- Add information on the **graspability** of the object:

   ```html
   <Graspable quality="1" />
  ```
  
- Add information on the **success** of the executed grasp, the **grasp stability** and the **object avoidance** (if in the cluttered mode):

  ```html
  <Grasped>
	  <Grasp name="Grasp 0" quality="1" />
     ...
    </Grasped>
    GraspStability>
      <Grasp name="Grasp 0" quality="0.25" />
      ...
    </GraspStability>
  <ObstacleAvoidance>
    <Grasp name="Grasp 0" quality="0" />
    ...
  </ObstacleAvoidance>
  ```

The `ComputedQuality` node you can see in [the data collected on the iCub](https://github.com/robotology-playground/GRASPA-test/blob/master/experiment_data/right_arm/grasps_data/layout_0/YcbBanana_grasp.xml#L84) is computed by the `compute-grasp-quality-with-visu` executable
when processing the information on the grasp pose. The use is not required to manually add it in the grasps data file.
