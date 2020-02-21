#include <iostream>
#include "vrepController.h"
#include "inOutController.h"
#include "commande_locale/SrvAddProduct.h"
#include "commande_locale/Msg_AddProduct.h"
#include <unistd.h>
#include <thread>
#include <ros/ros.h>

using namespace std;

vrepController VREPController;
commande_locale::Msg_AddProduct msg0;

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
	VREPController.addProduct(req.choixProduit,req.choixPoste);
	return true;
}

void SpawnShuttlesCallback(const std_msgs::Int32::ConstPtr& msg)
{
	for(int i=0; i<msg->data; i++) // i<nbShuttles
	{
		VREPController.loadModelInit(i);
	}
}

int main(int argc, char **argv)
{
	//Initialisation du noeud ROS
	ros::init(argc, argv, "commande_locale");
	ros::NodeHandle nh;

	ros::Publisher pubProductAdd= nh.advertise<commande_locale::Msg_AddProduct>("/commande_locale/AddProduct",10);
	ros::Subscriber sub_spawnShuttles = nh.subscribe("/commande_locale/nbNavettes",10,SpawnShuttlesCallback);

	ros::ServiceServer service = nh.advertiseService("srv_add_product", AddProduct);

	ROS_INFO("Simulation file: %s \n", argv[1]);

	// VREP CONTROLLER
	VREPController.init(nh,argv[0], argv[1]);

	// IN & OUT CONTROLLER
	inOutController IOController(&VREPController);
	IOController.init(nh);

	sleep(3);

	cout << "Pause envoyée" << endl;
	VREPController.pause();





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
					msg0.num_poste = choixPoste;
					msg0.num_produit = choixProduit*10+4;
					pubProductAdd.publish(msg0);
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
