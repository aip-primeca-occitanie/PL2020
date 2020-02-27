/**** Projet long N7 2017 ****/

#include "Robot.h"

#include <ros/ros.h>

void ShutdownCallback(const std_msgs::Byte::ConstPtr& msg)
{
		ros::shutdown();
}

int main(int argc, char **argv)
{
	//Initialisation du noeud ROS
	ros::init(argc,argv,"robot4");
	ros::NodeHandle nh;
	ros::Subscriber sub_shutdown = nh.subscribe("/commande_locale/shutdown",10,&ShutdownCallback);

	//CmdRobot
	Robot robot4(4);
	robot4.init(nh);

	int compteur=0;
	ros::Rate loop_rate(25);
	while(ros::ok())
	{
		if(compteur++>25)
		{
			robot4.update();
			compteur=0;
		}
		ros::spinOnce();
		loop_rate.sleep();
	}
}
