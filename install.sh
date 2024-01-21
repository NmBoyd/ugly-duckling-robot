#!/bin/bash
sudo apt install ros-noetic-desktop-full
sudo apt install libglpk-dev
sudo apt-get install catkin
sudo apt install ros-noetic-pybind11-catkin
sudo apt install python3-catkin-tools
sudo apt install doxygen doxygen-latex
sudo apt install liburdfdom-dev liboctomap-dev libassimp-dev
sudo apt-get install ros-noetic-rqt-multiplot
sudo apt install ros-noetic-grid-map*
sudo apt-get install lmpfr-dev

DUCKLING_PATH_WS=~/duckling_ws
DUCKLING_PATH=$DUCKLING_PATH_WS/src/ugly-duckling-robot
OCS2_PATH=$DUCKLING_PATH/third_party

#setupp elevation mapping
cd $OCS2_PATH/src/elevation_mapping_cupy
pip3 install -r requirements.txt

# setup ocs2 onnx
cd /tmp
wget https://github.com/microsoft/onnxruntime/releases/download/v1.7.0/onnxruntime-linux-x64-1.7.0.tgz
tar xf onnxruntime-linux-x64-1.7.0.tgz
mkdir -p ~/.local/bin ~/.local/include/onnxruntime ~/.local/lib ~/.local/share/cmake/onnxruntime
rsync -a /tmp/onnxruntime-linux-x64-1.7.0/include/ ~/.local/include/onnxruntime
rsync -a /tmp/onnxruntime-linux-x64-1.7.0/lib/ ~/.local/lib
rsync -a $OCS2_PATH/ocs2_mpcnet/ocs2_mpcnet_core/misc/onnxruntime/cmake/ ~/.local/share/cmake/onnxruntime

cd ~/duckling_ws
# TODO(nathan): remove segmented_planes_terrain_model after fixing SignedDistanceField 
catkin config --blacklist ocs2_doc segmented_planes_terrain_model ocs2_quadruped_interface ocs2_quadruped_loopshaping_interface ocs2_anymal_mpc ocs2_anymal_loopshaping_mpc ocs2_raisim ocs2_legged_robot_raisim ocs2_raisim_core ocs2_raisim_ros ocs2_mpcnet ocs2_mpcnet_core ocs2_ballbox_mpcnet ocs2_legged_robot_mpcnet