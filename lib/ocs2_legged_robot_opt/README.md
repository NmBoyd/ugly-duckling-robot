
# model_hierarchy_mpc

## Dependencies

### OCS2 
https://github.gatech.edu/GeorgiaTechLIDARGroup/ocs2

### Cassie Drivers
Just include it as a package adjacent to model hierarchy mpc. (note: do not include yet. Nathan is still fixing some dependency issues with it when building the mpc)
```
git clone --recurse-submodules -b ros https://github.gatech.edu/GeorgiaTechLIDARGroup/cassie_software.git
```

## Compiling 
Currently we are dealing with global build issues due to pinocchio update dependencies. However we can still build the package by neglecting some pacakges and building only the desired mpc controller.
```
catkin build ocs2_legged_robot_opt
```
