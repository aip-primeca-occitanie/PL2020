/**** Maxime Maurin - Projet Long N7 2017 ****/

#ifndef ORDO
#define ORDO

#include <ros/ros.h>
#include <string>
#include <std_msgs/Int32.h>
#include <std_msgs/Bool.h>
#include <std_msgs/String.h>
#include <std_msgs/Byte.h>
#include <std_msgs/Float32.h>
#include <cstdlib>
#include <stdlib.h>
#include <map>
#include <vector>
#include <fstream>
#include <iostream>
#include <cstring>
#include <shuttles/srvGetShuttleStatus.h>

#include "scheduling/Msg_LoadShuttle.h"
#include "product.h"

//La classe Scheduler permet la gestion de l'ordonnacement, elle s'initialise par rapport au fichier de configuration


class Scheduler
{
private:

	int nbLoop;
	int deltaLoop;
	int nbNavettes;
	int scheduledLaunchDate[10];
	int numberOfProduct;
	int nextCount;
	int finEnvoi;
	int productNumber;
	int compteur;
	float lastLaunchDate;
	bool tableDispo;
	bool init_var;

	std::string nom_produits[100];

	std::string configFile, logFile;


	std::map<int,Product*>::iterator iteratorPMap,iteratorPMapOut;
	std::map<int,Product*> ProductsMap; // la clé est le numéro de la phase de lancement

	ros::Publisher pubCreateShuttle, pubDelShuttle;
	ros::Publisher pubNombreDeProduits;
	ros::Publisher pubNomProduits;
	ros::Publisher pubProductAddTable;
	ros::Publisher pubNbShuttle;
	std_msgs::Int32 msgShuttle;
	ros::Subscriber subEndOfProductManufacture;
	ros::Subscriber subProduitsPris;

	ros::Publisher pubSim_GetTime;
	ros::Subscriber subSim_GetTime;
	bool repSim_GetTime;
	float valueSim_GetTime;

	ros::ServiceClient client_GetShuttleStatus;
	shuttles::srvGetShuttleStatus srv_GetShuttleStatus;

	ros::Subscriber sub_DemandeNbrSh;

	ros::Rate* loop_rate;

public:

	Scheduler();
	~Scheduler();

	bool init(ros::NodeHandle nh, std::string executionPath);

	void initProduct(std::string pName, int pFirstDestination, int initProductColor, int pManRSize, int order);

	void launchNextSchedule();

	void productOutCallBack(const std_msgs::Int32::ConstPtr& msg);

	void tableUpdateCallback(const std_msgs::Bool::ConstPtr& msg);
	void SendNbrShuttleCallback(const std_msgs::Int32::ConstPtr& msg);

	void SimGetTimeCallback(const std_msgs::Float32::ConstPtr& msg);
};


#endif
