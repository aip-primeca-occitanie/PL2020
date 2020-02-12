#include <ros/ros.h>
#include <unistd.h>
#include <vector>
#include "Aiguillage.h"
#include <iostream>

using namespace std;

int main(int argc, char **argv)
{
	ros::init(argc, argv, "main_aiguillage");
	ros::NodeHandle noeud;

	vector<Aiguillage*> liste_aiguillage;//0 aiguillage fantôme
	Aiguillage* pointeur_aig;

	for (int i=1; i<13; i++)
	{
		pointeur_aig=new Aiguillage(noeud, i);
		//on créé les aiguillages de 1 à 12
		liste_aiguillage.push_back(pointeur_aig);
	}

	ros::Rate loop_rate(25); //fréquence de la boucle

	while (ros::ok())
	{
		ros::spinOnce(); //permet aux fonction callback de ros dans les objets d'êtres appelées
		loop_rate.sleep(); //permet de synchroniser la boucle while. Il attend le temps qu'il reste pour faire le 25Hz (ou la fréquence indiquée dans le loop_rate)
	}

	return 0;
}
