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
	int NombreNavettes;
	int nbLoop;
	int deltaLoop;
	vrepController* vrepCAcces;

	std::string configFile;

	std::map<std::string,Product*>::iterator iteratorPMap;
	std::map<std::string,Product*> ProductsMap;
	
	ros::Publisher pubProductAdd;

	std_msgs::Int32 retourTraitement;

public:
	int numberOfProduct;
	bool activeProduct[7];

	Configuration(vrepController* VREPC);

	bool init(ros::NodeHandle nh, std::string executionPath);
	void ProductAddTable(int typeProduct, int poste);
	int getNbNavettes();
};


#endif
