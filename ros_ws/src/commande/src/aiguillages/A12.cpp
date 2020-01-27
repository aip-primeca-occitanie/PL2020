#include "A12.h"

using namespace std;

A12::A12(ros::NodeHandle nh)
{
	cout<<"Initialisation"<<endl;


	client_get_vrep_time = nh.serviceClient<vrep_common::simRosGetInfo>("/vrep/simRosGetInfo");

	VREPsubSensor = nh.subscribe("vrep/StopSensor", 1000, &A12::SensorCallback, this);
	VREPsubSwitchSensor = nh.subscribe("vrep/SwitchSensor", 1000, &A12::SwitchSensorCallback, this);
	
	getHandleA12 = nh.subscribe("/HandleExchange/SendHandleShToA12", 1000, &A12::getHandleA12Callback, this);
	getHandleFromPoste2 = nh.subscribe("/HandleExchange/SendHandleShToA12bis", 1000, &A12::getHandleFromPoste2Callback, this);

	ShStop = nh.advertise<std_msgs::Int32>("/commande/ArretNavette", 1000);
	ShStart = nh.advertise<std_msgs::Int32>("/commande/DemarrerNavette", 1000);
	AigDev = nh.advertise<std_msgs::Int32>("/commande/DeverouilleAiguillage", 1000);
	AigVer = nh.advertise<std_msgs::Int32>("/commande/VerouilleAiguillage", 1000);
	AigGauche = nh.advertise<std_msgs::Int32>("/commande/AiguillageGauche", 1000);
	AigDroit = nh.advertise<std_msgs::Int32>("/commande/AiguillageDroit", 1000);

	SendHandleShToA1 = nh.advertise<std_msgs::Int32>("/HandleExchange/SendHandleShToA1", 1000);

	cout<<"time : "<<this->get_time()<<endl;


	
	Dt=0.7;


	PSG=0;
	PSD=0;

	PSG_mem=0;
	PSD_mem=0;

	mem_handle1=0;
	mem_handle2=0;
	

	num_aiguillage = 12;
	num_stop_droit = 24;
	num_stop_gauche = 23;
	num_capt_sortie = 1;

	//HandleSh_msg.data=0;
	num_STOPg.data=num_stop_gauche;
	num_STOPd.data=num_stop_droit;

	num_AIG.data=num_aiguillage;

	
	usleep(1000000);
	this->STOP_DROIT();
	this->STOP_VIRAGE();

	
}
 



//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////Callback////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////

void A12::SensorCallback(const std_msgs::Int32::ConstPtr& msg)
{

	PSG= (msg->data & (int32_t)pow(2,num_stop_gauche -1)) > 0;
	PSD= (msg->data & (int32_t)pow(2,num_stop_droit -1)) > 0;
	PS = (msg->data & (int32_t)pow(2,num_capt_sortie -1)) > 0;

	if (PSG==0 && PSG_mem==1)
	{
		if (listeHandle.size()!=0)
		{
			HandleSh_msg.data=listeHandle[0];
			cout << "envoie handle de A12 à A1 " << HandleSh_msg.data << endl;
			SendHandleShToA1.publish(HandleSh_msg);
			listeHandle.erase(listeHandle.begin());
		}
	}

	if (PSD==0 && PSD_mem==1)
	{
		if (listeHandle.size()!=0)
		{
			HandleSh_msg.data=listeHandle[0];
			cout << "envoie handle de A12 à A1 " << HandleSh_msg.data << endl;
			SendHandleShToA1.publish(HandleSh_msg);
			listeHandle.erase(listeHandle.begin());
		}
	}

	PSG_mem=PSG;
	PSD_mem=PSD;

	
}

//SwitchSensor
void A12::SwitchSensorCallback(const std_msgs::Int32::ConstPtr& msg)
{
	Aig_D = (msg->data & (int32_t)pow(2,2*num_aiguillage-2)) > 0;
	Aig_G = (msg->data & (int32_t)pow(2,2*num_aiguillage-1)) > 0;
}

void A12::getHandleA12Callback(const std_msgs::Int32::ConstPtr& msg)
{
	listeHandle.push_back(msg->data);
}

void A12::getHandleFromPoste2Callback(const std_msgs::Int32::ConstPtr& msg)
{
	listeHandle.push_back(msg->data);
}



//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////Commande Aiguillage/////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////

void A12::STOP_DROIT(){
	ShStop.publish(num_STOPd);
}
void A12::STOP_VIRAGE(){
	ShStop.publish(num_STOPg);
}


void A12::START_DROIT(){

	ShStart.publish(num_STOPd);

}
void A12::START_VIRAGE(){

	ShStart.publish(num_STOPg);

}


void A12::Virage()
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
			if(Aig_D) AigGauche.publish(num_AIG);
		}

		//Verouillage de l'aiguillage	
		AigVer.publish(num_AIG);
	}
	usleep(100000);

}

void A12::Droit()
{
	
	if(!Aig_D)
	{
		//Deverouillage de l'aiguillage	
		AigDev.publish(num_AIG);

		usleep(1000000);

		//Envoie ordre mouvement à droite
		AigDroit.publish(num_AIG);


		//Attente...
		while(!Aig_D)
		{
		
			ros::spinOnce();
			if(Aig_G) AigDroit.publish(num_AIG);
			usleep(100000);
		}	

		//Verouillage de l'aiguillage
		AigVer.publish(num_AIG);
	}

	usleep(100000);

	
}
	

float A12::get_time()
{
	client_get_vrep_time.call(srv_get_time);
	return (float) srv_get_time.response.simulationTime;

}

void A12::wait_vrep(float dt)
{
	float t=this->get_time();
	while(this->get_time()-t<dt)
	{
		ros::spinOnce();
	}

}

int A12::get_PS24()  //<---MODIF
{
	return PSD;
}

int A12::get_PS23()  //<---MODIF
{
	return PSG;
}

int A12::get_posAig_Droit()
{
	return Aig_D;
}


int A12::get_posAig_Virage()
{
	return Aig_G;
}


