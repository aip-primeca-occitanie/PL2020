#include <ros/ros.h>
#include <ros/package.h>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <image_transport/image_transport.h>
#include <cv_bridge/cv_bridge.h>
#include <iostream>

using namespace cv;
using namespace std;

cv::Mat imageSimu;

void update()
{
    imshow( "Display window", imageSimu );                   // Show our image inside it.
    waitKey(30);                                          // Wait for a keystroke in the window
}

void getSimuStream(const sensor_msgs::ImageConstPtr& msg)
{
	imageSimu = cv_bridge::toCvShare(msg, "bgr8")->image;
	update();
}

int main(int argc, char **argv)
{
	ros::init(argc, argv, "display_node");//numéro de l'aiguillage
	ros::NodeHandle nh;

    image_transport::Subscriber subImage;	// Subscriber pour recuperer l'image depuis la simu V-rep
    image_transport::ImageTransport it(nh);
	subImage = it.subscribe("sim_ros_interface/VisionSensorData", 1, &getSimuStream);

    namedWindow( "Display window", WINDOW_AUTOSIZE );// Create a window for display.

	ros::Rate loop_rate(25); //fréquence de la boucle

	while (ros::ok())
	{
		ros::spinOnce(); //permet aux fonction callback de ros dans les objets d'êtres appelées
		loop_rate.sleep(); //permet de synchroniser la boucle while. Il attend le temps qu'il reste pour faire le 25Hz (ou la fréquence indiquée dans le loop_rate)
	}

	return 0;
}
