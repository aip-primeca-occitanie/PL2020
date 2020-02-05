/**** Maxime Maurin - Projet Long N7 2017 ****/

#ifndef CONFIG
#define CONFIG

#include <ros/ros.h>
#include <string>
#include <std_msgs/Int32.h>
#include <std_msgs/Bool.h>
#include <std_msgs/Byte.h>
#include <std_msgs/Float32.h>
#include <cstdlib>
#include <stdlib.h>
#include <map>
#include <vector>
#include <fstream>
#include <iostream>
#include <cstring>

#include "product.h"
#include "vrepController.h"

// La classe Configuration permet la génération manuel de produit en accord avec la configuration établit dans le fichier.config

class Configuration
{
private:
	int maxShuttleNumber;
	vrepController* vrepCAcces;

	std::string configFile, logFile;

	std::map<std::string,Product*>::iterator iteratorPMap;
	std::map<std::string,Product*> ProductsMap;

	ros::Publisher pubSim_GetTime;
	ros::Subscriber subSim_GetTime;
	bool repSim_GetTime;
	float valueSim_GetTime;

	ros::Publisher pubManualProduct;

	ros::Rate* loop_rate;


public:
	int numberOfProduct;
	bool activeProduct[7];
	
	
	Configuration(vrepController* VREPC);
	~Configuration();
	
	bool init(ros::NodeHandle nh, std::string executionPath);
	
	void initProduct(std::string pName, int pFirstDestination, int initProductNumber, int pManRSize);
	void ProductAdd(char shuttleLetter, int product);

	void SimGetTimeCallback(const std_msgs::Float32::ConstPtr& msg);
	
};


#endif
