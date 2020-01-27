#include "A3.h"  

using namespace std;

A3::A3(ros::NodeHandle nh)
{
	cout<<"Initialisation du callback"<<endl;
	client_get_vrep_time = nh.serviceClient<vrep_common::simRosGetInfo>("/vrep/simRosGetInfo");

	VREPsubStopSensor = nh.subscribe("vrep/StopSensor", 10, &A3::StopSensorCallback, this);
	VREPsubSwitchSensor = nh.subscribe("vrep/SwitchSensor", 10, &A3::SwitchSensorCallback, this);
	VREPsubRailSensor = nh.subscribe("vrep/RailSensor", 10, &A3::RailSensorCallback, this);

	getHandle = nh.subscribe("/HandleExchange/SendHandleShToA3", 1000, &A3::getHandleCallback, this);

	SendHandleShtoA11= nh.advertise<std_msgs::Int32>("/HandleExchange/SendHandleShToA11", 1000);
	SendHandleShtoA10= nh.advertise<std_msgs::Int32>("/HandleExchange/SendHandleShToA10", 1000);



	ShStop = nh.advertise<std_msgs::Int32>("/commande/ArretNavette", 10);
	ShStart = nh.advertise<std_msgs::Int32>("/commande/DemarrerNavette", 10);
	AigDev = nh.advertise<std_msgs::Int32>("/commande/DeverouilleAiguillage", 10);
	AigVer = nh.advertise<std_msgs::Int32>("/commande/VerouilleAiguillage", 10);
	AigGauche = nh.advertise<std_msgs::Int32>("/commande/AiguillageGauche", 10);
	AigDroit = nh.advertise<std_msgs::Int32>("/commande/AiguillageDroit", 10);
	
	Dt=0.7;

	CPb=0;

	num_aiguillage = 3;
	num_capt_stop = 6;
	num_capt_sortie_haut = 20;
	num_capt_sortie_bas = 3;


	num_STOP.data=num_capt_stop;

	num_AIG.data=num_aiguillage;

	PS=0;
	PS_mem=0;

	mem_handle1=0;
	mem_handle2=0;
	
	
	usleep(1000000);
	this->STOP();

	

	
}
 



//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////Callback////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////

void A3::getHandleCallback(const std_msgs::Int32::ConstPtr& msg)
{
	listeHandle.push_back(msg->data);
}

void A3::StopSensorCallback(const std_msgs::Int32::ConstPtr& msg)
{

	PS=(msg->data & (int32_t)pow(2,num_capt_stop-1)) > 0;
	if (PS==0 && PS_mem==1)
	{

		if (listeHandle.size()!=0)
		{
			HandleSh_msg.data=listeHandle[0];
	  		if (Aig_D==1)
			{
				cout << "envoie handle de A13 à A10 " << HandleSh_msg.data << endl;
				SendHandleShtoA10.publish(HandleSh_msg);
				listeHandle.erase(listeHandle.begin());
			}
		
			if (Aig_G==1)
			{
				cout << "envoie handle de A3 à A11 " << HandleSh_msg.data << endl;
				SendHandleShtoA11.publish(HandleSh_msg);
				listeHandle.erase(listeHandle.begin());
			}
		}

	}
	PS_mem=PS;

	
}

//RailSensor
void A3::RailSensorCallback(const std_msgs::Int32::ConstPtr& msg)
{
	CPb = (msg->data & (int32_t)pow(2,num_capt_sortie_bas -1)) > 0;

}

//SwitchSensor
void A3::SwitchSensorCallback(const std_msgs::Int32::ConstPtr& msg)
{

	Aig_D = (msg->data & (int32_t)pow(2,2*num_aiguillage-2)) > 0;
	Aig_G = (msg->data & (int32_t)pow(2,2*num_aiguillage-1)) > 0;
}




//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////Commande Aiguillage/////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////

void A3::STOP(){
	ShStop.publish(num_STOP);
}

void A3::START(){

	ShStart.publish(num_STOP);
}


void A3::Droit()
{

	//Deverouillage de l'aiguillage
	AigDev.publish(num_AIG);

	usleep(100000);

	//Envoie ordre mouvement à droite
	AigDroit.publish(num_AIG);

	usleep(100000);

	//Attente...
		while(!Aig_D)
		{
			if(Aig_G) 
			{
				AigDroit.publish(num_AIG);
				usleep(100000);
			}
			ros::spinOnce();
		}

	//Verouillage de l'aiguillage	
	AigVer.publish(num_AIG);
	usleep(100000);
	
}

void A3::Virage()
{
	//On prévient que la navette veut faire demi tour
	
	//Deverouillage de l'aiguillage	
	AigDev.publish(num_AIG);

	usleep(100000);

	//Envoie ordre mouvement a gauche haut
	AigGauche.publish(num_AIG);

	//usleep(300000);

	//Attente...
		while(!Aig_G)
		{
			if(Aig_D) AigGauche.publish(num_AIG);
			usleep(100000);
			ros::spinOnce();
		}

	//Verouillage des aiguillages	
	AigVer.publish(num_AIG);
}

void A3::ModeManuel(){
	int aux;
	cout << "\nMode Manuel"<< endl;
	cout << "1) Demi-tour \t 2) Droite "<< endl;
	cin >> aux;

		if(aux==1){
			this->Virage();
	 	}
		if(aux==2){
			this->Droit();
		}
		
}


float A3::get_time()
{
	client_get_vrep_time.call(srv_get_time);
	return (float) srv_get_time.response.simulationTime;


}

void A3::wait_vrep(float dt)
{
	float t=this->get_time();
	while(this->get_time()-t<dt)
	{
		ros::spinOnce();
	}

}

int A3::get_PS6()  //<---MODIF
{
	return PS;
}

int A3::get_posAig_Droit()
{
	return Aig_D;
}


int A3::get_posAig_Virage()
{
	return Aig_G;
}

int A3::get_CP3()
{
	return CPb;
}

