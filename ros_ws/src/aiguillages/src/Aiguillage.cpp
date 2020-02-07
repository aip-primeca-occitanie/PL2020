#include "Aiguillage.h"

using namespace std;

//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////Constructeur////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
Aiguillage::Aiguillage(ros::NodeHandle nh, int id_aiguillage)
{
	std::string num_str;
	switch(id_aiguillage){

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

	cout<<"Initialisation : "<<endl;
	num_aiguillage=id_aiguillage;

	loop_rate=new ros::Rate(25);

	//client_SetShuttleState = nh.serviceClient<shuttles::srvGetShuttleStatus>("/commande_navette/srvGetShuttleStatus");

	//VREPsubStopSensor = nh.subscribe("vrep/StopSensor", 1000, &Aiguillage::StopSensorCallback, this);//numéro de l'aiguillage à modifier
	//VREPsubRailSensor = nh.subscribe("vrep/RailSensor", 1000, &Aiguillage::RailSensorCallback, this);
	VREPsubSwitchSensor = nh.subscribe("vrep/SwitchSensor", 1000, &Aiguillage::SwitchSensorCallback, this);//Info sur position aiguillage

	sub_cmd_Droite = nh.subscribe("/commande/Simulation/AiguillageDroite"+num_str,1000,&Aiguillage::DroiteCallback,this);
	sub_cmd_Gauche = nh.subscribe("/commande/Simulation/AiguillageGauche"+num_str,1000,&Aiguillage::GaucheCallback,this);

	//SendShuttle_g = nh.advertise<aiguillages::ExchangeSh>("/IOShuttle/Aiguillage_A2", 1000);
	//SendShuttle_d = nh.advertise<aiguillages::ExchangeSh>("/IOShuttle/Aiguillage_P3", 1000);
	//ReceiveShuttle = nh.subscribe("/IOShuttle/Aiguillage2_Aiguillage", 1000, &Aiguillage::NewShuttleCallBack, this);

	//Dt=0.7;

	//ShStop = nh.advertise<std_msgs::Int32>("/commande/ArretNavette", 1000);
	//ShStart = nh.advertise<std_msgs::Int32>("/commande/DemarrerNavette", 1000);
	AigDev = nh.advertise<std_msgs::Int32>("/commande/DeverouilleAiguillage", 1000);
	AigVer = nh.advertise<std_msgs::Int32>("/commande/VerouilleAiguillage", 1000);
	AigGauche = nh.advertise<std_msgs::Int32>("/commande/AiguillageGauche", 1000);
	AigDroite = nh.advertise<std_msgs::Int32>("/commande/AiguillageDroite", 1000);

	//cout<<"time : "<<this->get_time()<<endl;

	//Nav_CPD=0;
	//Nav_CPG=0;
	//PS=0;

	//Numéro des capteurs et de l'aiguillage
	//num_aiguillage=1;//aiguillage
	//num_capt_stop=1;//capteur et stop en entrée de l'aiguillage
	//num_capt_droit=1;//capteur en sorti de l'aiguillage droit
	//num_capt_gauche=2;//capteur en sorti de l'aiguillage gauche

	num_AIG.data=num_aiguillage;
	//num_STOP.data=num_capt_stop;

	usleep(1000000);
	//this->STOP();
}

Aiguillage::~Aiguillage()
{
	delete loop_rate;
}

//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////Callback////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////

//StopSensor
/*void Aiguillage::StopSensorCallback(const std_msgs::Int32::ConstPtr& msg)
{

	bool PS_test;

	PS_test = (msg->data & (int32_t)pow(2,num_capt_stop-1)) > 0;
	if(PS_test) PS=1;


}*/

//RailSensor
/*void Aiguillage::RailSensorCallback(const std_msgs::Int32::ConstPtr& msg)
{
	CPD = (msg->data & (int32_t)pow(2,num_capt_droit-1)) > 0;
	CPG = (msg->data & (int32_t)pow(2,num_capt_gauche-1)) > 0;

	//Detection passage d'une navette en CP1
	if(CPD)
	{

		if(!CPD_past)
		{

			CPD_past=CPD;
			Nav_CPD=1;
		}
	}
	else
	{
		CPD_past=CPD;
	}

	//Detection passage d'une navette en CP2
	if(CPG)
	{

		if(!CPG_past)
		{

			CPG_past=CPG;
			Nav_CPG=1;
		}
	}
	else
	{
		CPG_past=CPG;
	}

}*/


//SwitchSensor
void Aiguillage::SwitchSensorCallback(const std_msgs::Int32::ConstPtr& msg)
{
	Aig_D = (msg->data & (int32_t)pow(2,2*num_aiguillage-2)) > 0;
	Aig_V = (msg->data & (int32_t)pow(2,2*num_aiguillage-1)) > 0;
}

/*void Aiguillage::NewShuttleCallBack(const aiguillages::ExchangeSh::ConstPtr& msg)
{
	std_msgs::Header h = msg->header;

	Sh* newShuttle = new Sh(msg->handle,h.seq);

	ShuttlesMap.insert(std::pair<int,Sh*>(0,newShuttle));


	/////test
	std::map<int,Sh*>::iterator it;
	Sh* shuttlePointer;

	 std::pair <std::multimap<int,Sh*>::iterator, std::multimap<int,Sh*>::iterator> ret2;
   	 ret2 = ShuttlesMap.equal_range(0);
   	 cout<<"handle des navettes dans la map :"<<endl;
   	 for (std::multimap<int,Sh*>::iterator it=ret2.first; it!=ret2.second; ++it)
	{
		shuttlePointer = it->second;
      		std::cout << " " << shuttlePointer->get_handle();
   		std::cout << endl;
	}

}*/
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////Commande Aiguillage/////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
/*void Aiguillage::STOP()
{
	ShStop.publish(num_STOP);
}

void Aiguillage::START()
{
	ShStart.publish(num_STOP);
}*/



void Aiguillage::GaucheCallback(const std_msgs::Int32::ConstPtr& msg)
{
	if(!Aig_V)
	{
		//Deverouillage de l'aiguillage
		AigDev.publish(num_AIG);
		usleep(100000);
		//Envoie ordre mouvement à gauche
		AigGauche.publish(num_AIG);

		//Attente...
		while(!Aig_V)
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


void Aiguillage::DroiteCallback(const std_msgs::Int32::ConstPtr& msg)
{
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
			if(Aig_V) AigDroite.publish(num_AIG);
			loop_rate->sleep();
		}

		//Verouillage de l'aiguillage
		AigVer.publish(num_AIG);
	}
	usleep(100000);
}


/*void Aiguillage::Aiguille_Navette()
{

	if(PS && this->get_Sh_Handle()!=-1)
	{


		srv_SetShuttleState.request.handle=this->get_Sh_Handle();
		client_SetShuttleState.call(srv_SetShuttleState);
		int destination = srv_SetShuttleState.response.destination;
		cout<<"destination : "<<destination<<endl;
		if(destination==3 || destination==4)
		{
			this->STOP();

			this->Droit();

			this->START();

			// Attente en fonction du temps de Vrep
			wait_vrep(Dt);
			this->STOP();


			PS=0;
			Send_Sh(1);

			// Attente que la navette passe jusqu'à ce que CP1 s'active
			while(!Nav_CPD)
			{
				ros::spinOnce();
				loop_rate->sleep();
			}

			// Remise à zéro de CP1
			Nav_CPD=0;
		}else
		{
			this->STOP();

			this->Gauche();

			this->START();

			// Attente en fonction du temps de Vrep
			wait_vrep(Dt);
			this->STOP();

			PS=0;
			Send_Sh(-1);

			// Attente que la navette passe jusqu'à ce que CP2 s'active
			while(!Nav_CPG)
			{
				ros::spinOnce();
				loop_rate->sleep();
			}

			// Remise à zéro de CP2
			Nav_CPG=0;
		}

	}

}*/



/*int Aiguillage::get_Sh_Handle()
{
	std::map<int,Sh*>::iterator it;

	Sh* shuttlePointer;

	std::pair <std::multimap<int,Sh*>::iterator, std::multimap<int,Sh*>::iterator> ret2;
   	ret2 = ShuttlesMap.equal_range(0);
   	cout<<"handle des navettes dans la map :"<<" --> "<<endl;
	int min=-1;
	int Sh_min=-1;
   	 for (std::multimap<int,Sh*>::iterator it=ret2.first; it!=ret2.second; ++it)
	{
		shuttlePointer = it->second;
      		std::cout << " " << shuttlePointer->get_handle();
		if(shuttlePointer->get_time()<min || min==-1)
		{
			min=shuttlePointer->get_time();
			Sh_min=shuttlePointer->get_handle();
		}
   		 std::cout << endl;
	}

	return Sh_min;
}*/


/*void Aiguillage::Send_Sh(int destination)
{
	std::map<int,Sh*>::iterator it;
	int handle_min;

	handle_min=this->get_Sh_Handle();

	HandleShuttle.handle=handle_min;
	if(destination==1)
	{
		SendShuttle_d.publish(HandleShuttle);
	}else if(destination==-1)
	{
		SendShuttle_g.publish(HandleShuttle);
	}

	Sh* shuttlePointer;

	std::pair <std::multimap<int,Sh*>::iterator, std::multimap<int,Sh*>::iterator> ret2;
   	ret2 = ShuttlesMap.equal_range(0);
   	 for (std::multimap<int,Sh*>::iterator it=ret2.first; it!=ret2.second; ++it)
	{
		shuttlePointer = it->second;
		if(shuttlePointer->get_handle()==handle_min)
		{
			ShuttlesMap.erase(it);
		}

	}

}*/

/*float Aiguillage::get_time()
{
	client_get_vrep_time.call(srv_get_time);
	return (float) srv_get_time.response.simulationTime;
}*/

/*void Aiguillage::wait_vrep(float dt)
{
	float t=this->get_time();
        while(this->get_time()-t<dt)
        {
                ros::spinOnce();
                loop_rate->sleep();
        }
}*/
