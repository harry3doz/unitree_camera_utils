#include <iostream>
#include <signal.h>
#include <cmath>
#include <cerrno>
#include <cfenv>
#include <unistd.h>

#include <UnitreeCameraSDK.hpp>

#include <ros/ros.h>
#include <sensor_msgs/Image.h>
#include <image_transport/image_transport.h>
#include <cv_bridge/cv_bridge.h>

int main(int argc, char *argv[]){

    std::string node_name;
    if (argc < 3)
    {
      node_name = std::string("unitree_camera_node");
    }
    else
    {
      node_name = std::string(argv[2]);
    }
    //double offsetTime = static_cast<double>(std::atof(argv[3]));


    ros::init(argc, argv, node_name);

    ros::NodeHandle nh("~");

    std::string config_file;
    nh.getParam("config_file", config_file);
    std::string run_config_file;
    nh.param<std::string>("run_config_file", run_config_file, "");

    std::string cameraConfig;
    if (config_file.size() != 0)
      cameraConfig = config_file;
    else if (run_config_file.size() != 0)
      cameraConfig = run_config_file;

    std::cout << "Camera Config File:" << cameraConfig << std::endl;


    image_transport::ImageTransport it(nh);


    UnitreeCamera cam(cameraConfig);  ///< init camera by device node number
    if(!cam.isOpened())
        exit(EXIT_FAILURE);

    int camPosNum = cam.getPosNumber();
    std::string topicName = "image_";
    switch(camPosNum)
    {
    case 1:{topicName += "face";} break;
    case 2:{topicName += "chin";} break;
    case 3:{topicName += "left";} break;
    case 4:{topicName += "right";} break;
    case 5:{topicName += "rearDown";} break;
    default:{;}
    }

    std::cout << "Camera PositionNumber -> " << camPosNum << "topic name ->" << topicName << std::endl;

    image_transport::Publisher camera_pub = it.advertise(topicName, 1);
    ros::Rate loop_rate(5);
    
    cam.startCapture();            ///< start camera capturing

    while(ros::ok() && cam.isOpened()){
        sensor_msgs::ImagePtr img_msg;
        cv::Mat frame;
        std::chrono::microseconds t;

        if(!cam.getRawFrame(frame, t)){ ///< get camera raw image
            usleep(1000);
            continue;
        }
 	cv_bridge::CvImagePtr cv_image =
          boost::make_shared<cv_bridge::CvImage>(std_msgs::Header(), "bgr8", frame);

	img_msg = cv_image->toImageMsg();

	camera_pub.publish(img_msg);

	ros::spinOnce();

	loop_rate.sleep();

    }
    
    cam.stopCapture();  ///< stop camera capturing
    
    return 0;
}
