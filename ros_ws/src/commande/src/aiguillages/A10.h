#ifndef AIG_10
#define AIG_10

#include <ros/ros.h>
#include <std_msgs/Int32.h>
#include <unistd.h>
#include <vrep_common/simRosEnablePublisher.h>
#include <vrep_common/simRosEnableSubscriber.h>
#include <vrep_common/simRosGetInfo.h>
#include <shuttles/msgShuttleChange.h>
#include <map>
#include "Sh.h"
#include "aiguillages/ExchangeSh.h"
#include <commande_locale/Msg_Color.h>
#include <shuttles/srvGetShuttleStatus.h>
#include <vrep_common/simRosEnablePublisher.h>
#include <vrep_common/simRosEnableSubscriber.h>

#include <vector>

using namespace std;


//aiguillage 1->2 et 2->1
class A10
{
private:

	//Subscribe Sensor
	ros::Subscriber VREPsubStopSensor;
	ros::Subscriber VREPsubSwitchSensor;
	ros::Subscriber DemiTour;
	ros::Subscriber subNbShuttle;

	//Pour les handles des Sh (coloration)
	ros::Subscriber getHandle;
	ros::Subscriber VREPsubRailSensor;
	ros::Publisher SendHandleShToA11;

	vector<int> listeHandle; 
	vector<int> listeHandleinit;
	vector<int> listeHandledecolor; 
	ros::Publisher pubProductChange;
	ros::Publisher ShStop;
	ros::Publisher ShStart;
	ros::Publisher AigDev;
	ros::Publisher AigVer;
	ros::Publisher AigGauche;
	ros::Publisher AigDroit;
	ros::Publisher pub_demandeNbrSh;
	ros::ServiceClient clientGetShuttleStatus;
	shuttles::srvGetShuttleStatus ShuttleInfo;

	//time
	ros::ServiceClient client_get_vrep_time;
	vrep_common::simRosGetInfo srv_get_time;

	std::multimap<int,Sh*> ShuttlesMap;

	ros::Publisher pubChangeColor;
	commande_locale::Msg_Color PlatformColor;


	int dernier_handle_recu;
	bool mem_handle1;
	bool mem_handle2;
	int handle_nav2;
	int envoi;

	float Dt;
	bool CP8; //pour la decoloration navette/sortie de produit fini
	bool CP8_mem;

	bool PS_mem;

	bool PS;//capteur 20
	
	bool PSD;///Capteur stop 19 en entrée de l'aiguillage haut
	
	bool Aig_D;//Capteurs aiguillage
	bool Aig_G;

	int num_aiguillage;
	int num_capt_stop;
	int num_capt_sortie;

	int nbShuttle;

	std_msgs::Int32 num_STOP;

	std_msgs::Int32 HandleSh_msg;

	std_msgs::Int32 num_AIG;

	std_msgs::Int32 topic;



public:
	A10(ros::NodeHandle nh);
	void StopSensorCallback(const std_msgs::Int32::ConstPtr& msg);
	void SwitchSensorCallback(const std_msgs::Int32::ConstPtr& msg);
	void getHandleCallback(const std_msgs::Int32::ConstPtr& msg);
	void RailSensorCallback(const std_msgs::Int32::ConstPtr& msg);
	void NbShuttleCallback(const std_msgs::Int32::ConstPtr& msg);


	void Droit();
	void Virage();

	//get des capteurs
	int get_posAig_Droit();
	int get_posAig_Virage();
	int get_PS19();
	bool get_CP8();

	//void Aiguille_Navette();
	void ModeManuel();
	void STOP();	
	void START();

	float get_time();
	void wait_vrep(float dt);

};

#endif
