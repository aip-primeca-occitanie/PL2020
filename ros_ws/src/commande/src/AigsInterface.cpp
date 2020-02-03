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
	 pub_aig_droit1=noeud.advertise<std_msgs::Int32>("/commande/Simulation/AiguillageDroit1",1);
	 pub_aig_droit2=noeud.advertise<std_msgs::Int32>("/commande/Simulation/AiguillageDroit2",1);
	 pub_aig_droit3=noeud.advertise<std_msgs::Int32>("/commande/Simulation/AiguillageDroit3",1);
	 pub_aig_droit4=noeud.advertise<std_msgs::Int32>("/commande/Simulation/AiguillageDroit4",1);
	 pub_aig_droit5=noeud.advertise<std_msgs::Int32>("/commande/Simulation/AiguillageDroit5",1);
	 pub_aig_droit6=noeud.advertise<std_msgs::Int32>("/commande/Simulation/AiguillageDroit6",1);
	 pub_aig_droit7=noeud.advertise<std_msgs::Int32>("/commande/Simulation/AiguillageDroit7",1);
	 pub_aig_droit8=noeud.advertise<std_msgs::Int32>("/commande/Simulation/AiguillageDroit8",1);
	 pub_aig_droit9=noeud.advertise<std_msgs::Int32>("/commande/Simulation/AiguillageDroit9",1);
	 pub_aig_droit10=noeud.advertise<std_msgs::Int32>("/commande/Simulation/AiguillageDroit10",1);
	 pub_aig_droit11=noeud.advertise<std_msgs::Int32>("/commande/Simulation/AiguillageDroit11",1);
	 pub_aig_droit12=noeud.advertise<std_msgs::Int32>("/commande/Simulation/AiguillageDroit12",1);


	 pub_aig_virage1=noeud.advertise<std_msgs::Int32>("/commande/Simulation/AiguillageVirage1",1);
	 pub_aig_virage2=noeud.advertise<std_msgs::Int32>("/commande/Simulation/AiguillageVirage2",1);
	 pub_aig_virage3=noeud.advertise<std_msgs::Int32>("/commande/Simulation/AiguillageVirage3",1);
	 pub_aig_virage4=noeud.advertise<std_msgs::Int32>("/commande/Simulation/AiguillageVirage4",1);
	 pub_aig_virage5=noeud.advertise<std_msgs::Int32>("/commande/Simulation/AiguillageVirage5",1);
	 pub_aig_virage6=noeud.advertise<std_msgs::Int32>("/commande/Simulation/AiguillageVirage6",1);
	 pub_aig_virage7=noeud.advertise<std_msgs::Int32>("/commande/Simulation/AiguillageVirage7",1);
	 pub_aig_virage8=noeud.advertise<std_msgs::Int32>("/commande/Simulation/AiguillageVirage8",1);
	 pub_aig_virage9=noeud.advertise<std_msgs::Int32>("/commande/Simulation/AiguillageVirage9",1);
	 pub_aig_virage10=noeud.advertise<std_msgs::Int32>("/commande/Simulation/AiguillageVirage10",1);
	 pub_aig_virage11=noeud.advertise<std_msgs::Int32>("/commande/Simulation/AiguillageVirage11",1);
	 pub_aig_virage12=noeud.advertise<std_msgs::Int32>("/commande/Simulation/AiguillageVirage12",1);
	/*** Subscribers ***/
}



//Destructeur
AigsInterface::~AigsInterface()
{

}

//Methodes
void AigsInterface::Droit(int num_Aig)
{

	switch(num_Aig)
	{

		case 1:
			pub_aig_droit1.publish(msg);
			break;
		case 2:
			pub_aig_droit2.publish(msg);
			break;
		case 3:
			pub_aig_droit3.publish(msg);
			break;
		case 4:
			pub_aig_droit4.publish(msg);
			break;
		case 5:
			pub_aig_droit5.publish(msg);
			break;
		case 6:
			pub_aig_droit6.publish(msg);
			break;
		case 7:
			pub_aig_droit7.publish(msg);
			break;
		case 8:
			pub_aig_droit8.publish(msg);
			break;
		case 9:
			pub_aig_droit9.publish(msg);
			break;
		case 10:
			pub_aig_droit10.publish(msg);
			break;
		case 11:
			pub_aig_droit11.publish(msg);
			break;
		case 12:
			pub_aig_droit12.publish(msg);
			break;
	}
}

void AigsInterface::Virage(int num_Aig)
{
	switch(num_Aig)
	{

		case 1:
			pub_aig_virage1.publish(msg);
			break;
		case 2:
			pub_aig_virage2.publish(msg);
			break;
		case 3:
			pub_aig_virage3.publish(msg);
			break;
		case 4:
			pub_aig_virage4.publish(msg);
			break;
		case 5:
			pub_aig_virage5.publish(msg);
			break;
		case 6:
			pub_aig_virage6.publish(msg);
			break;
		case 7:
			pub_aig_virage7.publish(msg);
			break;
		case 8:
			pub_aig_virage8.publish(msg);
			break;
		case 9:
			pub_aig_virage9.publish(msg);
			break;
		case 10:
			pub_aig_virage10.publish(msg);
			break;
		case 11:
			pub_aig_virage11.publish(msg);
			break;
		case 12:
			pub_aig_virage12.publish(msg);
			break;
	}
}
