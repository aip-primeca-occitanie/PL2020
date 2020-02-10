#ifndef AIGUILLAGE
#define AIGUILLAGE

#include <ros/ros.h>
#include <std_msgs/Int32.h>
#include <unistd.h>
#include <map>
#include "aiguillages/Msg_Aiguillage.h"

using namespace std;


//aiguillage 1->2
class Aiguillage
{
private:

	//Subscriber

	ros::Subscriber VREPsubSwitchSensor;

	ros::Subscriber sub_cmd_Droite;
	ros::Subscriber sub_cmd_Gauche;


	ros::Rate* loop_rate;


	//Publisher

	ros::Publisher AigDev;
	ros::Publisher AigVer;
	ros::Publisher AigGauche;
	ros::Publisher AigDroite;


	//std::multimap<int,Sh*> ShuttlesMap;
	std_msgs::Int32 num_AIG;


	bool Aig_D;//Capteurs aiguillage
	bool Aig_V;



	int num_aiguillage;


public:
	Aiguillage(ros::NodeHandle nh, int id_aiguillage);
	~Aiguillage();

	void SwitchSensorCallback(const std_msgs::Int32::ConstPtr& msg);

	void GaucheCallback(const aiguillages::Msg_Aiguillage::ConstPtr& msg_aigs);
	void DroiteCallback(const aiguillages::Msg_Aiguillage::ConstPtr& msg_aigs);
	
};

#endif
