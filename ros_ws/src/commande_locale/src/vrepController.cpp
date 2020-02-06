/*
 * ********************************* *
 * Copyright 2016, STEC Projet Long. *
 * All rights reserved.  	     *
 * ********************************* *
 * Mise à jour par le Projet Long    *
 * ENSEEIHT 2017		     *
 * ********************************* *
*/

#include "vrepController.h"

// Initialisation du nombre de plateformes F
vrepController::vrepController(){
	nShuttleF = 0;

	repSim_getColor=false;
	repSim_changeColor=false;
	repSim_changeShuttleColor=false;
	repSim_startSimulation=false;
	repSim_pauseSimulation=false;
	repSim_loadModel=false;
	repSim_removeModel=false;
	repSim_getObjectHandle=false;

	loop_rate = new ros::Rate(25);
}

// PAUSE
void vrepController::pause()
{
	pubSim_pauseSimulation.publish(msgSim_pauseSimulation);
	while(!repSim_pauseSimulation)
	{
		ros::spinOnce();
		loop_rate->sleep();
	}
	repSim_pauseSimulation = false;
}

// PLAY
void vrepController::play()
{
	pubSim_startSimulation.publish(msgSim_startSimulation);
	while(!repSim_startSimulation)
	{
		ros::spinOnce();
		loop_rate->sleep();
	}
	repSim_startSimulation = false;
}

// SHUTTLE (pour charger une plateforme dans la simulation: plateforme aléatoire puis couleur de plateforme aléatoire)
int vrepController::LoadShuttle(char shuttleNumber, int type, int firstDestination)
{

		// Pour charger une plateforme F avec une couleur aléatoire

		std::string shuttleName = "models/montrac/shuttle"+std::string(&shuttleNumber,1)+".ttm";
		//ROS_INFO("------------ ShuttleName: %s" , shuttleName.c_str());

		msgSim_loadModel.data = shuttleName;

		// Choix du type de produit (changement de la couleur)
		int shuttleColor ;
		if (type == -1){
				shuttleColor = (rand() % 7)*10;
		}else {
				shuttleColor = type;
				ROS_INFO("%d",type);
		}

		//ROS_INFO("------------ ShuttleColor: %i" , shuttleColor);


		std::string shuttleNameComplete, platformNameComplete;
		if (nShuttleF == 0) {
			shuttleNameComplete = "Shuttle"+std::string(&shuttleNumber,1);
			platformNameComplete = "PlatformUp"+std::string(&shuttleNumber,1);
		}
		else{
			char aux = nShuttleF - 1 + '0';	 // transformation int -> char
			//ROS_INFO("-------- Aux : %c" ,aux);
			shuttleNameComplete = "Shuttle"+std::string(&shuttleNumber,1)+"#"+ std::string(&aux,1);
			platformNameComplete = "PlatformUp"+std::string(&shuttleNumber,1)+"#"+ std::string(&aux,1);
		}
		//ROS_INFO("------ ShuttleNameComplete: %s" , shuttleNameComplete.c_str());
		nShuttleF ++;

		pubSim_loadModel.publish(msgSim_loadModel);
		while(!repSim_loadModel)
	{
		ros::spinOnce();
		loop_rate->sleep();
	}
		repSim_loadModel = false;


		//Recuperation nom signal couleur : <handle de la navette>_color

		int handle_navette = valueSim_loadModel;
		char intStr[1000];
		sprintf (intStr, "%d", handle_navette);

		std::cout<<"test : "<<*intStr<<std::endl;
		int dim;

		dim=floor(log10(handle_navette))+1;
		std::cout<<"dim : "<<dim<<std::endl;

		std::string shuttleColorSignal= std::string(intStr,dim) + "_color"; ;



		std::cout<<"test : "<<shuttleColorSignal<<std::endl;

		int shuttleId=shuttleNumber-64;
		if(shuttleNumber=='Z')
			shuttleId=0;

		do
		{
			msgSim_changeShuttleColor.data.clear();
			msgSim_changeShuttleColor.data.push_back(shuttleId);
			msgSim_changeShuttleColor.data.push_back(shuttleColor);
			pubSim_changeShuttleColor.publish(msgSim_changeShuttleColor);
			while(!repSim_changeShuttleColor)
	{
		ros::spinOnce();
		loop_rate->sleep();
	}
			repSim_changeShuttleColor = false;

			msgSim_getColor.data = shuttleColorSignal;
			pubSim_getColor.publish(msgSim_getColor);
			while(!repSim_getColor)
	{
		ros::spinOnce();
		loop_rate->sleep();
	}
			repSim_getColor = false;

		}while(valueSim_getColor!=shuttleColor);


		int32_t handle_plateforme = vrepController::getObjectHandle(platformNameComplete);

		// std::cout << "Handle de la plateforme: " <<handle_plateforme <<std::endl;

		addNewShuttle(handle_navette, handle_plateforme,shuttleColor, firstDestination );

		Sh_Handle.handle = handle_navette;
		pub_Shuttle_Handle.publish(Sh_Handle);


		return handle_navette;


}

void vrepController::addNewShuttle(int handle_navette, int handle_plateforme, int type, int destination) {


	char aux[1000];
	sprintf(aux,"Shuttle%d",handle_navette);
	std::string name(aux);

	Cr_Shuttle.name = name;
	Cr_Shuttle.product = type;
	Cr_Shuttle.destination = destination;
	Cr_Shuttle.handle = handle_navette;
	Cr_Shuttle.handlePlatform = handle_plateforme;
	Cr_Shuttle.zone = 0;

	ROS_INFO( "Ajout navette d'handle : %d",handle_navette);
	createShuttle.publish(Cr_Shuttle);
}

// Chargement des modèles dans la simulation lors de son lancement
void vrepController::loadModelInit(char shuttleNumber)
{
	if(shuttleNumber>54 || shuttleNumber<48) printf(" ATTENTION, LE NUMERO DU SHUTTLE DOIT ETRE COMPRIS ENTRE 0 ET 6 \n");
	else {
		if(shuttleNumber == 48) shuttleNumber = char(74); // SI 0 -> ShuttleZ
		shuttleNumber = char(shuttleNumber+16);
		std::string shuttleName = "models/montrac/shuttle"+std::string(&shuttleNumber)+".ttm";

		msgSim_loadModel.data = shuttleName;
		pubSim_loadModel.publish(msgSim_loadModel);
		while(!repSim_loadModel)
	{
		ros::spinOnce();
		loop_rate->sleep();
	}
		repSim_loadModel = false;
	}
}


// Suppression des modèles dans la simulation
void vrepController::removeModel(int handle)

//objectName (string): name of the object. If possibe, don't rely on the automatic name adjustment mechanism, and always specify the full object name, including the #: if the object is "myJoint", specify "myJoint#", if the object is "myJoint#0", specify "myJoint#0", etc.
{
	msgSim_removeModel.data = handle;
	pubSim_removeModel.publish(msgSim_removeModel);
	while(!repSim_removeModel)
	{
		ros::spinOnce();
		loop_rate->sleep();
	}
	repSim_removeModel = false;
}

//Obtention d'un handle d'objet dans la simulation
int32_t vrepController::getObjectHandle(std::string objectName)
{
	msgSim_getObjectHandle.data = objectName;
	pubSim_getObjectHandle.publish(msgSim_getObjectHandle);
	while(!repSim_getObjectHandle)
	{
		ros::spinOnce();
		loop_rate->sleep();
	}
	repSim_getObjectHandle = false;
	return valueSim_getObjectHandle;
}


// On ferme le processus vrep
void vrepController::close()
{
	system("pkill vrep");
}

//Callback de suppression
void vrepController::deleteShuttleCallBack(const aiguillages::ExchangeSh::ConstPtr& msg)
{
	this->removeModel(msg->handle);
}

// Callback couleur
void vrepController::ColorCallBack(const commande_locale::Msg_Color::ConstPtr& msg)
{
	int Handle;
	Handle=msg->Handle;
	int Color;
	Color=msg->Color;


	char intStr[1000];
	sprintf (intStr, "%d", Handle);


	int dim;

	dim=floor(log10(Handle))+1;
	std::string shuttleColorSignal= std::string(intStr,dim) + "_color"; ;

	std::cout<<"test : "<<shuttleColorSignal<<std::endl;
	ROS_INFO("-------- ShuttleColorSignal: %s" , shuttleColorSignal.c_str());

	char shuttleNumber=shuttleColorSignal[7];
	int shuttleId = shuttleNumber-64;
	if(shuttleNumber=='Z')
		shuttleId=0;

	do
	{
		msgSim_changeShuttleColor.data.push_back(shuttleId);
		msgSim_changeShuttleColor.data.push_back(Color);
		pubSim_changeShuttleColor.publish(msgSim_changeShuttleColor);
		while(!repSim_changeShuttleColor)
	{
		ros::spinOnce();
		loop_rate->sleep();
	}
		repSim_changeShuttleColor = false;

		msgSim_getColor.data = shuttleColorSignal;
		pubSim_getColor.publish(msgSim_getColor);
		while(!repSim_getColor)
	{
		ros::spinOnce();
		loop_rate->sleep();
	}
		repSim_getColor = false;
	}while(valueSim_getColor!=Color);
}

void vrepController::init(ros::NodeHandle n,std::string executionPath, std::string simulationFileName)
{

// Launch of V-Rep
	int count = 0 ;
	int pos = executionPath.length()-1;
	while (count < 5 || pos<0)
	{
		if(executionPath[pos] == '/') count++;
		pos--;
	}
	if(pos<0) ROS_ERROR("pos negative !!!");

	std::string VRepPath = executionPath.substr(0,pos+2)+ "V-Rep";


	char final_command[1000];
	sprintf(final_command, "cd %s &&(./vrep.sh -h ../sim/%s.ttt &)", VRepPath.c_str(), simulationFileName.c_str());
	system(final_command); // On execute VREP sans afficher la fenetre

	//Launch of the different services
	sleep(2);

	// Topic pour V-Rep
	pubSim_getColor = n.advertise<std_msgs::String>("/sim_ros_interface/services/GetColor/vrep_controller",100);
	subSim_getColor = n.subscribe("/sim_ros_interface/services/response/GetColor/vrep_controller",100,&vrepController::simGetColorCallback,this);

	pubSim_changeColor = n.advertise<std_msgs::Int32MultiArray>("/sim_ros_interface/services/ChangeColor/vrep_controller",100);
	subSim_changeColor = n.subscribe("/sim_ros_interface/services/response/ChangeColor/vrep_controller",100,&vrepController::simChangeColorCallback,this);

	pubSim_changeShuttleColor = n.advertise<std_msgs::Int32MultiArray>("/sim_ros_interface/services/ChangeShuttleColor/vrep_controller",100);
	subSim_changeShuttleColor = n.subscribe("/sim_ros_interface/services/response/ChangeShuttleColor/vrep_controller",100,&vrepController::simChangeShuttleColorCallback,this);

	pubSim_startSimulation = n.advertise<std_msgs::Byte>("/sim_ros_interface/services/StartSimulation/vrep_controller",100);
	subSim_startSimulation = n.subscribe("/sim_ros_interface/services/response/StartSimulation/vrep_controller",100,&vrepController::simStartSimulationCallback,this);

	pubSim_pauseSimulation = n.advertise<std_msgs::Byte>("/sim_ros_interface/services/PauseSimulation/vrep_controller",100);
	subSim_pauseSimulation = n.subscribe("/sim_ros_interface/services/response/PauseSimulation/vrep_controller",100,&vrepController::simPauseSimulationCallback,this);

	pubSim_loadModel = n.advertise<std_msgs::String>("/sim_ros_interface/services/LoadModel/vrep_controller",100);
	subSim_loadModel = n.subscribe("/sim_ros_interface/services/response/LoadModel/vrep_controller",100,&vrepController::simLoadModelCallback,this);

	pubSim_removeModel = n.advertise<std_msgs::Int32>("/sim_ros_interface/services/RemoveModel/vrep_controller",100);
	subSim_removeModel = n.subscribe("/sim_ros_interface/services/response/RemoveModel/vrep_controller",100,&vrepController::simRemoveModelCallback,this);

	pubSim_getObjectHandle = n.advertise<std_msgs::String>("/sim_ros_interface/services/GetObjectHandle/vrep_controller",100);
	subSim_getObjectHandle = n.subscribe("/sim_ros_interface/services/response/GetObjectHandle/vrep_controller",100,&vrepController::simGetObjectHandleCallback,this);

	pub_Shuttle_Handle = n.advertise<aiguillages::ExchangeSh>("/commande_locale/New_Shuttle_Handle", 10);
	DeleteShuttle = n.subscribe("/commande_locale/Delete_Shuttle", 10, &vrepController::deleteShuttleCallBack, this);
	createShuttle = n.advertise<shuttles::msgShuttleCreate>("/commande_navette/AddShuttle",10);
	ChangeColor = n.subscribe("/commande_locale/Change_Color", 10, &vrepController::ColorCallBack, this);
}

void vrepController::setSimulationFile(std::string fileName){
	this->SimulationFileName = fileName;
}


/** Callbacks pour V-Rep **/
void vrepController::simGetColorCallback(const std_msgs::Int32::ConstPtr& msg)
{
	valueSim_getColor=msg->data;

	repSim_getColor=true;
}

void vrepController::simChangeColorCallback(const std_msgs::Byte::ConstPtr& msg)
{
	repSim_changeColor=true;
}

void vrepController::simChangeShuttleColorCallback(const std_msgs::Byte::ConstPtr& msg)
{
	repSim_changeShuttleColor=true;
}

void vrepController::simStartSimulationCallback(const std_msgs::Byte::ConstPtr& msg)
{
	repSim_startSimulation=true;
}

void vrepController::simPauseSimulationCallback(const std_msgs::Byte::ConstPtr& msg)
{
	repSim_pauseSimulation=true;
}

void vrepController::simLoadModelCallback(const std_msgs::Int32::ConstPtr& msg)
{
	valueSim_loadModel=msg->data;

	repSim_loadModel=true;
}

void vrepController::simRemoveModelCallback(const std_msgs::Int32::ConstPtr& msg)
{
	valueSim_removeModel=msg->data;

	repSim_removeModel=true;
}

void vrepController::simGetObjectHandleCallback(const std_msgs::Int32::ConstPtr& msg)
{
	valueSim_getObjectHandle=msg->data;

	repSim_getObjectHandle=true;
}
