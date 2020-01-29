/**** Projet long N7 2017 ****/

#include "Robot.h"

#include <ros/ros.h>


int main(int argc, char **argv)
{
	//Initialisation du noeud ROS
	ros::init(argc,argv,"robot1");
	ros::NodeHandle nh;

	//CmdRobot
	Robot robot1(1);
	robot1.init(nh);

	while(ros::ok())
	{
		ros::spinOnce();
	}
}
