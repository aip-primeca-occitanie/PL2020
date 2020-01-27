#include "Capteur_Poste.h"
#include <ros/ros.h>
using namespace std;

Capteur_Poste::Capteur_Poste(ros::NodeHandle n)
{
	client_get_vrep_time = n.serviceClient<vrep_common::simRosGetInfo>("/vrep/simRosGetInfo");
	VREPsubStopSensor = n.subscribe("vrep/StopSensor", 10, &Capteur_Poste::StopSensorCallback, this);
	ShStart = n.advertise<std_msgs::Int32>("/commande/DemarrerNavette", 20);
	ShStop = n.advertise<std_msgs::Int32>("/commande/ArretNavette", 1000);

	
	Dt=0.7;
}

void Capteur_Poste::StopSensorCallback(const std_msgs::Int32::ConstPtr& msg)
{
	PS22 = (msg->data & (int32_t)pow(2,21)) > 0;
	PS21 = (msg->data & (int32_t)pow(2,20)) > 0;
	PS2 = (msg->data & (int32_t)pow(2,1)) > 0;
	PS3 = (msg->data & (int32_t)pow(2,2)) > 0;
}

void Capteur_Poste::StartShuttle(int num_poste)
{
	switch(num_poste)
	{
		default:
			cout << "Erreur de numéro de poste" << endl;
		case 1:
			num_STOP.data=21;
			ShStart.publish(num_STOP);
			this->wait_vrep(Dt);
			ShStop.publish(num_STOP);

		break;

		case 2:
			num_STOP.data=22;
			ShStart.publish(num_STOP);
			this->wait_vrep(Dt);
			ShStop.publish(num_STOP);
		break;

		case 3:
			num_STOP.data=2;
			ShStart.publish(num_STOP);
			this->wait_vrep(Dt);
			ShStop.publish(num_STOP);
		break;

		case 4:
			num_STOP.data=3;
			ShStart.publish(num_STOP);
			this->wait_vrep(Dt);
			ShStop.publish(num_STOP);
		break;
	
		}
}

void Capteur_Poste::StopShuttle(int num_poste)
{
	switch(num_poste)
	{
		default:
			cout << "Erreur de numéro de poste" << endl;
		case 1:
			num_STOP.data=21;
			ShStop.publish(num_STOP);
		break;

		case 2:
			num_STOP.data=22;
			ShStop.publish(num_STOP);
		break;

		case 3:
			num_STOP.data=2;
			ShStop.publish(num_STOP);
		break;

		case 4:
			num_STOP.data=3;
			ShStop.publish(num_STOP);
		break;
	}
}


int Capteur_Poste::get_PS22()
{
	return PS22;
}

int Capteur_Poste::get_PS21()
{
	return PS21;
}

int Capteur_Poste::get_PS2()
{
	return PS2;
}

int Capteur_Poste::get_PS3()
{
	return PS3;
}

float Capteur_Poste::get_time()
{
	
	client_get_vrep_time.call(srv_get_time);
	return (float) srv_get_time.response.simulationTime;


}

void Capteur_Poste::wait_vrep(float dt)
{
	
	float t=this->get_time();
	while(this->get_time()-t<dt)
	{
		//cout << "running" << get_time()-t << "< " <<dt<< endl;
		
		ros::spinOnce();
	}

}

