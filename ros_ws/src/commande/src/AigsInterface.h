/**** Projet long N7 2020 ****/

#ifndef AIGSINTERFACE
#define AIGSINTERFACE

#include <ros/ros.h>
#include <iostream>

//Messages
#include <std_msgs/Int32.h>
#include "aiguillages/Msg_Aiguillage.h"


class AigsInterface
{
private:
	//Publishers


	ros::Publisher pub_aig_Droite;

	ros::Publisher pub_aig_Gauche;

	//Attributs
	std_msgs::Int32 msg;
	aiguillages::Msg_Aiguillage msg_aigs;

public:
	AigsInterface(ros::NodeHandle noeud);
	~AigsInterface();

	//Contrôles des aiguillages
	void Droite(int num_Aig);
	void Gauche(int num_Aig);

};
#endif
