
#ifndef CAPTEUR_POSTE
#define CAPTEUR_POSTE


#include <ros/ros.h>
#include <string>
#include <std_msgs/Int32.h>
#include <std_msgs/Bool.h>
#include <cstdlib>
#include <stdlib.h>
#include <map>
#include <vector>
#include <fstream>
#include <iostream>
#include <cstring>

#include <vrep_common/simRosEnablePublisher.h>
#include <vrep_common/simRosEnableSubscriber.h>
#include <vrep_common/simRosGetInfo.h>

using namespace std;

class Capteur_Poste
{
private:
	ros:: Subscriber VREPsubStopSensor;
	ros::Publisher ShStart;
	ros::Publisher ShStop;
	ros::Publisher pubPinOff;

	ros::ServiceClient client_get_vrep_time;
	vrep_common::simRosGetInfo srv_get_time;

	float Dt;

	int PS21, PS22, PS2, PS3;

	int num_capt_stop;
	std_msgs::Int32 num_STOP;

	int num_ergot;
	std_msgs::Int32 num_PIN;

public:

	float get_time();
	void wait_vrep(float dt);
	Capteur_Poste(ros::NodeHandle nh);
	void StartShuttle(int);
	void StopShuttle(int);
	void StopSensorCallback(const std_msgs::Int32::ConstPtr& msg);
	int get_PS21();
	int get_PS22();
	int get_PS2();
	int get_PS3();
};

#endif


