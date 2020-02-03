/**** Projet long N7 2020 ****/

#ifndef AIGSINTERFACE
#define AIGSINTERFACE

#include <ros/ros.h>
#include <iostream>

//Messages
#include <std_msgs/Int32.h>


class AigsInterface
{
private:
	//Publishers


	ros::Publisher pub_aig_Droite1, pub_aig_Droite2, pub_aig_Droite3, pub_aig_Droite4, pub_aig_Droite5, pub_aig_Droite6, pub_aig_Droite7, pub_aig_Droite8, pub_aig_Droite9, pub_aig_Droite10, pub_aig_Droite11, pub_aig_Droite12;

	ros::Publisher pub_aig_Gauche1, pub_aig_Gauche2, pub_aig_Gauche3, pub_aig_Gauche4, pub_aig_Gauche5, pub_aig_Gauche6, pub_aig_Gauche7, pub_aig_Gauche8, pub_aig_Gauche9, pub_aig_Gauche10, pub_aig_Gauche11, pub_aig_Gauche12;

	//Attributs
	std_msgs::Int32 msg;

public:
	AigsInterface(ros::NodeHandle noeud);
	~AigsInterface();

	//Contrôles des aiguillages
	void Droite(int num_Aig);
	void Gauche(int num_Aig);

};
#endif
