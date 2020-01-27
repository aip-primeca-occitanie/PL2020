#include "A10.h"


using namespace std;

A10::A10(ros::NodeHandle nh)
{
	cout<<"Initialisation du callback"<<endl;



	VREPsubStopSensor = nh.subscribe("vrep/StopSensor", 10, &A10::StopSensorCallback, this);
	VREPsubSwitchSensor = nh.subscribe("vrep/SwitchSensor", 10, &A10::SwitchSensorCallback, this);
	VREPsubRailSensor = nh.subscribe("vrep/RailSensor", 1000, &A10::RailSensorCallback, this);
	subNbShuttle = nh.subscribe("/A10/NombreShuttle",10, &A10::NbShuttleCallback, this);
	pub_demandeNbrSh = nh.advertise<std_msgs::Int32>("/A10/DemandeNbrSh", 10);
	getHandle = nh.subscribe("/HandleExchange/SendHandleShToA10", 1000, &A10::getHandleCallback, this);
	pubProductChange = nh.advertise<shuttles::msgShuttleChange>("/commande_navette/ShuttlePChange", 10);

	clientGetShuttleStatus = nh.serviceClient<shuttles::srvGetShuttleStatus>("/commande_navette/srvGetShuttleStatus");
	pubChangeColor = nh.advertise<commande_locale::Msg_Color>("/commande_locale/Change_Color", 10);


	client_get_vrep_time = nh.serviceClient<vrep_common::simRosGetInfo>("/vrep/simRosGetInfo");

	ShStop = nh.advertise<std_msgs::Int32>("/commande/ArretNavette", 10);
	ShStart = nh.advertise<std_msgs::Int32>("/commande/DemarrerNavette", 10);
	AigDev = nh.advertise<std_msgs::Int32>("/commande/DeverouilleAiguillage", 10);
	AigVer = nh.advertise<std_msgs::Int32>("/commande/VerouilleAiguillage", 10);
	AigGauche = nh.advertise<std_msgs::Int32>("/commande/AiguillageGauche", 10);
	AigDroit = nh.advertise<std_msgs::Int32>("/commande/AiguillageDroit", 10);


	SendHandleShToA11 = nh.advertise<std_msgs::Int32>("/HandleExchange/SendHandleShToA11", 1000);
	
	Dt=0.7;
	envoi=0;

	PS_mem=0;
	num_aiguillage = 10;
	num_capt_stop = 19;
	num_capt_sortie = 20;


	num_STOP.data=num_capt_stop;


	num_AIG.data=num_aiguillage;


	usleep(100000);
	this->STOP();

	mem_handle1=1;
	mem_handle2=1;

	//*********************************************************************************
	//****************initialisation des handles des navette***************************
	//*********************************************************************************

	pub_demandeNbrSh.publish(num_STOP); //le message sert à rien c'est juste pour activer le Callback correspondant
	listeHandleinit.push_back(590);
	listeHandleinit.push_back(612);
	listeHandleinit.push_back(639);
	listeHandleinit.push_back(665);
	listeHandleinit.push_back(690);
	listeHandleinit.push_back(717);
	int done=0;
	while (nbShuttle<1)
	{
	pub_demandeNbrSh.publish(num_STOP); //le message sert à rien c'est juste pour activer le Callback correspondant
	ros::spinOnce();
	if (done==0)
	{
	cout << "Initialisation ";
	done=1;	
	}
		usleep(50000);
		cout << ".";
	}
	cout << " " << endl;
	cout << "nbrShuttle : " << nbShuttle << endl;
	int k;
	for (k = 0 ; k < nbShuttle ; k++)
	{
		listeHandle.push_back(listeHandleinit[k]);
	}

}
 




//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////Callback////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////

void A10::NbShuttleCallback(const std_msgs::Int32::ConstPtr& msg)
{
	nbShuttle=msg->data;
}

void A10::getHandleCallback(const std_msgs::Int32::ConstPtr& msg)
{
	listeHandle.push_back(msg->data);
	listeHandledecolor.push_back(msg->data);
	cout << "handle A10 : "<< msg->data << endl;
}

void A10::StopSensorCallback(const std_msgs::Int32::ConstPtr& msg)
{

	PSD= (msg->data & (int32_t)pow(2,num_capt_stop -1)) > 0;
	PS = (msg->data & (int32_t)pow(2,num_capt_sortie -1)) > 0;
	if (PSD==0 && PS_mem==1)
	{
		if (listeHandle.size()!=0)
		{
			HandleSh_msg.data=listeHandle[0];
	  		if (Aig_D==1)
			{
				cout << "envoie handle de A10 à A11 " << HandleSh_msg.data << endl;
				SendHandleShToA11.publish(HandleSh_msg);
				listeHandle.erase(listeHandle.begin());
			}
		}	
	}
	PS_mem=PSD;
}


//SwitchSensor
void A10::SwitchSensorCallback(const std_msgs::Int32::ConstPtr& msg)
{
	Aig_D = (msg->data & (int32_t)pow(2,2*num_aiguillage-2)) > 0;
	Aig_G = (msg->data & (int32_t)pow(2,2*num_aiguillage-1)) > 0;
}

void A10::RailSensorCallback(const std_msgs::Int32::ConstPtr& msg)
{
	CP8 = (msg->data & (int32_t)pow(2,7)) > 0;
	//cout << "CP8 = "<< CP8 << endl;
	if ((CP8==0 && CP8_mem==1)&&(listeHandledecolor.size()!=0))
	{

	//cout << "DECOLORATION " << listeHandledecolor[0] << endl;	
	//cout << "Handle navette : " << endl;

	//ROS_INFO("Le produit est recupere - Navette vide");

	// Pour changer la couleur de la plateforme
	PlatformColor.Color = 0;
	PlatformColor.Handle = listeHandledecolor[0];
	pubChangeColor.publish(PlatformColor);



	listeHandledecolor.erase(listeHandledecolor.begin());

	}
	CP8_mem=CP8;
	

}



//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////Commande Aiguillage/////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////

void A10::STOP(){
	ShStop.publish(num_STOP);
}


void A10::START(){
	ShStart.publish(num_STOP);
}




void A10::Droit()
{
	//Deverouillage de l'aiguillage	haut
	if(!Aig_D)
	{
		AigDev.publish(num_AIG);

		usleep(100000);

		//Envoie ordre mouvement a droite haut
		AigDroit.publish(num_AIG);

		usleep(300000);

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

		//Verouillage des aiguillages	
		AigVer.publish(num_AIG);
	}
	
}

void A10::Virage()
{
	if(!Aig_G)
	{
		//Deverouillage de l'aiguillage	haut
		AigDev.publish(num_AIG);

		
		usleep(100000);

		//Envoie ordre mouvement a gauche haut
		AigGauche.publish(num_AIG);

		//usleep(300000);

		//Attente...
		while(!Aig_G)
		{
			if(Aig_D)
			{
				AigGauche.publish(num_AIG);
				usleep(100000);
			}
			ros::spinOnce();
		}

		//Verouillage des aiguillages	
		AigVer.publish(num_AIG);
	}
}

void A10::ModeManuel(){
	int aux;
	cout << "\nMode Manuel"<< endl;
	cout << "1) Gauche \t 2) Droite "<< endl;
	cin >> aux;

		if(aux==1){
			this->Virage();
	 	}
		if(aux==2){
			this->Droit();
		}
		
}


int A10::get_PS19()  //<---MODIF
{
	return PSD;
}

int A10::get_posAig_Droit()
{
	return Aig_D;
}


int A10::get_posAig_Virage()
{
	return Aig_G;
}




float A10::get_time()
{
	
	client_get_vrep_time.call(srv_get_time);
	return (float) srv_get_time.response.simulationTime;


}

void A10::wait_vrep(float dt)
{
	
	float t=this->get_time();
	while(this->get_time()-t<dt)
	{
		
		ros::spinOnce();
	}

}

bool A10::get_CP8()
{
	return CP8;
}


