#ifndef AIG_2
#define AIG_2

#include <ros/ros.h>
#include <std_msgs/Int32.h>
#include <unistd.h>
#include <vrep_common/simRosEnablePublisher.h>
#include <vrep_common/simRosEnableSubscriber.h>
#include <vrep_common/simRosGetInfo.h>
#include <map>
#include "Sh.h" 
#include "aiguillages/ExchangeSh.h"

#include <vector>

using namespace std;

class A2
{
private:

	//Subscriber
	ros::Subscriber VREPsubSensor;
	ros::Subscriber VREPsubSwitchSensor;


	ros::Subscriber getHandle;
	std_msgs::Int32 HandleSh_msg;

	ros::Publisher SendHandleShToA3;

	vector<int> listeHandle; 


	//Publisher
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

	std_msgs::Int32 num_STOPg;
	std_msgs::Int32 num_STOPd;
	std_msgs::Int32 num_AIG;



	bool PSG; // capteur PSG en entrée aiguillage gauche
	bool PSG_mem;
	bool PSD_mem;

	bool PSD; // capteur PSD en entrée aiguillage droit
	

	bool PS;

	bool Aig_D;
	bool Aig_G;

	bool Att_D;
	bool Att_G;

	bool mem_handle1;
	bool mem_handle2;
	int handle_nav2;

	int num_aiguillage ;
	int num_stop_droit;
	int num_stop_gauche;
	int num_capt_sortie;

	

	float Dt;




 
public:
	A2(ros::NodeHandle nh);

	void SensorCallback(const std_msgs::Int32::ConstPtr& msg);

	void getHandleCallback(const std_msgs::Int32::ConstPtr& msg);

	void SwitchSensorCallback(const std_msgs::Int32::ConstPtr& msg);

	//get des capteurs
	int get_posAig_Droit();
	int get_posAig_Virage();
	int get_PS4();
	int get_PS5();

	void STOP_DROIT();
	void STOP_VIRAGE();
	void START_DROIT();
	void START_VIRAGE();
	void Virage();
	void Droit();

	float get_time();
	void wait_vrep(float dt);

};

#endif
