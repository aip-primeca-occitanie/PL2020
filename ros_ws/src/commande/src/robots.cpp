/**** Projet long N7 2017 ****/

#include "robots.h"

#include <ros/ros.h>
#include <iostream>
#include <unistd.h>

using namespace std;

//Messages
#include <robots/RobotJoints.h>
#include <std_msgs/Int32.h>

//Couleur des messages des robots
#define RESET   "\033[0m"
#define BOLDCYAN    "\033[1m\033[36m"      /* Bold Cyan */
#define BOLDMAGENTA "\033[1m\033[35m"      /* Bold Magenta */



//Constructeur
Robots::Robots(ros::NodeHandle noeud)
{

	//Mise à 0 des retours des robots et des traitements
	for(int i=0;i<4;i++)
	{
		robotInit[i]=0;
		robotBras[i]=0;
		robotPosition[i]=0;

		bras[i]=-10;
		pince[i]=-10;
	}


	/** Publishers **/
	//Positions prédéfinies
	pub_robot_position=noeud.advertise<robots::Msg_numrobot>("/commande/Simulation/SendPositionRobot",1);


	//Positions manuelles
	pub_robot_joints=noeud.advertise<robots::RobotJoints>("/commande/Simulation/SendJointsRobot",1);

	//Fermer les pinces
	pub_pince_fermer=noeud.advertise<robots::Msg_numrobot>("/commande/Simulation/FermerPinceRobot",1);

	//Ouvrir les pinces
	pub_pince_ouvrir=noeud.advertise<robots::Msg_numrobot>("/commande/Simulation/OuvrirPinceRobot",1);
	//Descendre les bras
	pub_descendre=noeud.advertise<robots::Msg_numrobot>("/commande/Simulation/DescendreBras",1);

	//Descendre les bras
	pub_monter=noeud.advertise<robots::Msg_numrobot>("/commande/Simulation/MonterBras",1);

	//Contrôler les robots
	pub_controler_robot=noeud.advertise<robots::MoveRobot>("/commande/Simulation/ControlerBras",10);

	pub_colorer=noeud.advertise<robots::ColorMsg>("/commande/Simulation/Colorer",10);
	pub_doTask=noeud.advertise<robots::DoTaskMsg>("/commande/Simulation/doTask", 10);



	/*** Subscribers ***/
	//Retour des robots vers la commande
	sub_retourRobot1 = noeud.subscribe("/commande/Simulation/retourCommande1", 100, &Robots::RetourRobot1Callback,this);
	sub_retourRobot2 = noeud.subscribe("/commande/Simulation/retourCommande2", 100, &Robots::RetourRobot2Callback,this);
	sub_retourRobot3 = noeud.subscribe("/commande/Simulation/retourCommande3", 100, &Robots::RetourRobot3Callback,this);
	sub_retourRobot4 = noeud.subscribe("/commande/Simulation/retourCommande4", 100, &Robots::RetourRobot4Callback,this);
	
	sleep(1);
}



//Destructeur
Robots::~Robots()
{}



/*** Envoyer les robots automatiquement ***/
//Fonction pour envoyer un robot choisi dans une position prédéfinie
void Robots::EnvoyerPosition(int numRobot, int numPosition)
{
	//Déclaration du message
	robots::Msg_numrobot msg;
	
	//Numéro de la position souhaitée
	if(numPosition<1 || numPosition>4)
	{
		cout <<  BOLDMAGENTA << "Le numero de la position souhaitee doit etre compris entre 1 et 4." << RESET << endl;
	}
	else
	{
		msg.data = numPosition;
	}
	msg.num_robot=numRobot;
	//Publication du message vers le node robot en fonction du numéro de robot à commander
	switch(numRobot)
	{
		case 1:
			robotPosition[0]=-10;
			pub_robot_position.publish(msg);  // pub_robot_position1 est un publisher qui publie dans le topic /commande/Simulation/SendPositionRobot1
			break;

		case 2:
			robotPosition[1]=-10;
			pub_robot_position.publish(msg);
			break;

		case 3:
			robotPosition[2]=-10;
			pub_robot_position.publish(msg);
			break;

		case 4:
			robotPosition[3]=-10;
			pub_robot_position.publish(msg);
			break;

		default:
			cout <<  BOLDMAGENTA << "Le numero du robot doit etre compris entre 1 et 4." << RESET << endl;
			break;
	}
}



/*** Envoyer les robots manuellement ***/
//Fonction pour envoyer un robot choisi dans une position définie par l'utilisateur
void Robots::EnvoyerAngles(int numRobot, int angle1, int angle2, int angle3, int angle4, int angle5, int angle6, int angle7)
{
	//Déclaration du message
	robots::RobotJoints msg;

	//Angles (en degrés) choisis par l'utilisateur
	msg.joint1 = angle1;
	msg.joint2 = angle2;
	msg.joint3 = angle3;
	msg.joint4 = angle4;
	msg.joint5 = angle5;
	msg.joint6 = angle6;
	msg.joint7 = angle7;
	msg.num_robot=numRobot;

	//Publication du message vers le node robot en fonction du numéro de robot à commander
	switch(numRobot)
	{
		case 1:
			robotPosition[0]=-10;
			pub_robot_joints.publish(msg);
			break;

		case 2:
			robotPosition[1]=-10;
			pub_robot_joints.publish(msg);
			break;

		case 3:
			robotPosition[2]=-10;
			pub_robot_joints.publish(msg);
			break;

		case 4:
			robotPosition[3]=-10;
			pub_robot_joints.publish(msg);
			break;

		default:
			cout <<  BOLDMAGENTA << "Le numero du robot doit etre compris entre 1 et 4." << RESET << endl;
			break;
	}
}



/*** Contrôler tous les mouvements des robots ***/
//Fonction permettant d'envoyer une position prédéfinie, l'état souhaité du bras et l'état souhaité de la pince d'un robot choisi
void Robots::ControlerRobot(int numRobot, int numPosition, int bras, int pince)
{
	robots::MoveRobot controle; //robots::MoveRobot est une strcture contenue dans le namespace robots (voir fichier MoveRobot.h dans le dossier include/robots)

	controle.position = numPosition; // cette strcture (MoveRobot) contient 3 variables, postion, bras et pince
	controle.bras = bras;
	controle.pince = pince;
	controle.num_robot=numRobot;


	if(numPosition<1 || numPosition>4)
	{
		cout <<  BOLDMAGENTA << "Le numero de la position doit etre compris entre 1 et 4." << RESET << endl; // on a que 4 position possibles, ces position sont 1 2 3 et 4 et ont étés definies dans un autre fichier
	}

	if(bras != -1 && bras != 1)
	{
		cout <<  BOLDMAGENTA << "Cet etat du bras est inaccessible." << RESET << endl; // on a que 2 états du bras possibles, ces états sont -1 et +1 voir fichier
	}

	if(pince != -1 && pince != 1)
	{
		cout <<  BOLDMAGENTA << "Cet etat de la pince est inaccessible." << RESET << endl; // on a que 2 états de pince possibles, ces états sont -1 et +1 voir fichier
	}

	switch(numRobot)         // on fait un switch case celon le numéro du robot eton publie dans les topics correspondant au 4 robots disponibles
	{
		case 1:
			robotPosition[0]=-10;
			pub_controler_robot.publish(controle); //  pub_controler_robot1 est un publisher qui publie dans le topic /commande/Simulation/retourCommande1 le subscriber est Robot1
			break;

		case 2:
			robotPosition[1]=-10;
			pub_controler_robot.publish(controle);
			break;

		case 3:
			robotPosition[2]=-10;
			pub_controler_robot.publish(controle);
			break;

		case 4:
			robotPosition[3]=-10;
			pub_controler_robot.publish(controle);
			break;

		default:
			cout <<  BOLDMAGENTA << "Le numero du robot doit etre compris entre 1 et 4." << RESET << endl;
			break;
	}
}



/*** Fermer les pinces ***/
//Fonction pour fermer la pince d'un robot choisi
void Robots::FermerPince(int numRobot)
{
	//Déclaration du message
	robots::Msg_numrobot msg;
	msg.data=1;
	msg.num_robot=numRobot;
	//Publication du message vers le node robot en fonction du numéro de robot à commander
	switch(numRobot)
	{
		case 1:
			pub_pince_fermer.publish(msg);
			break;

		case 2:
			pub_pince_fermer.publish(msg);
			break;

		case 3:
			pub_pince_fermer.publish(msg);
			break;

		case 4:
			pub_pince_fermer.publish(msg);
			break;

		default:
			cout <<  BOLDMAGENTA << "Le numero du robot doit etre compris entre 1 et 4." << RESET << endl;
			break;
	}
}



/*** Ouvrir les pinces ***/
//Fonction pour ouvrir la pince d'un robot choisi
void Robots::OuvrirPince(int numRobot)
{
	//Déclaration du message
	robots::Msg_numrobot msg;
	msg.data=0;
	msg.num_robot=numRobot;
	//Publication du message vers le node robot en fonction du numéro de robot à commander
	switch(numRobot)
	{
		case 1:
			pub_pince_ouvrir.publish(msg);
			break;

		case 2:
			pub_pince_ouvrir.publish(msg);
			break;

		case 3:
			pub_pince_ouvrir.publish(msg);
			break;

		case 4:
			pub_pince_ouvrir.publish(msg);
			break;

		default:
			cout <<  BOLDMAGENTA << "Le numero du robot doit etre compris entre 1 et 4." << RESET << endl;
			break;
	}
}



/*** Descendre les bras ***/
//Fonction pour descendre le bras d'un robot choisi
void Robots::DescendreBras(int numRobot)
{
	//Déclaration du message
	robots::Msg_numrobot msg;
	msg.data=1;
	msg.num_robot=numRobot;
	//Publication du message vers le node robot en fonction du numéro de robot à commander
	switch(numRobot)
	{
		case 1:
			pub_descendre.publish(msg);
			break;

		case 2:
			pub_descendre.publish(msg);
			break;

		case 3:
			pub_descendre.publish(msg);
			break;

		case 4:
			pub_descendre.publish(msg);
			break;

		default:
			cout <<  BOLDMAGENTA << "Le numero du robot doit etre compris entre 1 et 4." << RESET << endl;
			break;
	}
}



/*** Monter les bras ***/
//Fonction pour monter le bras d'un robot choisi
void Robots::MonterBras(int numRobot)
{
	//Déclaration du message
	robots::Msg_numrobot msg;
	msg.data=0;
	msg.num_robot=numRobot;

	//Publication du message vers le node robot en fonction du numéro de robot à commander
	switch(numRobot)
	{
		case 1:
			pub_monter.publish(msg);
			break;

		case 2:
			pub_monter.publish(msg);
			break;

		case 3:
			pub_monter.publish(msg);
			break;

		case 4:
			pub_monter.publish(msg);
			break;

		default:
			cout <<  BOLDMAGENTA << "Le numero du robot doit etre compris entre 1 et 4." << RESET << endl;
			break;
	}
}




/*** Retour des robots ***/
//Fonction Callback pour le retour des informations relatives au robot 1
void Robots::RetourRobot1Callback(const std_msgs::Int32::ConstPtr& msg)
{
	int retour;
	retour=msg->data;

	switch(retour)
	{
		case 0:
			cout <<  BOLDCYAN << "Robot 1 initialise" << RESET << endl;
			robotInit[0]=1;
			break;

		case 1:
			cout <<  BOLDCYAN << "Mouvement non effectue pour le robot 1" << RESET << endl;
			robotPosition[0]=0;
			break;

		case 2:
			cout <<  BOLDCYAN << "Robot 1 en position" << RESET << endl;
			robotPosition[0]=1;
			break;

		case 3:
			cout <<  BOLDCYAN << "Bras bloque pour le robot 1" << RESET << endl;
			robotBras[0] = 0;
			break;

		case 4:
			cout << BOLDCYAN << "Bras descendu pour le robot 1" << RESET << endl;
			robotBras[0] = -1;
			break;

		case 5:
			cout << BOLDCYAN << "Bras monte pour le robot 1" << RESET << endl;
			robotBras[0] = 1;
			break;

		case 6:
			cout << BOLDCYAN << "Pince fermee pour le robot 1" << RESET << endl;
			robotPince[0] = 1;
			break;

		case 7:
			cout << BOLDCYAN << "Pince ouverte pour le robot 1" << RESET << endl;
			robotPince[0] = -1;
			break;
	}
}


//Fonction Callback pour le retour des informations relatives au robot 2
void Robots::RetourRobot2Callback(const std_msgs::Int32::ConstPtr& msg)
{
	int retour;
	retour=msg->data;

	switch(retour)
	{
		case 0:
			cout <<  BOLDCYAN << "Robot 2 initialise" << RESET << endl;
			robotInit[1]=1;
			break;

		case 1:
			cout <<  BOLDCYAN << "Mouvement non effectue pour le robot 2" << RESET << endl;
			robotPosition[1]=0;
			break;

		case 2:
			cout <<  BOLDCYAN << "Robot 2 en position" << RESET << endl;
			robotPosition[1]=1;
			break;

		case 3:
			cout <<  BOLDCYAN << "Bras bloque pour le robot 2" << RESET << endl;
			robotBras[1] = 0;
			break;

		case 4:
			cout << BOLDCYAN << "Bras descendu pour le robot 2" << RESET << endl;
			robotBras[1] = -1;
			break;

		case 5:
			cout << BOLDCYAN << "Bras monte pour le robot 2" << RESET << endl;
			robotBras[1] = 1;
			break;

		case 6:
			cout << BOLDCYAN << "Pince fermee pour le robot 2" << RESET << endl;
			robotPince[1] = 1;
			break;

		case 7:
			cout << BOLDCYAN << "Pince ouverte pour le robot 2" << RESET << endl;
			robotPince[1] = -1;
			break;
	}
}


//Fonction Callback pour le retour des informations relatives au robot 3
void Robots::RetourRobot3Callback(const std_msgs::Int32::ConstPtr& msg)
{
	int retour;
	retour=msg->data;

	switch(retour)
	{
		case 0:
			cout <<  BOLDCYAN << "Robot 3 initialise" << RESET << endl;
			robotInit[2]=1;
			break;

		case 1:
			cout <<  BOLDCYAN << "Mouvement non effectue pour le robot 3" << RESET << endl;
			robotPosition[2]=0;
			break;

		case 2:
			cout <<  BOLDCYAN << "Robot 3 en position" << RESET << endl;
			robotPosition[2]=1;
			break;

		case 3:
			cout <<  BOLDCYAN << "Bras bloque pour le robot 3" << RESET << endl;
			robotBras[2] = 0;
			break;

		case 4:
			cout << BOLDCYAN << "Bras descendu pour le robot 3" << RESET << endl;
			robotBras[2] = -1;
			break;

		case 5:
			cout << BOLDCYAN << "Bras monte pour le robot 3" << RESET << endl;
			robotBras[2] = 1;
			break;

		case 6:
			cout << BOLDCYAN << "Pince fermee pour le robot 3" << RESET << endl;
			robotPince[2] = 1;
			break;

		case 7:
			cout << BOLDCYAN << "Pince ouverte pour le robot 3" << RESET << endl;
			robotPince[2] = -1;
			break;
	}
}


//Fonction Callback pour le retour des informations relatives au robot 4
void Robots::RetourRobot4Callback(const std_msgs::Int32::ConstPtr& msg)
{
	int retour;
	retour=msg->data;

	switch(retour)
	{
		case 0:
			cout <<  BOLDCYAN << "Robot 4 initialise" << RESET << endl;
			robotInit[3]=1;
			break;

		case 1:
			cout <<  BOLDCYAN << "Mouvement non effectue pour le robot 4" << RESET << endl;
			robotPosition[3]=0;
			break;

		case 2:
			cout <<  BOLDCYAN << "Robot 4 en position" << RESET << endl;
			robotPosition[3]=1;
			break;

		case 3:
			cout <<  BOLDCYAN << "Bras bloque pour le robot 4" << RESET << endl;
			robotBras[3] = 0;
			break;

		case 4:
			cout << BOLDCYAN << "Bras descendu pour le robot 4" << RESET << endl;
			robotBras[3] = -1;
			break;

		case 5:
			cout << BOLDCYAN << "Bras monte pour le robot 4" << RESET << endl;
			robotBras[3] = 1;
			break;

		case 6:
			cout << BOLDCYAN << "Pince fermee pour le robot 4" << RESET << endl;
			robotPince[3] = 1;
			break;

		case 7:
			cout << BOLDCYAN << "Pince ouverte pour le robot 4" << RESET << endl;
			robotPince[3] = -1;
			break;
	}
}



//Fonction permettant de savoir si le robot choisi est initalisé
int Robots::RobotInitialise(int numRobot)
{
	int Robot;

	ros::spinOnce();

	if(numRobot<1 || numRobot>4)
	{
		cout <<  BOLDMAGENTA << "Le numero du robot doit etre compris entre 1 et 4." << RESET << endl;
	}
	else
	{
		Robot=robotInit[numRobot-1];
	}
	return Robot;
}



//Fonction permettant de savoir si le robot choisi est en position
int Robots::RobotEnPosition(int numRobot)
{
	int Robot;

	ros::spinOnce();

	if(numRobot<1 || numRobot>4)
	{
		cout <<  BOLDMAGENTA << "Le numero du robot doit etre compris entre 1 et 4." << RESET << endl;
	}
	else
	{
		Robot=robotPosition[numRobot-1];
	}

	return Robot;
}



//Fonction permettant de savoir si le bras du robot choisi est en position
int Robots::BrasEnPosition(int numRobot)
{
	int Robot;

	ros::spinOnce();

	if(numRobot<1 || numRobot>4)
	{
		cout <<  BOLDMAGENTA << "Le numero du robot doit etre compris entre 1 et 4." << RESET << endl;
	}
	else
	{
		Robot=robotBras[numRobot-1];
		robotBras[numRobot-1]=-10;

		if((Robot != 0)&&(Robot != -1)&&(Robot != 1))
		{
			Robot=bras[numRobot-1];
		}

		bras[numRobot-1] = Robot;
	}

	return Robot;
}



//Fonction permettant de savoir si la pince du robot choisi est en position
int Robots::PinceEnPosition(int numRobot)
{
	int Robot;

	ros::spinOnce();

	if(numRobot<1 || numRobot>4)
	{
		cout <<  BOLDMAGENTA << "Le numero du robot doit etre compris entre 1 et 4." << RESET << endl;
	}
	else
	{
		Robot=robotPince[numRobot-1];
		robotPince[numRobot-1]=-10;

		if((Robot != 0)&&(Robot != -1)&&(Robot != 1))
		{
			Robot=pince[numRobot-1];
		}

		pince[numRobot-1] = Robot;
	}

	return Robot;
}

void Robots::DeplacerPiece(int num_robot, int positionA, int positionB)
{
	if ((positionA<5 && positionA>0)&&(positionB<5 && positionB>0))
	{
		EnvoyerPosition(num_robot,positionA);
		while(RobotEnPosition(num_robot)!=1){usleep(100000);};
		DescendreBras(num_robot);
		while(BrasEnPosition(num_robot)!=-1){usleep(100000);};
		FermerPince(num_robot);
		while(PinceEnPosition(num_robot)!=1){usleep(100000);};

		//ici on considère qu'on a pris une pièce
		//decoloration, donc
		Colorer(num_robot,positionA);//le robot a rien en mémoire, il décolore

		MonterBras(num_robot);
		while(BrasEnPosition(num_robot)!=1){usleep(100000);};
		EnvoyerPosition(num_robot,positionB);
		while(RobotEnPosition(num_robot)!=1){usleep(100000);};
		DescendreBras(num_robot);
		while(BrasEnPosition(num_robot)!=-1){usleep(100000);};
		OuvrirPince(num_robot);
		while(PinceEnPosition(num_robot)!=-1){usleep(100000);};

		//ici on considère qu'on a déposé la pièce
		//coloration, donc
		Colorer(num_robot,positionB);//le robot a la couleur du produit en memoire, il colore
		MonterBras(num_robot);
		while(BrasEnPosition(num_robot)!=1){usleep(100000);};
	}
}

void Robots::Colorer(int num_robot,int position)
{
	msgColor.num_robot=num_robot;
	msgColor.position=position;
	pub_colorer.publish(msgColor);

	usleep(1200000);
}


void Robots::DoTask(int num_robot, int position, int num_tache)
{
	tache_msg.num_robot=num_robot;
	tache_msg.position=position;
	tache_msg.num_tache=num_tache;
	pub_doTask.publish(tache_msg);
}
