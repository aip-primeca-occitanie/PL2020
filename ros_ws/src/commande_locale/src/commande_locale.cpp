using namespace std;

#include <iostream>
#include "vrepController.h"
#include "inOutController.h"
#include "configuration.h"
#include "commande_locale/SrvAddProduct.h"
#include <unistd.h>
#include <thread>

#include <ros/ros.h>

vrepController VREPController;
Configuration config(&VREPController);



void spinner()
{
	ros::Rate loop_rate(25);
	while(ros::ok())
	{
		ros::spinOnce();
		loop_rate.sleep();
	}
}

bool AddProduct(commande_locale::SrvAddProduct::Request &req, commande_locale::SrvAddProduct::Response &res)
{
	config.ProductAddTable(req.choixProduit,req.choixPoste);
	//rappel, code produit A:14, B:24, C:34 etc.
	VREPController.addProduct(req.choixProduit,req.choixPoste);
	return true;
}

int main(int argc, char **argv)
{
	//Initialisation du noeud ROS
	ros::init(argc, argv, "commande_locale");
	ros::NodeHandle nh;

	ros::ServiceServer service = nh.advertiseService("srv_add_product", AddProduct);

	ROS_INFO("Simulation file: %s \n", argv[1]);

	// VREP CONTROLLER
	VREPController.init(nh,argv[0], argv[1]);

	// CONFIGURATION
	config.init(nh, argv[0]);

	// IN & OUT CONTROLLER
	inOutController IOController(&VREPController, &config);
	IOController.init(nh);

	sleep(3);

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

	thread spinnerThread(spinner);

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
			"	3- Play simu" 	<< endl;
		cout << "Choix : ";
		cin >> choix;
		if(cin.fail())
		{
			cout << endl << " [Erreur mauvais choix ..]" << endl;
			cin.clear();
			cin.ignore(256,'\n');
		}
		else
		{
			cout << endl;
			switch(choix)
			{
				case 1:
					cout << "Ajout de produit : quel poste ? [1..4]" << endl;
					cin >> choixPoste;
					if(cin.fail() || choixPoste<1 || choixPoste>4)
					{
						cout << endl << " [Erreur mauvais choix ..]" << endl;
						cin.clear();
						cin.ignore(256,'\n');
						break;
					}
					cout << "Quel produit ? [1..6]" << endl;
					cin >> choixProduit;	
					if(cin.fail() || choixProduit<1 || choixProduit>6)
					{
						cout << endl << " [Erreur mauvais choix ..]" << endl;	
						cin.clear();
						cin.ignore(256,'\n');
						break;
					}
					config.ProductAddTable(choixProduit,choixPoste);
					VREPController.addProduct(choixProduit,choixPoste);
					break;

				case 2:
					cout << "Mise en pause de la simu" << endl;
					VREPController.pause();
					break;

				case 3:
					cout << "Mise en Play de la simu" << endl;
					VREPController.play();
					break;

				default:
					cout << endl << " [Erreur mauvais choix ..]" << endl;
					break;
			}
		}
	}


	VREPController.close();
	return 0;
}
