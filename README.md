# AUV-Seesaw-Project

軟體組小專題 – 平衡球([https://hackmd.io/bwQgfSxHSui3MNc3rfpiHw?view](https://hackmd.io/bwQgfSxHSui3MNc3rfpiHw?view))

ROS Tutorial([https://industrial-training-master.readthedocs.io/en/melodic/index.html](https://industrial-training-master.readthedocs.io/en/melodic/index.html))

orca_ros_bridge([https://github.com/NCTU-AUV/orca_ros_bridge](https://github.com/NCTU-AUV/orca_ros_bridge))

## To Run ROS on RPI (or Test It on Your Computer)

'''bash
docker pull dongdonghsu065/ros_test
docker run -itd --name container_on_rpi --hostname rpi_host dongdonghsu065/ros_test
docker exec -it container_on_rpi bash

sudo apt update
sudo apt install git

git clone -b wrap-source-code-with-ros-workspace https://github.com/DianYueGuo/AUV-Seesaw-Project.git
cd /AUV-Seesaw-Project/catkin_ws_on_rpi/

catkin_make

roscore
'''

To another terminal window:

'''bash
docker exec -it container_on_rpi bash
cd /AUV-Seesaw-Project/catkin_ws_on_rpi/
source devel/setup.bash
rosrun controller_pkg controller
'''