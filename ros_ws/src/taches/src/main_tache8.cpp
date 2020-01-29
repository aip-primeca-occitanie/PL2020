/**** Projet long N7 2017 ****/

#include "tache.h"

#include <ros/ros.h>


int main(int argc, char **argv)
{
	//Initialisation du noeud ROS
	ros::init(argc,argv,"tache8");
	ros::NodeHandle nh;

	Tache Tache8(8);
	Tache8.init(nh,argv[0]);

	ros::Rate loop_rate(25);
        while(ros::ok())
        {
                ros::spinOnce();
                loop_rate.sleep();
        }

}
