#ifndef ROBOTS
#define ROBOTS

#include <ros/ros.h>
#include <iostream>

#include <robots/RobotJoints.h>
#include <robots/MoveRobot.h>
#include <std_msgs/Int32.h>
#include <std_msgs/Bool.h>
#include <std_msgs/Byte.h>
#include <robots/ColorMsg.h>
#include "robots/DoTaskMsg.h"
#include "robots/Msg_numrobot.h"
#include "commande_locale/SrvAddProduct.h"
#include "commande_locale/Msg_AddProduct.h"
#include "commande/DeplacerPieceMsg.h"

// Etats robot
#define HAUT		1
#define BAS		-1
#define FERMEE		1
#define OUVERTE		-1
#define EN_POSITION	1

// Robots
#define ROBOT_1 1
#define ROBOT_2 2
#define ROBOT_3 3
#define ROBOT_4 4

// Postes
#define POSTE_1 1
#define POSTE_2 2
#define POSTE_3 3
#define POSTE_4 4
#define POSTE_5 5
#define POSTE_6 6
#define POSTE_7 7
#define POSTE_8 8


class Robots
{
private:

	ros::Publisher pub_robot_position;
	ros::Publisher pub_robot_joints;
	ros::Publisher pub_pince_fermer;
	ros::Publisher pub_pince_ouvrir;
	ros::Publisher pub_pince_descendre;
	ros::Publisher pub_descendre;
	ros::Publisher pub_monter;
	ros::Publisher pub_controler_robot;
	ros::Publisher pub_colorer;
	ros::Publisher pub_doTask;
	ros::Publisher pub_robot_transport1;
	ros::Publisher pub_robot_transport2;
	ros::Publisher pub_robot_transport3;
	ros::Publisher pub_robot_transport4;
	ros::Publisher pub_evacuer_piece;
	ros::Publisher pubProductAdd;
	ros::Publisher pub_deplacer_piece;

	ros::Subscriber sub_retourRobot;

	ros::ServiceClient client;
	commande_locale::SrvAddProduct srv;
	commande_locale::Msg_AddProduct msg0;
	commande::DeplacerPieceMsg deplacer_msg;

	int robotInit[4];
	int robotPosition[4];
	int robotBras[4];
	int bras[4];
	int robotPince[4];
	int pince[4];
	int robotTask[4][2];
	int robotMacroDeplacement[4];

	robots::ColorMsg msgColor;
	robots::DoTaskMsg tache_msg;

public:
	Robots(ros::NodeHandle noeud);
	~Robots();

	//Contrôles des robots
	void EnvoyerPosition(int numRobot, int numPosition);
	void EnvoyerAngles(int numRobot, int angle1, int angle2, int angle3, int angle4, int angle5, int angle6, int angle7);
	void ControlerRobot(int numRobot, int numPosition, int bras, int pince);
	void FermerPince(int numRobot);
	void OuvrirPince(int numRobot);
	void DescendreBras(int numRobot);
	void MonterBras(int numRobot);

	void DeplacerPiece(int num_robot, int positionA, int positionB);

	//Retour depuis les nodes des robots
	void RetourRobotCallback(const robots::Msg_numrobot::ConstPtr& msg);

	int RobotInitialise(int numRobot); //retourne 1 si le robot numRobot est initialisé
	int RobotEnPosition(int numRobot); //retourne 1 si le robot numRobot est en position
	int BrasEnPosition(int numRobot);  //retourne 1 si le bras du robot numRobot est monté, -1 si le bras du robot numRobot est descendu
	int PinceEnPosition(int numRobot); //retourne 1 si la pince du robot numRobot est fermée, -1 si elle est ouverte

	void Colorer(int num_robot, int position, int type);
	void RobotTransport(int num_robot, bool valeur);
	void DoTask(int num_poste, int duree);
	int IsTaskOver(int num_poste);
	void computeFromNumPoste(int num_poste,int tab[2]);
	int FinDeplacerPiece(int num_robot);
	void Evacuer();
	void AjouterProduit(int poste, int produit);
};
#endif
