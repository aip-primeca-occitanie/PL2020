#ifndef AIG_12
#define AIG_12

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
#include <shuttles/srvGetShuttleStatus.h>

using namespace std;

class A12
{
private:

	//Subscribe Sensor
	ros::Subscriber VREPsubSensor;
	ros::Subscriber VREPsubSwitchSensor;

	vector<int> listeHandle; 

	//Publisher
	ros::Publisher ShStop;
	ros::Publisher ShStart;
	ros::Publisher AigDev;
	ros::Publisher AigVer;
	ros::Publisher AigGauche;
	ros::Publisher AigDroit; 


	ros::Publisher SendHandleShToA1;


	std_msgs::Int32 num_STOPg;
	std_msgs::Int32 num_STOPd;

	std_msgs::Int32 num_AIG;

	//time
	ros::ServiceClient client_get_vrep_time;
	vrep_common::simRosGetInfo srv_get_time;

	std_msgs::Int32 HandleSh_msg;

	ros::Subscriber getHandleA12;
	ros::Subscriber getHandleFromPoste2;

	bool PSG; // capteur PSG en entrée aiguillage gauche
	

	bool PSD; // capteur PSD en entrée aiguillage droit


	bool PS;
	bool PSG_mem;
	bool PSD_mem;



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
	A12(ros::NodeHandle nh);


	void SensorCallback(const std_msgs::Int32::ConstPtr& msg);
	void SwitchSensorCallback(const std_msgs::Int32::ConstPtr& msg);
	void getHandleA12Callback(const std_msgs::Int32::ConstPtr& msg);
	void getHandleFromPoste2Callback(const std_msgs::Int32::ConstPtr& msg);



	//get des capteurs
	int get_posAig_Droit();
	int get_posAig_Virage();
	int get_PS24();
	int get_PS23();

	void STOP_DROIT();
	void STOP_VIRAGE();
	void START_DROIT();
	void START_VIRAGE();
	void Virage();
	void Droit();
	//void Aiguille_Navette();

	float get_time();
	void wait_vrep(float dt);

};

#endif
