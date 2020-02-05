/*
 * ************************************* *
 * 	  Projet Long N7 2017  	         *
 * ************************************* *
*/
#ifndef COMMANDE_SIMU
#define COMMANDE_SIMU

#include "vrep_common/simRosEnablePublisher.h"
#include "vrep_common/simRosEnableSubscriber.h"
#include <ros/ros.h>
#include "actionneurs.h"
#include "capteurs.h"
#include "robots.h"
#include <std_msgs/Bool.h>
#include <std_msgs/Int32.h>
#include <commande_locale/Msg_StopControl.h>
#include "shuttles/msgShuttleCreate.h"
#include <fstream>
#include <iostream>
#include <cstring>
#include <string>
#include <cstdlib>
#include <stdlib.h>
#include <map>

class Commande
{
	private:

	//Subscriber
	// Aiguillages
	ros::Subscriber SubDeverouilleAiguillages;
	ros::Subscriber SubVerouilleAiguillages;
	ros::Subscriber SubAiguillagesGauches;
	ros::Subscriber SubAiguillagesDroits;

	// Ergots
	ros::Subscriber subPinOn;
	ros::Subscriber subPinOff;

	ros::Subscriber NewShuttle;
	ros::Subscriber SubNouvelleNavette;

//Publisher
	// Actionneurs
	ros::Publisher pub_navettes_stops;
	ros::Publisher pub_actionneurs_simu_aiguillages;
  ros::Publisher pub_actionneurs_simu_pins;

//Variables
  int produitNavette;
	int arrivee_navette;
	/* Capteurs */
	bool PSx[25],DxD[13],DxG[13],CPx[11],CPIx[9];
	/* Actionneurs */
	bool STx[25],RxD[13],RxG[13],Vx[13],Dx[13],PIx[9];
	//messages pour actionneurs
	commande_locale::Msg_StopControl actionneurs_simulation_Stop;
	commande_locale::Msg_SwitchControl actionneurs_simulation_Aiguillages;
	commande_locale::Msg_PinControl actionneurs_simulation_Pin;

public:

	Commande(ros::NodeHandle noeud, std::string executionPath);

	void Initialisation();

  void NouvelleNavette(const shuttles::msgShuttleCreate::ConstPtr& msg);
	void Stop_PS(int point_stop);
	void Ouvrir_PS(int point_stop);

	void DeverouilleAiguillagesCallback(const std_msgs::Int32::ConstPtr& msg);
	void VerouilleAiguillagesCallback(const std_msgs::Int32::ConstPtr& msg);
	void AiguillagesgauchesCallback(const std_msgs::Int32::ConstPtr& msg);
	void AiguillagesdroitsCallback(const std_msgs::Int32::ConstPtr& msg);

	void SortirErgotCallback(const std_msgs::Int32::ConstPtr& msg);
	void RentrerErgotCallback(const std_msgs::Int32::ConstPtr& msg);


	int get_arrivee_navette();
	int get_produit_navette();
	void ReinitialiserArriveeNavette();
};

#endif
