#include "commande.h"
#include <ros/ros.h>
#include <iostream>
#include <std_msgs/Int32.h>

using namespace std;

//the following are UBUNTU/LINUX ONLY terminal color codes.
#define RESET   "\033[0m"
#define BLACK   "\033[30m"      /* Black */
#define RED     "\033[31m"      /* Red */
#define GREEN   "\033[32m"      /* Green */
#define YELLOW  "\033[33m"      /* Yellow */
#define BLUE    "\033[34m"      /* Blue */
#define MAGENTA "\033[35m"      /* Magenta */
#define CYAN    "\033[36m"      /* Cyan */
#define WHITE   "\033[37m"      /* White */
#define BOLDBLACK   "\033[1m\033[30m"      /* Bold Black */
#define BOLDRED     "\033[1m\033[31m"      /* Bold Red */
#define BOLDGREEN   "\033[1m\033[32m"      /* Bold Green */
#define BOLDYELLOW  "\033[1m\033[33m"      /* Bold Yellow */
#define BOLDBLUE    "\033[1m\033[34m"      /* Bold Blue */
#define BOLDMAGENTA "\033[1m\033[35m"      /* Bold Magenta */
#define BOLDCYAN    "\033[1m\033[36m"      /* Bold Cyan */
#define BOLDWHITE   "\033[1m\033[37m"      /* Bold White */

Commande::Commande(ros::NodeHandle noeud, std::string executionPath)
{
  	//Pour être informé de l'arrivée d'un produit sur un poste
	sub_nouveau_produit = noeud.subscribe("/commande_locale/AddProduct", 1000, &Commande::NouveauProduitCallback, this);

	// Actionner ergots
	subPinOn = noeud.subscribe("/Poste_Cmde/SortirErgots", 10, &Commande::SortirErgotCallback, this);
	subPinOff = noeud.subscribe("/Poste_Cmde/RentrerErgots", 10, &Commande::RentrerErgotCallback, this);

	//Commande aiguillages
	SubDeverouilleAiguillages = noeud.subscribe("/commande/DeverouilleAiguillage", 1000, &Commande::DeverouilleAiguillagesCallback, this);
	SubVerouilleAiguillages = noeud.subscribe("/commande/VerouilleAiguillage", 1000, &Commande::VerouilleAiguillagesCallback, this);
	SubAiguillagesGauches = noeud.subscribe("/commande/AiguillageGauche", 1000, &Commande::AiguillagesgauchesCallback, this);
	SubAiguillagesDroits = noeud.subscribe("/commande/AiguillageDroite", 1000, &Commande::AiguillagesdroitsCallback, this);

	// Publishers messages actionneurs
	pub_navettes_stops = noeud.advertise<commande_locale::Msg_StopControl>("/commande/Simulation/Actionneurs_stops", 1);
	pub_actionneurs_simu_aiguillages = noeud.advertise<commande_locale::Msg_SwitchControl>("/commande/Simulation/Actionneurs_aiguillages", 1);
  	pub_actionneurs_simu_pins = noeud.advertise<commande_locale::Msg_PinControl>("/commande/Simulation/Actionneurs_pins", 10);

	// Initialisation des Actionneurs
	for(int i=1;i<=24;i++) actionneurs_simulation_Stop.STOP[i] = 0;
	for(int i=1;i<=24;i++) actionneurs_simulation_Stop.GO[i] = 1;

	for(int i=1;i<=12;i++) actionneurs_simulation_Aiguillages.LOCK[i] = 0;
	for(int i=1;i<=12;i++) actionneurs_simulation_Aiguillages.RD[i] = 0;
	for(int i=1;i<=12;i++) actionneurs_simulation_Aiguillages.RG[i] = 0;

  	for(int i=1;i<=8;i++) actionneurs_simulation_Pin.PINON[i] = 0;
  	for(int i=1;i<=8;i++) actionneurs_simulation_Pin.PINOFF[i] = 1;
}

void Commande::Initialisation()
{
	for(int i=0;i<25;i++) PSx[i]=0;
	for(int i=0;i<13;i++) DxD[i]=0;
	for(int i=0;i<13;i++) DxG[i]=0;
	for(int i=0;i<11;i++) CPx[i]=0;
	for(int i=0;i<9;i++) CPIx[i]=0;

	for(int i=0;i<25;i++) STx[i]=1;
	for(int i=0;i<13;i++) RxD[i]=0;
	for(int i=0;i<13;i++) RxG[i]=0;

	for(int i=0;i<13;i++) Vx[i]=0;
	for(int i=0;i<13;i++) Dx[i]=0;
	for(int i=0;i<9;i++) PIx[i]=0;
}

void Commande::NouveauProduitCallback(const commande_locale::Msg_AddProduct::ConstPtr& msg)
{
	ROS_INFO("Je recois les messages trkl");
	poste=msg->num_poste;
	produit=msg->num_produit;
	arrivee_produit=1;
}

int Commande::get_arrivee_nouveau_produit()
{
	return arrivee_produit;
}

int Commande::get_code_arrivee()
{
	return produit+poste;
}

void Commande::renitialiser_arrivee_nouveau_produit()
{
  arrivee_produit=0;
}

void Commande::Stop_PS(int point_stop)
{
	actionneurs_simulation_Stop.STOP[point_stop]=1;
	actionneurs_simulation_Stop.GO[point_stop]=0;
	pub_navettes_stops.publish(actionneurs_simulation_Stop);
}

void Commande::Ouvrir_PS(int point_stop)
{
	actionneurs_simulation_Stop.STOP[point_stop]=0;
	actionneurs_simulation_Stop.GO[point_stop]=1;
	pub_navettes_stops.publish(actionneurs_simulation_Stop);
}

void Commande::DeverouilleAiguillagesCallback(const std_msgs::Int32::ConstPtr& msg)
{
	Dx[msg->data]=1;
	Vx[msg->data]=0;
}

void Commande::VerouilleAiguillagesCallback(const std_msgs::Int32::ConstPtr& msg)
{
	Dx[msg->data]=0;
	Vx[msg->data]=1;
}

void Commande::AiguillagesgauchesCallback(const std_msgs::Int32::ConstPtr& msg)
{
	RxG[msg->data]=1;
	for(int i=0;i<13;i++) actionneurs_simulation_Aiguillages.RG[i] = RxG[i];
	for(int i=0;i<13;i++) actionneurs_simulation_Aiguillages.RD[i] = RxD[i];
	for(int i=0;i<13;i++) actionneurs_simulation_Aiguillages.LOCK[i] = !Vx[i] && Dx[i];
	pub_actionneurs_simu_aiguillages.publish(actionneurs_simulation_Aiguillages);
	RxG[msg->data]=0;
}

void Commande::AiguillagesdroitsCallback(const std_msgs::Int32::ConstPtr& msg)
{
	RxD[msg->data]=1;
	for(int i=0;i<13;i++) actionneurs_simulation_Aiguillages.RD[i] = RxD[i];
	for(int i=0;i<13;i++) actionneurs_simulation_Aiguillages.RG[i] = RxG[i];
	for(int i=0;i<13;i++) actionneurs_simulation_Aiguillages.LOCK[i] = !Vx[i] && Dx[i];
	pub_actionneurs_simu_aiguillages.publish(actionneurs_simulation_Aiguillages);
	RxD[msg->data]=0;
}

void Commande::SortirErgotCallback(const std_msgs::Int32::ConstPtr& msg)
{
	PIx[msg->data]=1;
  for(int i=1;i<=8;i++) actionneurs_simulation_Pin.PINON[i] = PIx[i];
  for(int i=1;i<=8;i++) actionneurs_simulation_Pin.PINOFF[i] = !PIx[i];
	pub_actionneurs_simu_pins.publish(actionneurs_simulation_Pin);
}

void Commande::RentrerErgotCallback(const std_msgs::Int32::ConstPtr& msg)
{
	PIx[msg->data]=0;
  for(int i=1;i<=8;i++) actionneurs_simulation_Pin.PINON[i] = PIx[i];
  for(int i=1;i<=8;i++) actionneurs_simulation_Pin.PINOFF[i] = !PIx[i];
	pub_actionneurs_simu_pins.publish(actionneurs_simulation_Pin);
}
