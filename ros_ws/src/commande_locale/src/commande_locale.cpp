/*
 * ********************************* *
 * Copyright 2016, STEC Projet Long. *
 * All rights reserved.  	     *
 * ********************************* *
 * Mise à jour par le Projet Long    *
 * ENSEEIHT 2017		     *
 * ********************************* *
*/


using namespace std;

#include <iostream>
#include "vrepController.h"
#include "inOutController.h"
#include "configuration.h"
#include <unistd.h>
#include <thread>

#include <ros/ros.h>

int main(int argc, char **argv)
{
	//Initialisation du noeud ROS
	ros::init(argc, argv, "commande_locale");
	ros::NodeHandle nh;

	ROS_INFO("Simulation file: %s \n", argv[1]);

	// VREP CONTROLLER
	vrepController VREPController;
	VREPController.init(nh,argv[0], argv[1]);

	// CONFIGURATION
	Configuration config(&VREPController);
	config.init(nh, argv[0]);

	// IN & OUT CONTROLLER
	inOutController IOController(&VREPController, &config);
	IOController.init(nh);

	cout << "Pause envoyée" << endl;
	VREPController.pause();
	// Pause pour laisser à l'utilisateur le soin de lancer la simu avec le boutton Play

	//Creation des navettes
	int nbNavettes;
	nbNavettes = config.getNbNavettes();
	ros::Publisher pubCreateShuttle = nh.advertise<scheduling::Msg_LoadShuttle>("/scheduling/NextProduct",10);
	scheduling::Msg_LoadShuttle mymsg;

	if (nbNavettes>=1)
	{
	mymsg.shuttleType = 'A';
	mymsg.product = 0;
	pubCreateShuttle.publish(mymsg);
	std::cout << "Creation navette 1" << endl;
	}

	if (nbNavettes>=2)
	{
	mymsg.shuttleType = 'B';
	mymsg.product = 0;
	pubCreateShuttle.publish(mymsg);
	std::cout << "Creation navette 2" << endl;
	}

	if (nbNavettes>=3)
	{
	mymsg.shuttleType = 'C';
	mymsg.product = 0;
	pubCreateShuttle.publish(mymsg);
	std::cout << "Creation navette 3" << endl;
	}

	if (nbNavettes>=4)
	{
	mymsg.shuttleType = 'D';
	mymsg.product = 0;
	pubCreateShuttle.publish(mymsg);
	std::cout << "Creation navette 4" << endl;
	}

	if (nbNavettes>=5)
	{
	mymsg.shuttleType = 'E';
	mymsg.product = 0;
	pubCreateShuttle.publish(mymsg);
	std::cout << "Creation navette 5" << endl;
	}

	if (nbNavettes>=6)
	{
	mymsg.shuttleType = 'F';
	mymsg.product = 0;
	pubCreateShuttle.publish(mymsg);
	std::cout << "Creation navette 6" << endl;
	}
	///////////////////////

	int choix=0;
	while(ros::ok())
	{
		cout << endl << endl << endl;
		cout << "Que voulez faire ?" 	<< endl <<
			"	1- Ajouter un produit" << endl <<
			"	2- Pause simu" 	<< endl <<
			"	3- Play simu" 		<< endl <<
			"	4- Changer de mode"	<< endl;
		cout << "Choix : ";
		cin >> choix;
		cout << endl;
		switch(choix)
		{
			case 1:
				cout << "ajout de produit" << endl;
				//configuration->ProductAddTable(trucs);
				break;

			case 2:
				cout << "Mise en pause de la simu" << endl;
				VREPController.pause();
				break;

			case 3:
				cout << "Mise en Play de la simu" << endl;
				VREPController.play();
				break;

			case 4:
				cout << "On change de mode" << endl;
				break;

			default:
				cout << "mauvais choix .." << endl;
				break;
		}
	}


	VREPController.close();
	return 0;
}
