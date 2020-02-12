#include "Aiguillage.h"

using namespace std;

Aiguillage::Aiguillage(ros::NodeHandle nh, int id_aiguillage)
{
	num_aiguillage=id_aiguillage;

	loop_rate=new ros::Rate(25);

	VREPsubSwitchSensor = nh.subscribe("vrep/SwitchSensor", 1000, &Aiguillage::SwitchSensorCallback, this);//Info sur position aiguillage
	sub_cmd_Droite = nh.subscribe("/commande/Simulation/AiguillageDroite",1000,&Aiguillage::DroiteCallback,this);
	sub_cmd_Gauche = nh.subscribe("/commande/Simulation/AiguillageGauche",1000,&Aiguillage::GaucheCallback,this);

	AigDev = nh.advertise<std_msgs::Int32>("/commande/DeverouilleAiguillage", 1000);
	AigVer = nh.advertise<std_msgs::Int32>("/commande/VerouilleAiguillage", 1000);
	AigGauche = nh.advertise<std_msgs::Int32>("/commande/AiguillageGauche", 1000);
	AigDroite = nh.advertise<std_msgs::Int32>("/commande/AiguillageDroite", 1000);

	num_AIG.data=num_aiguillage;

	usleep(1000000);
}

Aiguillage::~Aiguillage()
{
	delete loop_rate;
}

void Aiguillage::SwitchSensorCallback(const std_msgs::Int32::ConstPtr& msg)
{
	Aig_D = (msg->data & (int32_t)pow(2,2*num_aiguillage-2)) > 0;
	Aig_G = (msg->data & (int32_t)pow(2,2*num_aiguillage-1)) > 0;
}

void Aiguillage::GaucheCallback(const std_msgs::Int32::ConstPtr& msg_aigs)
{
	if (num_aiguillage==msg_aigs->data)
	{
		ROS_INFO("On bouge a gauche, aig numero %d", num_aiguillage);
		if(!Aig_G)
		{
			//Deverouillage de l'aiguillage
			AigDev.publish(num_AIG);
			usleep(100000);
				//Envoie ordre mouvement à gauche
			AigGauche.publish(num_AIG);

			//Attente...
			while(!Aig_G)
			{
				ros::spinOnce();
				if(Aig_D) AigGauche.publish(num_AIG);
				loop_rate->sleep();
			}

			//Verouillage de l'aiguillage
			AigVer.publish(num_AIG);

		}
		usleep(100000);
	}
}


void Aiguillage::DroiteCallback(const std_msgs::Int32::ConstPtr& msg_aigs)
{
	if (num_aiguillage==msg_aigs->data)
	{
		ROS_INFO("On bouge a droite, aig numero %d", num_aiguillage);
		if(!Aig_D)
		{
			//Deverouillage de l'aiguillage
			AigDev.publish(num_AIG);
			usleep(100000);
			//Envoie ordre mouvement à droite
			AigDroite.publish(num_AIG);

			//Attente...
			while(!Aig_D)
			{
				ros::spinOnce();
				if(Aig_G) AigDroite.publish(num_AIG);
				loop_rate->sleep();
			}

			//Verouillage de l'aiguillage
			AigVer.publish(num_AIG);
		}
		usleep(100000);
	}
}
