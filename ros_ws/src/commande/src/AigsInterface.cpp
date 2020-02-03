/**** Projet long N7 2017 ****/

#include "AigsInterface.h"

#include <ros/ros.h>
#include <iostream>

using namespace std;

//Messages
#include <std_msgs/Int32.h>

//Couleur des messages des aiguillages
#define RESET   "\033[0m"
#define BOLDCYAN    "\033[1m\033[36m"      /* Bold Cyan */
#define BOLDMAGENTA "\033[1m\033[35m"      /* Bold Magenta */


//Constructeur
AigsInterface::AigsInterface(ros::NodeHandle noeud)
{
	msg.data=1;

	/** Publishers **/
	 pub_aig_Droite1=noeud.advertise<std_msgs::Int32>("/commande/Simulation/AiguillageDroite1",1);
	 pub_aig_Droite2=noeud.advertise<std_msgs::Int32>("/commande/Simulation/AiguillageDroite2",1);
	 pub_aig_Droite3=noeud.advertise<std_msgs::Int32>("/commande/Simulation/AiguillageDroite3",1);
	 pub_aig_Droite4=noeud.advertise<std_msgs::Int32>("/commande/Simulation/AiguillageDroite4",1);
	 pub_aig_Droite5=noeud.advertise<std_msgs::Int32>("/commande/Simulation/AiguillageDroite5",1);
	 pub_aig_Droite6=noeud.advertise<std_msgs::Int32>("/commande/Simulation/AiguillageDroite6",1);
	 pub_aig_Droite7=noeud.advertise<std_msgs::Int32>("/commande/Simulation/AiguillageDroite7",1);
	 pub_aig_Droite8=noeud.advertise<std_msgs::Int32>("/commande/Simulation/AiguillageDroite8",1);
	 pub_aig_Droite9=noeud.advertise<std_msgs::Int32>("/commande/Simulation/AiguillageDroite9",1);
	 pub_aig_Droite10=noeud.advertise<std_msgs::Int32>("/commande/Simulation/AiguillageDroite10",1);
	 pub_aig_Droite11=noeud.advertise<std_msgs::Int32>("/commande/Simulation/AiguillageDroite11",1);
	 pub_aig_Droite12=noeud.advertise<std_msgs::Int32>("/commande/Simulation/AiguillageDroite12",1);


	 pub_aig_Gauche1=noeud.advertise<std_msgs::Int32>("/commande/Simulation/AiguillageGauche1",1);
	 pub_aig_Gauche2=noeud.advertise<std_msgs::Int32>("/commande/Simulation/AiguillageGauche2",1);
	 pub_aig_Gauche3=noeud.advertise<std_msgs::Int32>("/commande/Simulation/AiguillageGauche3",1);
	 pub_aig_Gauche4=noeud.advertise<std_msgs::Int32>("/commande/Simulation/AiguillageGauche4",1);
	 pub_aig_Gauche5=noeud.advertise<std_msgs::Int32>("/commande/Simulation/AiguillageGauche5",1);
	 pub_aig_Gauche6=noeud.advertise<std_msgs::Int32>("/commande/Simulation/AiguillageGauche6",1);
	 pub_aig_Gauche7=noeud.advertise<std_msgs::Int32>("/commande/Simulation/AiguillageGauche7",1);
	 pub_aig_Gauche8=noeud.advertise<std_msgs::Int32>("/commande/Simulation/AiguillageGauche8",1);
	 pub_aig_Gauche9=noeud.advertise<std_msgs::Int32>("/commande/Simulation/AiguillageGauche9",1);
	 pub_aig_Gauche10=noeud.advertise<std_msgs::Int32>("/commande/Simulation/AiguillageGauche10",1);
	 pub_aig_Gauche11=noeud.advertise<std_msgs::Int32>("/commande/Simulation/AiguillageGauche11",1);
	 pub_aig_Gauche12=noeud.advertise<std_msgs::Int32>("/commande/Simulation/AiguillageGauche12",1);
	/*** Subscribers ***/
}



//Destructeur
AigsInterface::~AigsInterface()
{

}

//Methodes
void AigsInterface::Droite(int num_Aig)
{

	switch(num_Aig)
	{

		case 1:
			pub_aig_Droite1.publish(msg);
			break;
		case 2:
			pub_aig_Droite2.publish(msg);
			break;
		case 3:
			pub_aig_Droite3.publish(msg);
			break;
		case 4:
			pub_aig_Droite4.publish(msg);
			break;
		case 5:
			pub_aig_Droite5.publish(msg);
			break;
		case 6:
			pub_aig_Droite6.publish(msg);
			break;
		case 7:
			pub_aig_Droite7.publish(msg);
			break;
		case 8:
			pub_aig_Droite8.publish(msg);
			break;
		case 9:
			pub_aig_Droite9.publish(msg);
			break;
		case 10:
			pub_aig_Droite10.publish(msg);
			break;
		case 11:
			pub_aig_Droite11.publish(msg);
			break;
		case 12:
			pub_aig_Droite12.publish(msg);
			break;
	}
}

void AigsInterface::Gauche(int num_Aig)
{
	switch(num_Aig)
	{

		case 1:
			pub_aig_Gauche1.publish(msg);
			break;
		case 2:
			pub_aig_Gauche2.publish(msg);
			break;
		case 3:
			pub_aig_Gauche3.publish(msg);
			break;
		case 4:
			pub_aig_Gauche4.publish(msg);
			break;
		case 5:
			pub_aig_Gauche5.publish(msg);
			break;
		case 6:
			pub_aig_Gauche6.publish(msg);
			break;
		case 7:
			pub_aig_Gauche7.publish(msg);
			break;
		case 8:
			pub_aig_Gauche8.publish(msg);
			break;
		case 9:
			pub_aig_Gauche9.publish(msg);
			break;
		case 10:
			pub_aig_Gauche10.publish(msg);
			break;
		case 11:
			pub_aig_Gauche11.publish(msg);
			break;
		case 12:
			pub_aig_Gauche12.publish(msg);
			break;
	}
}
