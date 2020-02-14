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

using namespace std;

// Initialisation du nombre de plateformes F
vrepController::vrepController(){
	nShuttleF = 0;

	repSim_startSimulation=false;
	repSim_pauseSimulation=false;
	repSim_loadModel=false;
	//repSim_removeModel=false;
	//repSim_getObjectHandle=false;
	repSim_changeColor=false;

	loop_rate = new ros::Rate(25);
}

// PAUSE
void vrepController::pause()
{
	pubSim_pauseSimulation.publish(msgSim_pauseSimulation);
	cout << "debut pause" << endl;
	while(!repSim_pauseSimulation&&ros::ok())
	{
		ros::spinOnce();
		loop_rate->sleep();
	}
	repSim_pauseSimulation = false;
	cout << "fin pause" << endl;
}

// PLAY
void vrepController::play()
{
	pubSim_startSimulation.publish(msgSim_startSimulation);
	cout << "debut start" << endl;
	while(!repSim_startSimulation&&ros::ok())
	{
		ros::spinOnce();
		loop_rate->sleep();
	}
	repSim_startSimulation = false;
	cout << "fin play" << endl;
}

// Chargement des modèles dans la simulation lors de son lancement
void vrepController::loadModelInit(int shuttleNumber)
{
	char shuttleChar;
	cout << "shuttleNumber:" << shuttleNumber << endl;
	if(shuttleNumber<0 || shuttleNumber>6) printf(" ATTENTION, LE NUMERO DU SHUTTLE DOIT ETRE COMPRIS ENTRE 0 ET 6 \n");
	else {
		if(shuttleNumber == 0) shuttleChar = (char)(90); // SI 0 -> ShuttleZ
		else shuttleChar = char(shuttleNumber+64);
		std::string shuttleName = "models/montrac/shuttle"+std::string(&shuttleChar)+".ttm";

		msgSim_loadModel.data = shuttleName;
		pubSim_loadModel.publish(msgSim_loadModel);
		cout << "debut load init" << endl;
		while(!repSim_loadModel&&ros::ok())
		{
			ros::spinOnce();
			loop_rate->sleep();
		}
		repSim_loadModel = false;
		cout << "fin load init" << endl;
	}
}


// Suppression des modèles dans la simulation
/*void vrepController::removeModel(int handle)

//objectName (string): name of the object. If possibe, don't rely on the automatic name adjustment mechanism, and always specify the full object name, including the #: if the object is "myJoint", specify "myJoint#", if the object is "myJoint#0", specify "myJoint#0", etc.
{
	msgSim_removeModel.data = handle;
	pubSim_removeModel.publish(msgSim_removeModel);
	cout << "debut remove model" << endl;
	while(!repSim_removeModel&&ros::ok())
	{
		ros::spinOnce();
		loop_rate->sleep();
	}
	repSim_removeModel = false;
	cout << "fin remove model" << endl;
}*/

//Obtention d'un handle d'objet dans la simulation
/*int32_t vrepController::getObjectHandle(std::string objectName)
{
	msgSim_getObjectHandle.data = objectName;
	pubSim_getObjectHandle.publish(msgSim_getObjectHandle);
	cout << "debut get objact handle" << endl;
	while(!repSim_getObjectHandle&&ros::ok())
	{
		ros::spinOnce();
		loop_rate->sleep();
	}
	repSim_getObjectHandle = false;
	cout << "fin getobjecthandle" << endl;
	return valueSim_getObjectHandle;
}
*/

// On ferme le processus vrep
void vrepController::close()
{
	system("pkill vrep");
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


	std::string VRepPath = executionPath.substr(0,pos+2)+ "CoppeliaSim";


	char final_command[1000];
	// on doit maintenant lancer coppelia a la main a coté et play la simu avant de launch
	sprintf(final_command, "cd %s &&(./coppeliaSim.sh -h -s0 ../sim/%s.ttt &)", VRepPath.c_str(), simulationFileName.c_str());
	system(final_command); // On execute VREP sans afficher la fenetre


	// Topic pour V-Rep
	pubSim_startSimulation = n.advertise<std_msgs::Byte>("/sim_ros_interface/services/vrep_controller/StartSimulation",100);
	subSim_startSimulation = n.subscribe("/sim_ros_interface/services/response/vrep_controller/StartSimulation",100,&vrepController::simStartSimulationCallback,this);

	pubSim_pauseSimulation = n.advertise<std_msgs::Byte>("/sim_ros_interface/services/vrep_controller/PauseSimulation",100);
	subSim_pauseSimulation = n.subscribe("/sim_ros_interface/services/response/vrep_controller/PauseSimulation",100,&vrepController::simPauseSimulationCallback,this);

	pubSim_loadModel = n.advertise<std_msgs::String>("/sim_ros_interface/services/vrep_controller/LoadModel",100);
	subSim_loadModel = n.subscribe("/sim_ros_interface/services/response/vrep_controller/LoadModel",100,&vrepController::simLoadModelCallback,this);

	//pubSim_removeModel = n.advertise<std_msgs::Int32>("/sim_ros_interface/services/vrep_controller/RemoveModel",100);
	//subSim_removeModel = n.subscribe("/sim_ros_interface/services/response/vrep_controller/RemoveModel",100,&vrepController::simRemoveModelCallback,this);

	//pubSim_getObjectHandle = n.advertise<std_msgs::String>("/sim_ros_interface/services/vrep_controller/GetObjectHandle",100);
	//subSim_getObjectHandle = n.subscribe("/sim_ros_interface/services/response/vrep_controller/GetObjectHandle",100,&vrepController::simGetObjectHandleCallback,this);

    pubSim_changeColor = n.advertise<std_msgs::Int32MultiArray>("/sim_ros_interface/services/vrep_controller/ChangeColor",100);
    subSim_changeColor = n.subscribe("/sim_ros_interface/services/response/vrep_controller/ChangeColor",100,&vrepController::simChangeColorCallback,this);

	sleep(1);
}

void vrepController::setSimulationFile(std::string fileName){
	this->SimulationFileName = fileName;
}


int vrepController::computeTableId(int poste)
{
	int id=-1;

	switch(poste)
	{
		case 1:
			id=0;
			break;
		case 2:
			id=1;
			break;
		case 3:
			id=3;
			break;
		case 4:
			id=4;
			break;
	}

	return id;
}

void vrepController::addProduct(int produit, int poste)
{
	msgSim_changeColor.data.clear();
	msgSim_changeColor.data.push_back(computeTableId(poste));
	int couleur=-1;
	switch(produit)
	{
		case 1:
			couleur=14;
			break;
		case 2:
			couleur=24;
			break;
		case 3:
			couleur=34;
			break;
		case 4:
			couleur=44;
			break;
		case 5:
			couleur=54;
			break;
		case 6:
			couleur=64;
			break;
	}
	msgSim_changeColor.data.push_back(couleur);

	msgSim_changeColor.data.push_back(0);
	msgSim_changeColor.data.push_back(0);
	msgSim_changeColor.data.push_back(0);

	pubSim_changeColor.publish(msgSim_changeColor);
	while(!repSim_changeColor && ros::ok())
	{
		ros::spinOnce();
		loop_rate->sleep();
	}
	repSim_changeColor=false;
}


/** Callbacks pour V-Rep **/
void vrepController::simChangeColorCallback(const std_msgs::Byte::ConstPtr& msg)
{
	repSim_changeColor=true;
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

/*void vrepController::simRemoveModelCallback(const std_msgs::Int32::ConstPtr& msg)
{
	valueSim_removeModel=msg->data;

	repSim_removeModel=true;
}*/

/*void vrepController::simGetObjectHandleCallback(const std_msgs::Int32::ConstPtr& msg)
{
	valueSim_getObjectHandle=msg->data;

	repSim_getObjectHandle=true;
}*/
