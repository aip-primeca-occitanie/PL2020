#ifndef AIGUILLAGE
#define AIGUILLAGE

#include <ros/ros.h>
#include <std_msgs/Int32.h>
#include <unistd.h>
#include <aiguillages/Msg_Aiguillage.h>

using namespace std;

class Aiguillage
{
private:

	ros::Subscriber VREPsubSwitchSensor;//Capteurs butée aiguillage
	ros::Subscriber sub_cmd_Droite;
	ros::Subscriber sub_cmd_Gauche;

	ros::Rate* loop_rate;

	ros::Publisher AigDev;
	ros::Publisher AigVer;
	ros::Publisher AigGauche;
	ros::Publisher AigDroite;

	std_msgs::Int32 num_AIG;

	bool Aig_D;//Capteurs butée
	bool Aig_G;

	int num_aiguillage;

public:
	Aiguillage(ros::NodeHandle nh, int id_aiguillage);
	~Aiguillage();

	void SwitchSensorCallback(const std_msgs::Int32::ConstPtr& msg);
	void GaucheCallback(const aiguillages::Msg_Aiguillage::ConstPtr& msg_aigs);
	void DroiteCallback(const aiguillages::Msg_Aiguillage::ConstPtr& msg_aigs);
};
#endif
