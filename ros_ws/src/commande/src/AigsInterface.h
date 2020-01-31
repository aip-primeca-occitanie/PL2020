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


	ros::Publisher pub_aig_droit1, pub_aig_droit2, pub_aig_droit3, pub_aig_droit4, pub_aig_droit5, pub_aig_droit6, pub_aig_droit7, pub_aig_droit8, pub_aig_droit9, pub_aig_droit10, pub_aig_droit11, pub_aig_droit12;
	
	ros::Publisher pub_aig_virage1, pub_aig_virage2, pub_aig_virage3, pub_aig_virage4, pub_aig_virage5, pub_aig_virage6, pub_aig_virage7, pub_aig_virage8, pub_aig_virage9, pub_aig_virage10, pub_aig_virage11, pub_aig_virage12;

	//Attributs
	std_msgs::Int32 msg;

public:
	AigsInterface(ros::NodeHandle noeud);
	~AigsInterface();
	
	//Contrôles des aiguillages
	void Droit(int num_Aig);
	void Virage(int num_Aig);

};
#endif
