/**** Projet long N7 2017 ****/

#include "robot4.h"

#include <ros/ros.h>


int main(int argc, char **argv)
{
	//Initialisation du noeud ROS
	ros::init(argc,argv,"robot4");
	ros::NodeHandle nh;

	//CmdRobot
	Robot4 Robot4;
	Robot4.init(nh);

	ros::Rate loop_rate(25);
	while(ros::ok())
	{
		ros::spinOnce();
		loop_rate.sleep();
	}
}
