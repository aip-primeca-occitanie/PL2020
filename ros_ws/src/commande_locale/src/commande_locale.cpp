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
	for(int i=0; i<nbNavettes; i++)
	{
		VREPController.loadModelInit(i);
		std::cout << "Creation navette " << i << endl;
	}
	
	///////////////////////

	int choix=0;
	int choixProduit=0;
	int choixPoste=0;
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
				cout << "ajout de produit : quel poste ?" << endl;
				cin >> choixPoste;
				cout << "Quel produit ?" << endl;
				cin >> choixProduit;
				config.ProductAddTable(choixProduit,choixPoste);
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
				cout << "On change de mode (fake news)" << endl;
				break;

			default:
				cout << "mauvais choix .." << endl;
				break;
		}
	}


	VREPController.close();
	return 0;
}
