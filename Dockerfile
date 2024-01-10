FROM ros:eloquent-ros1-bridge

# setup env
ENV ROS_MASTER_URI=http://localhost:11311
CMD ros2 run ros1_bridge dynamic_bridge
