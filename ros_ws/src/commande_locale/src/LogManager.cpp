#include <ros/ros.h>
#include <ros/package.h>
#include <std_msgs/Int32.h>
#include <std_msgs/Int32MultiArray.h>
#include <std_msgs/Byte.h>
#include <std_msgs/Float32.h>
#include "commande_locale/Msg_AddProduct.h"
#include "robots/TacheFinieMsg.h"
#include <iostream>
#include <fstream>

using namespace std;

ros::Publisher pubSim_getTime;
ros::Subscriber subSim_getTime;
bool repSim_getTime;
float valueSim_getTime;

string path=ros::package::getPath("commande_locale");
string filepath=path.substr(0,path.length()-(15+11))+"log.txt"; // 15="commande_locale.length 11="ros_ws/src/".length

ofstream monFlux(filepath, ios::app);  //On essaye d'ouvrir le fichier

void ProduitEvacCallback(std_msgs::Int32MultiArray msg)
{
	ros::Rate loop_rate(25);
	pubSim_getTime.publish(std_msgs::Byte());
	while(!repSim_getTime && ros::ok())
	{
		ros::spinOnce();
		loop_rate.sleep();
	}
	repSim_getTime=false;
	float temps=valueSim_getTime;

	int couleur[4];
	monFlux<<"Sortie";
	for (int i=0;i<4;i++)
	{
		couleur[i]=msg.data[i];
		monFlux<<" :" <<couleur[i];
	}
	monFlux<<fixed<<setprecision(2)<<" :"<<temps;
	monFlux<<endl;

	//a supprimer plus tard
	ROS_INFO("Evacuation sion sion");
	for (int i=0;i<4;i++)
	{
		couleur[i]=msg.data[i];
		ROS_INFO("Etage %d : %d",i,couleur[i]);
	}
}
void NewProductCallback(commande_locale::Msg_AddProduct msg)
{
	ros::Rate loop_rate(25);
	pubSim_getTime.publish(std_msgs::Byte());
	while(!repSim_getTime && ros::ok())
	{
		ros::spinOnce();
		loop_rate.sleep();
	}
	repSim_getTime=false;
	float temps=valueSim_getTime;

	monFlux<<"NewProduct: ";
	monFlux<<msg.num_produit;
	monFlux<<fixed<<setprecision(2)<<" :"<<temps;
	monFlux<<endl;

	//a supprimer plus tard
	ROS_INFO("Add Product tuct tuct");
	ROS_INFO("NewProduct: %d",msg.num_produit);
}

void ErreurCallback(const std_msgs::Int32::ConstPtr& msg)
{
	if (msg->data!=66)//si ce n'est pas le code erreur ecrasement
	{
		monFlux<<"OperationPosteVide: ";
		monFlux<<msg->data;
		monFlux<<endl;

		//a supprimer plus tard
		ROS_INFO("ERREUR poste Vide ide ide");
		ROS_INFO("sur le poste: %d",msg->data);
	}
	else
	{
		monFlux<<"EcrasementProduit";
		monFlux<<endl;
		//a supprimer plus tard
		ROS_INFO("ERREUR On a ecrase un produit : c'est mal");
	}
}

void TachefinieCallback(const robots::TacheFinieMsg::ConstPtr& msg)
{
	monFlux<<"TempoT";
	monFlux<<": "<<msg->num_produit;
	monFlux<<": "<<msg->num_poste;
	monFlux<<": "<<msg->duree;
	monFlux<<endl;

	//a supprimer plus tard
	ROS_INFO("Tache finie nie nie");
	ROS_INFO("sur le poste: %d, le produit: %d, pendant la duree : %d",msg->num_poste, msg->num_produit, msg->duree);
}

void getTimeCallback(const std_msgs::Float32::ConstPtr& msg)
{
	valueSim_getTime=msg->data;
	repSim_getTime=true;
}

int main(int argc, char **argv)
{
	ofstream monFlux2(filepath);
	monFlux2.close();

	ros::init(argc, argv, "log_manager");
	ros::NodeHandle nh;

	ros::Subscriber subTacheFinie;
	ros::Subscriber subProduitEvac;
	ros::Subscriber subNewProduit;
	ros::Subscriber subErreur;

	subTacheFinie = nh.subscribe("/commande/Simulation/TacheFinie", 1, &TachefinieCallback);
	subNewProduit = nh.subscribe("/commande_locale/AddProduct", 1, &NewProductCallback);
	subProduitEvac = nh.subscribe("/commande/Simulation/produitEvac", 1, &ProduitEvacCallback);
	subErreur = nh.subscribe("/commande/Simulation/Erreur_log", 1, &ErreurCallback);

	// GetTime VREP
	pubSim_getTime=nh.advertise<std_msgs::Byte>("/sim_ros_interface/services/LogManager/GetTime",100);
	subSim_getTime=nh.subscribe("/sim_ros_interface/services/response/LogManager/GetTime",100,getTimeCallback);
	repSim_getTime=false;

	sleep(2);
	ros::Rate loop_rate(25); //fréquence de la boucle

	while (ros::ok())
	{
		ros::spinOnce(); //permet aux fonction callback de ros dans les objets d'êtres appelées
		loop_rate.sleep(); //permet de synchroniser la boucle while. Il attend le temps qu'il reste pour faire le 25Hz (ou la fréquence indiquée dans le loop_rate)
	}

	return 0;
}
