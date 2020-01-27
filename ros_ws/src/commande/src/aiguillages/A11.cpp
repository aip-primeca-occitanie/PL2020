#include "A11.h"  

using namespace std;

//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////Constructeur////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
A11::A11(ros::NodeHandle nh)
{
	cout<<"Initialisation"<<endl;

	client_get_vrep_time = nh.serviceClient<vrep_common::simRosGetInfo>("/vrep/simRosGetInfo");

	VREPsubStopSensor = nh.subscribe("vrep/StopSensor", 1000, &A11::StopSensorCallback, this);
	VREPsubRailSensor = nh.subscribe("vrep/RailSensor", 1000, &A11::RailSensorCallback, this);
	VREPsubSwitchSensor = nh.subscribe("vrep/SwitchSensor", 1000, &A11::SwitchSensorCallback, this);

	Dt=0.7;

	ShStop = nh.advertise<std_msgs::Int32>("/commande/ArretNavette", 1000);
	ShStart = nh.advertise<std_msgs::Int32>("/commande/DemarrerNavette", 1000);
	AigDev = nh.advertise<std_msgs::Int32>("/commande/DeverouilleAiguillage", 1000);
	AigVer = nh.advertise<std_msgs::Int32>("/commande/VerouilleAiguillage", 1000);
	AigGauche = nh.advertise<std_msgs::Int32>("/commande/AiguillageGauche", 1000);
	AigDroit = nh.advertise<std_msgs::Int32>("/commande/AiguillageDroit", 1000);

	getHandleShFromA10 = nh.subscribe("/HandleExchange/SendHandleShToA11", 1000, &A11::getHandleShFromA10Callback, this);
	SendHandleShtoPoste1= nh.advertise<std_msgs::Int32>("/HandleExchange/SendHandleShtoPoste1", 1000);
	SendHandleShToA12 = nh.advertise<std_msgs::Int32>("/HandleExchange/SendHandleShToA12", 1000);
	
	//cout<<"time : "<<this->get_time()<<endl;

	PS_mem=0;

	PS=0;


	//Numéro des capteurs et de l'aiguillage
	num_aiguillage=11;//aiguillage
	num_capt_stop=20;//capteur et stop en entrée de l'aiguillage
	num_capt_droit=9;//capteur en sorti de l'aiguillage droit
	num_capt_gauche=10;//capteur en sorti de l'aiguillage gauche

	Aig_D=1;//Aiguillage à droite au démarrage de la navette


	

	num_AIG.data=num_aiguillage;
	num_STOP.data=num_capt_stop;

	usleep(1000000);
	this->STOP();



}


//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////Callback////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////

//StopSensor
void A11::StopSensorCallback(const std_msgs::Int32::ConstPtr& msg)
{
	PS = (msg->data & (int32_t)pow(2,num_capt_stop-1)) > 0;
	if (PS==0 && PS_mem==1)
	{
		if (listeHandle.size()!=0)
		{
			HandleSh_msg.data=listeHandle[0];
	  		if (Aig_D==1)
			{
				cout << "envoie handle de A11 à poste 1 " << HandleSh_msg.data << endl;
				SendHandleShtoPoste1.publish(HandleSh_msg);
				listeHandle.erase(listeHandle.begin());
			}
		
			if (Aig_G==1)
			{
				cout << "envoie handle de A11 à A12 " << HandleSh_msg.data << endl;
				SendHandleShToA12.publish(HandleSh_msg);
				listeHandle.erase(listeHandle.begin());
			}
		}

	}
	PS_mem=PS;
}

//RailSensor
void A11::RailSensorCallback(const std_msgs::Int32::ConstPtr& msg)
{
	CPD = (msg->data & (int32_t)pow(2,num_capt_droit-1)) > 0;
	CPG = (msg->data & (int32_t)pow(2,num_capt_gauche-1)) > 0;

}


//SwitchSensor
void A11::SwitchSensorCallback(const std_msgs::Int32::ConstPtr& msg)
{
	Aig_D = (msg->data & (int32_t)pow(2,2*num_aiguillage-2)) > 0;
	Aig_G = (msg->data & (int32_t)pow(2,2*num_aiguillage-1)) > 0;
}


void A11::getHandleShFromA10Callback(const std_msgs::Int32::ConstPtr& msg)
{
	listeHandle.push_back(msg->data);
}


//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////Commande Aiguillage/////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
void A11::STOP()
{
	ShStop.publish(num_STOP);
}

void A11::START()
{
	ShStart.publish(num_STOP);
}




void A11::Droit()
{
	
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
			usleep(100000);
			if(Aig_D) AigGauche.publish(num_AIG);
		}


		//Verouillage de l'aiguillage
		AigVer.publish(num_AIG);
	}

	usleep(100000);	

	
}



void A11::Virage()
{
	
	if(!Aig_D)
	{
		
		//Deverouillage de l'aiguillage	
		AigDev.publish(num_AIG);

		usleep(100000);

		//Envoie ordre mouvement à droite
		AigDroit.publish(num_AIG);


		//Attente...
		while(!Aig_D)
		{
			ros::spinOnce();
			usleep(100000);
			if(Aig_G) AigDroit.publish(num_AIG);
		}

		//Verouillage de l'aiguillage
		AigVer.publish(num_AIG);
	}

	usleep(100000);

}


float A11::get_time()
{
	client_get_vrep_time.call(srv_get_time);
	return (float) srv_get_time.response.simulationTime;

}

void A11::wait_vrep(float dt)
{
	float t=this->get_time();
	while(this->get_time()-t<dt)
	{
		ros::spinOnce();
	}

}

int A11::get_CP10()
{
	return CPG;
}

int A11::get_CP9()
{
	return CPD;
}

int A11::get_PS20()  //<---MODIF
{
	return PS;
}

int A11::get_posAig_Droit()
{
	return Aig_G;
}


int A11::get_posAig_Virage()
{
	return Aig_D;
}
