cmake_minimum_required(VERSION 3.0.2)
project(unitree_camera_utils)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -pthread")

find_package(catkin REQUIRED COMPONENTS
  compressed_image_transport
  cv_bridge
  roscpp
  rospy
  std_msgs
  sensor_msgs
  pcl_ros
  message_generation
  #  camera_info_manager
)

catkin_package(
    #CATKIN_DEPENDS cv_bridge roscpp rospy std_msgs
    CATKIN_DEPENDS roscpp rospy std_msgs
)

find_package(OpenCV 4 REQUIRED)
if(OpenCV_FOUND)
    include_directories(${OpenCV_INCLUDE_DIR})
    message(STATUS "OpenCV ${OpenCV_VERSION} FOUND")
    message(STATUS ${OpenCV_INCLUDE_DIR})
    message(STATUS ${OpenCV_LIBS})
else()
    message(FATAL_ERROR "OpenCV Library Not Found")
endif()

set(SDKLIBS unitree_camera tstc_V4L2_xu_camera udev systemlog ${OpenCV_LIBS})

if(CMAKE_HOST_SYSTEM_PROCESSOR MATCHES "x86_64")
link_directories(${PROJECT_SOURCE_DIR}/lib/amd64/)
else()
link_directories(${PROJECT_SOURCE_DIR}/lib/arm64/)
endif()

include_directories(
    ${PROJECT_SOURCE_DIR}/include
    ${OpenCV_INCLUDE_DIRS}
    ${catkin_INCLUDE_DIRS}
)

#add_executable(rect_image_node src/rect_image_node.cc)
#add_dependencies(rect_image_node ${${PROJECT_NAME}_EXPORTED_TARGETS} ${catkin_EXPORTED_TARGETS})
#target_link_libraries(rect_image_node
#    ${catkin_LIBRARIES}
#    ${SDKLIBS}
#)

add_executable(pub_image src/pub_image.cc)
target_link_libraries(pub_image
    ${catkin_LIBRARIES}
    ${SDKLIBS}
)
add_executable(point_cloud_node src/point_cloud_node.cc)
target_link_libraries(point_cloud_node
    ${catkin_LIBRARIES}
    ${SDKLIBS}
)

add_executable(example_point src/example_point.cc)
target_link_libraries(example_point
    ${SDKLIBS}
)

