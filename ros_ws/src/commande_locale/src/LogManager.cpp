#include <ros/ros.h>
#include <ros/package.h>
#include <std_msgs/Int32MultiArray.h>
#include <iostream>

using namespace std;
void ProduitEvac(std_msgs::Int32MultiArray msg)
{
    cout <<"J'ai bien recu le msg de l'evac";
}


int main(int argc, char **argv)
{
	ros::init(argc, argv, "log_manager");
	ros::NodeHandle nh;

    //ros::Subscriber subTacheFinie;
    ros::Subscriber subProduitEvac;
    //ros::Subscriber subErreurTraitement;

    //subTacheFinie = nh.subscribe("", 1, &Tachefinie);
    subProduitEvac = nh.subscribe("/commande/Simulation/produitEvac", 1, &ProduitEvac);
    //subErreurTraitement = nh.subscribe("", 1, &ErreurTraitement);

    sleep(2);

	ros::Rate loop_rate(25); //fréquence de la boucle

	while (ros::ok())
	{
		ros::spinOnce(); //permet aux fonction callback de ros dans les objets d'êtres appelées
		loop_rate.sleep(); //permet de synchroniser la boucle while. Il attend le temps qu'il reste pour faire le 25Hz (ou la fréquence indiquée dans le loop_rate)
	}

	return 0;
}
