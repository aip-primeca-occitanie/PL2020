#ifndef AIG_11
#define AIG_11

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


//aiguillage 1->2
class A11
{
private:

	//Subscriber
	ros::Subscriber VREPsubStopSensor;
	ros::Subscriber VREPsubRailSensor;
	ros::Subscriber VREPsubSwitchSensor;
	
	ros::Subscriber getHandleShFromA10;
	
	
	vector<int> listeHandle; 
	//Publisher
	ros::Publisher ShStop;
	ros::Publisher ShStart;
	ros::Publisher AigDev;
	ros::Publisher AigVer;
	ros::Publisher AigGauche;
	ros::Publisher AigDroit;
	ros::Publisher SendHandleShtoPoste1;
	ros::Publisher SendHandleShToA12;

	//time
	ros::ServiceClient client_get_vrep_time;
	vrep_common::simRosGetInfo srv_get_time;


	std_msgs::Int32 HandleSh_msg;

	std_msgs::Int32 num_AIG;
	std_msgs::Int32 num_STOP;

	bool PS_mem;

	bool PS;///Capteur stop 20 en entrée de l'aiguillage

	bool CPD;//Capteur CP droit apres l'aiguillage

	bool CPG;

	bool Aig_D;//Capteurs aiguillage
	bool Aig_G;

	

	int num_aiguillage;
	int num_capt_stop;
	int num_capt_droit;
	int num_capt_gauche;

	bool mem_handle1;
	bool mem_handle2;
	int handle_nav2;

	

	float Dt;





public:
	A11(ros::NodeHandle nh);

	void StopSensorCallback(const std_msgs::Int32::ConstPtr& msg);
	void RailSensorCallback(const std_msgs::Int32::ConstPtr& msg);
	void SwitchSensorCallback(const std_msgs::Int32::ConstPtr& msg);
	void getHandleShFromA10Callback(const std_msgs::Int32::ConstPtr& msg);

	//get des capteurs
	int get_posAig_Droit();
	int get_posAig_Virage();
	int get_PS20();
	int get_CP10();
	int get_CP9();

	void STOP();
	void START();
	void Virage();
	void Droit();

	float get_time();
	void wait_vrep(float dt);

};

#endif
