#ifndef AIG_A3
#define AIG_A3

#include <ros/ros.h>
#include <std_msgs/Int32.h>
#include <unistd.h>
#include <vrep_common/simRosEnablePublisher.h>
#include <vrep_common/simRosEnableSubscriber.h>
#include <vrep_common/simRosGetInfo.h>
#include <map>
#include "Sh.h" 
#include "aiguillages/ExchangeSh.h"

#include <shuttles/srvGetShuttleStatus.h>

using namespace std;


#include <vector>

//aiguillage 1->2 et 2->1
class A3
{
private:

	//Subscribe Sensor
	ros::Subscriber VREPsubStopSensor;
	ros::Subscriber VREPsubRailSensor;
	ros::Subscriber VREPsubSwitchSensor;

	ros::Subscriber getHandle;
	std_msgs::Int32 HandleSh_msg;
	ros::Publisher SendHandleShtoA11;
	ros::Publisher SendHandleShtoA10;

	vector<int> listeHandle; 

	ros::Publisher ShStop;
	ros::Publisher ShStart;
	ros::Publisher AigDev;
	ros::Publisher AigVer;
	ros::Publisher AigGauche;
	ros::Publisher AigDroit;

	//time
	ros::ServiceClient client_get_vrep_time;
	vrep_common::simRosGetInfo srv_get_time;

	std::multimap<int,Sh*> ShuttlesMap;


	bool PS;///Capteur stop 6 en entrée de l'aiguillage bas
	bool PS_mem;
	float Dt;

	bool Aig_D;//Capteurs aiguillage
	bool Aig_G;

	bool CPb;

	bool mem_handle1;
	bool mem_handle2;
	int handle_nav2;

	int num_aiguillage;
	int num_capt_stop;
	int num_capt_sortie_haut;
	int num_capt_sortie_bas;

	std_msgs::Int32 num_STOP;

	std_msgs::Int32 num_AIG;



public:
	A3(ros::NodeHandle nh);
	
	void StopSensorCallback(const std_msgs::Int32::ConstPtr& msg);
	void RailSensorCallback(const std_msgs::Int32::ConstPtr& msg);
	void SwitchSensorCallback(const std_msgs::Int32::ConstPtr& msg);

	void getHandleCallback(const std_msgs::Int32::ConstPtr& msg);
	
	void Droit();
	void Virage();

	//get des capteurs
	int get_posAig_Droit();
	int get_posAig_Virage();
	int get_PS6();
	int get_CP3();

	void ModeManuel();
	void STOP();
	void START();
	int get_Sh_Handle();

	float get_time();
	void wait_vrep(float dt);

};

#endif
