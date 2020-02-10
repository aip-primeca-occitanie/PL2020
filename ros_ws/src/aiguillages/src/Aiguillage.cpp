#include "Aiguillage.h"

using namespace std;

Aiguillage::Aiguillage(ros::NodeHandle nh, int id_aiguillage)
{
	std::string num_str;
	switch(id_aiguillage)
	{
			case 1:
						num_str="1";
						break;
			case 2:
						num_str="2";
						break;
			case 3:
						num_str="3";
						break;
			case 4:
						num_str="4";
						break;
			case 5:
						num_str="5";
						break;
			case 6:
						num_str="6";
						break;
			case 7:
						num_str="7";
						break;
			case 8:
						num_str="8";
						break;
			case 9:
						num_str="9";
						break;
			case 10:
						num_str="10";
						break;
			case 11:
						num_str="11";
						break;
			case 12:
						num_str="12";
						break;
		default:
	ROS_INFO("CHOIX AIGUILLAGE INCORRECT");
	break;
	}

	num_aiguillage=id_aiguillage;

	loop_rate=new ros::Rate(25);

	VREPsubSwitchSensor = nh.subscribe("vrep/SwitchSensor", 1000, &Aiguillage::SwitchSensorCallback, this);//Info sur position aiguillage
	sub_cmd_Droite = nh.subscribe("/commande/Simulation/AiguillageDroite"+num_str,1000,&Aiguillage::DroiteCallback,this);
	sub_cmd_Gauche = nh.subscribe("/commande/Simulation/AiguillageGauche"+num_str,1000,&Aiguillage::GaucheCallback,this);

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

void Aiguillage::GaucheCallback(const aiguillages::Msg_Aiguillage::ConstPtr& msg_aigs)
{
	if (num_AIG.data==msg_aigs->Aiguillage)
	{
		ROS_INFO("On bouge a gauche");
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


void Aiguillage::DroiteCallback(const aiguillages::Msg_Aiguillage::ConstPtr& msg_aigs)
{
	if (num_AIG.data==msg_aigs->Aiguillage)
	{
		ROS_INFO("On bouge a droite");
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
