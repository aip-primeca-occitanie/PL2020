#include <iostream>
#include "vrepController.h"
#include "inOutController.h"
#include "commande_locale/SrvAddProduct.h"
#include "commande_locale/Msg_AddProduct.h"
#include "commande_locale/SrvFinInit.h"
#include "commande_locale/SrvAddProductPushBack.h"
#include <unistd.h>
#include <thread>
#include <ros/ros.h>
#include <std_msgs/Byte.h>


using namespace std;

vrepController VREPController;
commande_locale::Msg_AddProduct msg0;
bool initEnCours(true);

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

void ShutdownCallback(const std_msgs::Byte::ConstPtr& msg)
{
		ros::shutdown();
}

bool finInit(commande_locale::SrvFinInit::Request &req, commande_locale::SrvFinInit::Response &res)
{
	initEnCours=false;
	return true;
}

int main(int argc, char **argv)
{
	//Initialisation du noeud ROS
	ros::init(argc, argv, "commande_locale");
	ros::NodeHandle nh;

	ros::Publisher pubProductAdd= nh.advertise<commande_locale::Msg_AddProduct>("/commande_locale/AddProduct",10);
	ros::Subscriber sub_spawnShuttles = nh.subscribe("/commande_locale/nbNavettes",10,SpawnShuttlesCallback);

	ros::ServiceServer service = nh.advertiseService("srv_add_product", AddProduct);
	ros::ServiceClient clientAddProduct = nh.serviceClient<commande_locale::SrvAddProductPushBack>("srv_add_product_push_back");
	commande_locale::SrvAddProductPushBack srv;
	ros::ServiceServer serviceInit = nh.advertiseService("srv_fin_init", finInit);
	ros::Publisher pub_shutdown = nh.advertise<std_msgs::Byte>("/commande_locale/shutdown",10);
	ros::Subscriber sub_shutdown = nh.subscribe("/commande_locale/shutdown",10,ShutdownCallback);


	ROS_INFO("Simulation file: %s \n", argv[1]);

	// VREP CONTROLLER
	VREPController.init(nh,argv[0], argv[1]);

	// IN & OUT CONTROLLER
	inOutController IOController(&VREPController);
	IOController.init(nh);

	ros::Duration(3).sleep();

	cout << "Pause envoyée" << endl;
	VREPController.pause();

	thread spinnerThread(spinner);
	std_msgs::Byte msg_shutdown;
	///////////////////////

	// On attend l'initialisation du reste du projet
	cout << "Attente fin de l'initialisation ..." << endl;
	ros::Rate loop_rate(25);
	while(ros::ok() && initEnCours)
	{
		ros::spinOnce();
		loop_rate.sleep();
	}

	int choix=0;
	int choixProduit=0;
	int choixPoste=0;
	while(ros::ok())
	{
		cout << endl << endl << endl;
		cout << "Que voulez faire ?" 	<< endl <<
			"	1- Ajouter un produit" << endl <<
			"	2- Pause simu" 	<< endl <<
			"	3- Play simu" 	<< endl<<
			"	4- Fin programme" 	<< endl;
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
					cout << "Ajout de produit : quel poste ? [1..8]" << endl;
					cin >> choixPoste;
					if(cin.fail() || choixPoste<1 || choixPoste>8)
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
					pubProductAdd.publish(msg0); // log
					VREPController.addProduct(choixProduit,choixPoste);
					srv.request.poste=choixPoste;
					srv.request.produit=choixProduit;
					clientAddProduct.call(srv);
					break;

				case 2:
					cout << "Mise en Pause de la simu" << endl;
					VREPController.pause();
					break;

				case 3:
					cout << "Mise en Play de la simu" << endl;
					VREPController.play();
					break;

				case 4:
					cout << "Fin Programme" << endl;
					pub_shutdown.publish(msg_shutdown);
					ros::Duration(1).sleep();
					break;

				default:
					cout << endl << " [Erreur mauvais choix ..]" << endl;
					break;
			}
		}

		ros::spinOnce();
	}

	VREPController.close();
	return 0;
}
