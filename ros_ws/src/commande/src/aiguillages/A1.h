#ifndef AIG_1
#define AIG_1

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
class A1
{
private:

	//Subscriber
	ros::Subscriber VREPsubStopSensor;
	ros::Subscriber VREPsubRailSensor;
	ros::Subscriber VREPsubSwitchSensor;

	ros::Publisher SendHandleShToA2;
	ros::Publisher SendHandleShToPoste3;

	vector<int> listeHandle; 

	ros::Subscriber getHandle;
	std_msgs::Int32 HandleSh_msg;

	//time
	ros::ServiceClient client_get_vrep_time;
	vrep_common::simRosGetInfo srv_get_time;


	//Publisher
	ros::Publisher ShStop;
	ros::Publisher ShStart;
	ros::Publisher AigDev;
	ros::Publisher AigVer;
	ros::Publisher AigGauche;
	ros::Publisher AigDroit;


	std::multimap<int,Sh*> ShuttlesMap;
	std_msgs::Int32 num_AIG;
	std_msgs::Int32 num_STOP;

	float Dt;

	bool PS_mem;

	bool PS;///Capteur stop 01 en entrée de l'aiguillage

	bool CPD;//Capteur CP droit apres l'aiguillage
	bool CPD_past;

	bool CPG;
	bool CPG_past;

	bool Nav_CPD;///Une navette est passé en CP9
	bool Nav_CPG;

	bool Aig_D;//Capteurs aiguillage
	bool Aig_G;

	bool mem_handle1;
	bool mem_handle2;
	int handle_nav2;


	int num_aiguillage;
	int num_capt_stop;
	int num_capt_droit;
	int num_capt_gauche;



public:
	A1(ros::NodeHandle nh);
	void StopSensorCallback(const std_msgs::Int32::ConstPtr& msg);
	void RailSensorCallback(const std_msgs::Int32::ConstPtr& msg);
	void SwitchSensorCallback(const std_msgs::Int32::ConstPtr& msg);

	void getHandleCallback(const std_msgs::Int32::ConstPtr& msg);

	//void NewShuttleCallBack(const aiguillages::ExchangeSh::ConstPtr& msg);



	//get des capteurs
	int get_posAig_Droit();
	int get_posAig_Virage();
	int get_PS1();
	int get_CP1();
	int get_CP2();


	void STOP();
	void START();
	void Virage();
	void Droit();
	void Aiguille_Navette();

	//int get_Sh_Handle();
	//void Send_Sh(int destination);

	float get_time();
	void wait_vrep(float dt);

};

#endif
