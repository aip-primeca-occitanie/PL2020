#include <ros/ros.h>
#include <ros/package.h>
#include <std_msgs/Int32MultiArray.h>
#include <std_msgs/Byte.h>
#include <std_msgs/Float32.h>
#include "commande_locale/Msg_AddProduct.h"
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
	ROS_INFO("Evacuation TionTion");
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
	ROS_INFO("Add Product tucttuct");
	ROS_INFO("NewProduct: %d",msg.num_produit);
}


void getTimeCallback(const std_msgs::Float32::ConstPtr msg)
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

	if(!monFlux)
	{
		ROS_ERROR("ERREUR : Impossible d'ouvrir le fichier log.");
		exit(-1);
	}

	//ros::Subscriber subTacheFinie;
	ros::Subscriber subProduitEvac;
	ros::Subscriber subNewProduit;
	//ros::Subscriber subErreurTraitement;

	//subTacheFinie = nh.subscribe("", 1, &Tachefinie);
	subNewProduit = nh.subscribe("/commande_locale/AddProduct", 1, &NewProductCallback);
	subProduitEvac = nh.subscribe("/commande/Simulation/produitEvac", 1, &ProduitEvacCallback);
	//subErreurTraitement = nh.subscribe("", 1, &ErreurTraitement);

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
