#include <ros/ros.h>
#include <unistd.h>
#include "Aiguillage.h"

using namespace std;

int main(int argc, char **argv)
{
	ros::init(argc, argv, "aiguillage_10");	//numéro de l'aiguillage
	ros::NodeHandle noeud;
	Aiguillage Aig(noeud,10);
	ros::Rate loop_rate(25); //fréquence de la boucle

	while (ros::ok())
	{
		ros::spinOnce(); //permet aux fonction callback de ros dans les objets d'êtres appelées
		loop_rate.sleep(); //permet de synchroniser la boucle while. Il attend le temps qu'il reste pour faire le 25Hz (ou la fréquence indiquée dans le loop_rate)
	}
	
	return 0;
}
