#!/bin/bash
eval echo "[unitree_camera_utils]" >> .startlog
SCRIPT_DIR=$(cd $(dirname $0); pwd)
ipLastSegment=15
export SUDO_ASKPASS=passwd.sh



localIP=`cat /etc/network/interfaces | grep "address" | awk '{print $2}'`
export ROS_MASTER_URI=http://192.168.123.161:11311
export ROS_IP=$localIP
source ../../devel/setup.bash
patchlines=$(cat patch | wc -l)

sudo -A ntpdate -d 192.168.123.161 > offset
timeoffset=$(cat offset | grep "filter offset:" | awk '{print $3}')
rm offset

if [ "${patchlines}" == "1" ]; then
    camNum=$(cat patch | head -n 1 | awk '{print $2}')
    rosrun unitree_camera_utils point_cloud_node _run_config_file:=${SCRIPT_DIR}/config/stereo_camera_config.yaml camera${camNum} ${timeoffset} &
    sleep 5
elif [ "${patchlines}" == "2" ]; then
    camNum1=$(cat patch | head -n 1 | awk '{print $2}')
    camNum2=$(cat patch | tail -n 1 | awk '{print $2}')
    if [ "${camNum1}" == "2" ]; then
        rosrun unitree_camera_utils example_point ${SCRIPT_DIR}/config/stereo_camera_config.yaml &
    else
        rosrun unitree_camera_utils point_cloud_node _run_config_file:=${SCRIPT_DIR}/config/stereo_camera_config.yaml camera${camNum1} ${timeoffset} &
    fi
    sleep 5
    if [ "${camNum2}" == "2" ]; then
        rosrun unitree_camera_utils example_point ${SCRIPT_DIR}/config/stereo_camera_config1.yaml &
    else
        rosrun unitree_camera_utils point_cloud_node _run_config_file:=${SCRIPT_DIR}/config/stereo_camera_config1.yaml camera${camNum2} ${timeoffset} &
    fi
    sleep 5
fi
