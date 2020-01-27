#include "A2.h"  

using namespace std;

A2::A2(ros::NodeHandle nh)
{
	cout<<"Initialisation"<<endl;

	client_get_vrep_time = nh.serviceClient<vrep_common::simRosGetInfo>("/vrep/simRosGetInfo");

	VREPsubSensor = nh.subscribe("vrep/StopSensor", 1000, &A2::SensorCallback, this);
	VREPsubSwitchSensor = nh.subscribe("vrep/SwitchSensor", 1000, &A2::SwitchSensorCallback, this);

	getHandle = nh.subscribe("/HandleExchange/SendHandleShToA2", 1000, &A2::getHandleCallback, this);


	SendHandleShToA3= nh.advertise<std_msgs::Int32>("/HandleExchange/SendHandleShToA3", 1000);



	ShStop = nh.advertise<std_msgs::Int32>("/commande/ArretNavette", 1000);
	ShStart = nh.advertise<std_msgs::Int32>("/commande/DemarrerNavette", 1000);
	AigDev = nh.advertise<std_msgs::Int32>("/commande/DeverouilleAiguillage", 1000);
	AigVer = nh.advertise<std_msgs::Int32>("/commande/VerouilleAiguillage", 1000);
	AigGauche = nh.advertise<std_msgs::Int32>("/commande/AiguillageGauche", 1000);
	AigDroit = nh.advertise<std_msgs::Int32>("/commande/AiguillageDroit", 1000);

	cout<<"time : "<<this->get_time()<<endl;

	Dt=0.7;
	
	PS=0;
	PSG=0;
	PSD=0;
	PSG_mem=0;
	PSD_mem=0;

	num_aiguillage = 2;
	num_stop_droit = 5;
	num_stop_gauche = 4;
	num_capt_sortie = 6;

	mem_handle1=0;
	mem_handle2=0;


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

void A2::getHandleCallback(const std_msgs::Int32::ConstPtr& msg)
{
	listeHandle.push_back(msg->data);
}

void A2::SensorCallback(const std_msgs::Int32::ConstPtr& msg)
{

	PSG = (msg->data & (int32_t)pow(2,num_stop_gauche -1)) > 0;
	PSD = (msg->data & (int32_t)pow(2,num_stop_droit -1)) > 0;
	PS = (msg->data & (int32_t)pow(2,num_capt_sortie -1)) > 0;
	if ((PSG==0 && PSG_mem==1)||(PSD==0 && PSD_mem==1))
	{
		if (listeHandle.size()!=0)
		{
			HandleSh_msg.data=listeHandle[0];
			cout << "envoie handle à A3 de A2 " << HandleSh_msg.data << endl;
			SendHandleShToA3.publish(HandleSh_msg);
			listeHandle.erase(listeHandle.begin());
		}


	}
	PSG_mem=PSG;
	PSD_mem=PSD;
}

//SwitchSensor
void A2::SwitchSensorCallback(const std_msgs::Int32::ConstPtr& msg)
{
	Aig_D = (msg->data & (int32_t)pow(2,2*num_aiguillage-2)) > 0;
	Aig_G = (msg->data & (int32_t)pow(2,2*num_aiguillage-1)) > 0;
}



//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////Commande Aiguillage/////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////

void A2::STOP_VIRAGE(){
	ShStop.publish(num_STOPd);
}
void A2::STOP_DROIT(){
	ShStop.publish(num_STOPg);
}


void A2::START_VIRAGE(){

	ShStart.publish(num_STOPd);

}
void A2::START_DROIT(){

	ShStart.publish(num_STOPg);

}

void A2::Droit()
{
	

	if(!Aig_G)
	{
		//Deverouillage de l'aiguillage	
		AigDev.publish(num_AIG);

		usleep(1000000);

		//Envoie ordre mouvement à gauche
		AigGauche.publish(num_AIG);

	

		//Attente...
		while(!Aig_G)
		{
		
			ros::spinOnce();
			if(Aig_D) AigGauche.publish(num_AIG);
			usleep(100000);
			
		}
		

		//Verouillage de l'aiguillage	
		AigVer.publish(num_AIG);
	}
	usleep(100000);

}

void A2::Virage()
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

	usleep(1000000);
	
}

float A2::get_time()
{
	client_get_vrep_time.call(srv_get_time);
	return (float) srv_get_time.response.simulationTime;


}

void A2::wait_vrep(float dt)
{
	float t=this->get_time();
	while(this->get_time()-t<dt)
	{
		ros::spinOnce();
	}

}

int A2::get_PS4()  //<---MODIF
{
	return PSG;
}

int A2::get_PS5()  //<---MODIF
{
	return PSD;
}

int A2::get_posAig_Droit()
{
	return Aig_G;
}


int A2::get_posAig_Virage()
{
	return Aig_D;
}
