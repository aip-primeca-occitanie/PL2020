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
	 pub_aig_Droite=noeud.advertise<aiguillages::Msg_Aiguillage>("/commande/Simulation/AiguillageDroite",100);

	 pub_aig_Gauche=noeud.advertise<aiguillages::Msg_Aiguillage>("/commande/Simulation/AiguillageGauche",100);
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
			msg_aigs.Aiguillage=1;
			break;
		case 2:
			msg_aigs.Aiguillage=2;
			break;
		case 3:
			msg_aigs.Aiguillage=3;
			break;
		case 4:
			msg_aigs.Aiguillage=4;
			break;
		case 5:
			msg_aigs.Aiguillage=5;
			break;
		case 6:
			msg_aigs.Aiguillage=6;
			break;
		case 7:
			msg_aigs.Aiguillage=7;
			break;
		case 8:
			msg_aigs.Aiguillage=8;
			break;
		case 9:
			msg_aigs.Aiguillage=9;
			break;
		case 10:
			msg_aigs.Aiguillage=10;
				break;
		case 11:
			msg_aigs.Aiguillage=11;
			break;
		case 12:
			msg_aigs.Aiguillage=12;
				break;
	}
	pub_aig_Droite.publish(msg_aigs);

}

void AigsInterface::Gauche(int num_Aig)
{
	switch(num_Aig)
	{

		case 1:
			msg_aigs.Aiguillage=1;
			break;
		case 2:
			msg_aigs.Aiguillage=2;
			break;
		case 3:
			msg_aigs.Aiguillage=3;
			break;
		case 4:
			msg_aigs.Aiguillage=4;
			break;
		case 5:
			msg_aigs.Aiguillage=5;
			break;
		case 6:
			msg_aigs.Aiguillage=6;
			break;
		case 7:
			msg_aigs.Aiguillage=7;
			break;
		case 8:
			msg_aigs.Aiguillage=8;
			break;
		case 9:
			msg_aigs.Aiguillage=9;
			break;
		case 10:
			msg_aigs.Aiguillage=10;
				break;
		case 11:
			msg_aigs.Aiguillage=11;
			break;
		case 12:
			msg_aigs.Aiguillage=12;
				break;
	}
	pub_aig_Gauche.publish(msg_aigs);
}
