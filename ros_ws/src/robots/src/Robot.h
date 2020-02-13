/**** Projet long N7 2020 ****/

#ifndef ROBOT
#define ROBOT

#include <ros/ros.h>

//Messages
#include <robots/RobotJoints.h>
#include <robots/MoveRobot.h>
#include <robots/DoTaskMsg.h>
#include <robots/ColorMsg.h>
#include <std_msgs/Int32.h>
#include <std_msgs/String.h>
#include <std_msgs/Float32MultiArray.h>
#include <std_msgs/Int32MultiArray.h>
#include <std_msgs/Byte.h>
#include <std_msgs/Bool.h>
#include <std_msgs/Float32.h>
#include <sensor_msgs/JointState.h>
#include <robots/Msg_numrobot.h>

#include <vector>
#include <sstream>
#include <iostream>
#include <stdlib.h>
#include <string>
#include <unistd.h>
#include "Poste.h"


class Robot
{
private:

	/** Topic V-Rep **/
	// Le type Byte est utilisé lorsque le contenu du message n'est pas utile

	// GetObjectHandle
	ros::Publisher pubSim_getObjectHandle;
	std_msgs::String msgSim_getObjectHandle;
	ros::Subscriber subSim_getObjectHandle;
	bool repSim_getObjectHandle;
	int valueSim_getObjectHandle;

	// SetJointState
	ros::Publisher pubSim_setJointState;
	std_msgs::Float32MultiArray msgSim_setJointState;
	ros::Subscriber subSim_setJointState;
	bool repSim_setJointState;

	// GetJointState
	ros::Publisher pubSim_getJointState;
	std_msgs::Int32 msgSim_getJointState;
	ros::Subscriber subSim_getJointState;
	bool repSim_getJointState;
	float valueSim_getJointState;

	// GetTime
	ros::Publisher pubSim_getTime;
	std_msgs::Byte msgSim_getTime;
	ros::Subscriber subSim_getTime;
	bool repSim_getTime;
	float valueSim_getTime;

	// GetTimeUpdate
	ros::Publisher pubSim_getTimeUpdate;
	std_msgs::Byte msgSim_getTimeUpdate;
	ros::Subscriber subSim_getTimeUpdate;
	bool repSim_getTimeUpdate;
	float valueSim_getTimeUpdate;

	// ChangeColor
	ros::Publisher pubSim_changeColor;
	std_msgs::Int32MultiArray msgSim_changeColor;
	ros::Subscriber subSim_changeColor;
	bool repSim_changeColor;

	// ChangeShuttleColor
	ros::Publisher pubSim_changeShuttleColor;
	std_msgs::Int32MultiArray msgSim_changeShuttleColor;
	ros::Subscriber subSim_changeShuttleColor;
	bool repSim_changeShuttleColor;

	// GetColor
	ros::Publisher pubSim_getColor;
	std_msgs::String msgSim_getColor;
	ros::Subscriber subSim_getColor;
	bool repSim_getColor;
	int valueSim_getColor;

	/** Subscribers aux topics de la commande **/
	ros::Subscriber planifSendPosition;
	ros::Subscriber planifSendJoints;
	ros::Subscriber planifFermerPince;
	ros::Subscriber planifOuvrirPince;
	ros::Subscriber planifDescendreBras;
	ros::Subscriber planifMonterBras;
	ros::Subscriber planifControlerRobot;
	ros::Subscriber sub_colorer;
	ros::Subscriber sub_doTask;
	ros::Subscriber sub_nouveau_produit;
	ros::Subscriber sub_evacuer;

	/** Publishers pour retours **/
	ros::Publisher pub_pince;
	ros::Publisher pub_robotPosition, pub_robotBras, pub_robotPince;
	ros::Publisher pub_retourCommande;

	ros::Publisher pub_robot_transport;

	/** Variables **/
	float pi;
	unsigned char mymodes[7];
	float Rpos[7];
	int Rints[7];
	int couleur_transportee[4];
	// numero du robot
	int num_robot;
	ros::Rate* loop_rate;
	ros::Rate* loop_ok;

	/** Messages **/
	std_msgs::Int32 robotPosition;
	std_msgs::Int32 robotBras;
	std_msgs::Int32 robotPince;
	std_msgs::Int32 retour;

	int produit_sur_poste;
	Poste poste_pos_1;
	Poste poste_pos_4;


public:

	Robot(int num_du_robot);
	~Robot();


	//Initialisation
	void init(ros::NodeHandle noeud);

	int computeTableId(int position);
	void transport(bool valeur);
	void update();

	/** Fonctions internes permettant le contrôle du robot **/
	//Pour atteindre une position prédéfinie
	void EnvoyerRobot(int position);

	//Pour atteindre une position définie manuellement
	void EnvoyerJoints(int joint1, int joint2, int joint3, int joint4, int joint5, int joint6, int joint7);

	//Pour monter ou descendre le bras
	void DescendreBras();
	void MonterBras();

	//Pour ouvrir ou fermer la pince
	void FermerPince();
	void OuvrirPince();


	/** Fonctions permettant de controler le robot avec des ordres du noeud commande **/
	//Pour atteindre une position prédéfinie
	void SendPositionCallback(const robots::Msg_numrobot::ConstPtr& msg);

	//Pour atteindre une position définie manuellement
	void SendJointsCallback(const robots::RobotJoints::ConstPtr& msg);

	//Pour monter ou descendre le bras
	void DescendreBrasCallback(const robots::Msg_numrobot::ConstPtr& msg);
	void MonterBrasCallback(const robots::Msg_numrobot::ConstPtr& msg);

	//Pour ouvrir ou fermer la pince
	void FermerPinceCallback(const robots::Msg_numrobot::ConstPtr& msg);
	void OuvrirPinceCallback(const robots::Msg_numrobot::ConstPtr& msg);

	//Pour contrôler l'ensemble des mouvements du robot
	void ControlerRobotCallback(const robots::MoveRobot::ConstPtr& msg);
	void ColorerCallback(const robots::ColorMsg::ConstPtr& msg);
	void doTaskCallback(const robots::DoTaskMsg::ConstPtr& msg);
	void ajouter_produitCallback(commande_locale::Msg_AddProduct msg);

	void colorerPosteTask(string poste, int couleur_poste);
	void Evacuer(const std_msgs::Byte::ConstPtr& msg);

	// Callbacks pour V-Rep
	void simGetObjectHandleCallback(const std_msgs::Int32::ConstPtr& msg);
	void simSetJointStateCallback(const std_msgs::Byte::ConstPtr& msg);
	void simGetJointStateCallback(const sensor_msgs::JointState::ConstPtr& msg);
	void simGetTimeCallback(const std_msgs::Float32::ConstPtr& msg);
	void simGetTimeUpdateCallback(const std_msgs::Float32::ConstPtr& msg);
	void simChangeColorCallback(const std_msgs::Byte::ConstPtr& msg);
	void simChangeShuttleColorCallback(const std_msgs::Byte::ConstPtr& msg);
	void simGetColorCallback(const std_msgs::Int32::ConstPtr& msg);
};


#endif
