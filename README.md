# AUV-Seesaw-Project

軟體組小專題 – 平衡球([https://hackmd.io/bwQgfSxHSui3MNc3rfpiHw?view](https://hackmd.io/bwQgfSxHSui3MNc3rfpiHw?view))

ROS Tutorial([https://industrial-training-master.readthedocs.io/en/melodic/index.html](https://industrial-training-master.readthedocs.io/en/melodic/index.html))

orca_ros_bridge([https://github.com/NCTU-AUV/orca_ros_bridge](https://github.com/NCTU-AUV/orca_ros_bridge))

## To Run ROS on RPI (or Test It on Your Computer)

```bash
docker pull dongdonghsu065/ros_test
docker run -itd --name container_on_rpi --hostname rpi_host dongdonghsu065/ros_test
docker exec -it container_on_rpi bash

sudo apt update
sudo apt install git

git clone https://github.com/DianYueGuo/AUV-Seesaw-Project.git
cd /AUV-Seesaw-Project/catkin_ws_on_rpi/

catkin_make

roscore
```

To another terminal window:

```bash
docker exec -it container_on_rpi bash
cd /AUV-Seesaw-Project/catkin_ws_on_rpi/
source devel/setup.bash
rosrun controller_pkg controller
```

To run controller_tester, open another terminal window:

```bash
docker exec -it container_on_rpi bash
cd /AUV-Seesaw-Project/catkin_ws_on_rpi/
source devel/setup.bash
rosrun controller_pkg controller_tester
```

## To use ros bridge(暫定)

Usage
1.build container(沒記錯的話在rpi上不需要這行)
```bash
./script/build.sh
```
2.execute bridge (ensure ros1 core is running)
```bash
./script/run
```

範例使用 --- 需開四個terminal

terminal 1: 開 ros1 roscore
1.連到敞篷主機(在rpi上使用這步可略過)
2.source /opt/ros/noetic/setup.bash
3.roscore
```bash
ssh auv
source /opt/ros/noetic/setup.bash
roscore
```

terminal 2: 開 bridge:
1.連到敞篷主機
2.cd workspaces/ros2_ws/src/orca_ros_bridge/
3./scripts/run
(會出現running bridge)
```bash
ssh auv
cd workspaces/ros2_ws/src/orca_ros_bridge/
./scripts/run
```

terminal 3: run ros2(talker)
1.連到敞篷主機
2.docker run -it --rm --net=host --pid=host orca_ros2
會出現/workspaces/ros2_ws$的指令列
3.ros2 run program_name talker
```bash
ssh auv
docker run -it --rm --net=host --pid=host orca_ros2
$ros2 run demo_nodes_cpp(for example) talker
```
理論上talker端會一直發送hello world 的訊息

terminal 4: run ros1(listener)
1.連ssh auv
2.source /opt/ros/noetic/setup.bash
3.rosrun program_name listener
```bash
ssh auv
source /opt/ros/noetic/setup.bash
rosrun rospy_tutorials(for example) listener
```
理論上listener端會一直接收來自talker端hello world 的訊息

ps.前兩個terminal 我在rpi試過了 是可行的 後兩個當時rpi上還沒裝ros2我
不確定能不能跑 但應該可行 可能需要微調
