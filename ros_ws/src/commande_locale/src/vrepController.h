/*
 * ********************************* *
 * Copyright 2016, STEC Projet Long. *
 * All rights reserved.  	     *
 * ********************************* *
 * Mise à jour par le Projet Long    *
 * ENSEEIHT 2017		     *
 * ********************************* *
*/

#ifndef VREP_CONTROLLER
#define VREP_CONTROLLER

#include <ros/ros.h>
#include <vector>
#include <string>
#include <math.h> 
#include <std_msgs/Int32.h>
#include <std_msgs/String.h>
#include <std_msgs/Byte.h>
#include <std_msgs/Int32MultiArray.h>

#include <shuttles/srvGetShuttleStatus.h>
#include <shuttles/srvGetEmptyShuttles.h>
#include <shuttles/msgShuttleChange.h>
#include <shuttles/msgShuttleCreate.h>

#include "commande_locale/Msg_Color.h"

#include <aiguillages/ExchangeSh.h>


class vrepController
{
	private:
		//** Topic V-Rep **//

		// GetColor
		ros::Publisher pubSim_getColor;
		std_msgs::String msgSim_getColor;
		ros::Subscriber subSim_getColor;
		bool repSim_getColor;
		int valueSim_getColor;

		// ChangeColor
		ros::Publisher pubSim_changeColor;
		std_msgs::Int32MultiArray msgSim_changeColor;
		ros::Subscriber subSim_changeColor;
		bool repSim_changeColor;

		// ChangeShuttleColor
		ros::Publisher pubSim_changeShuttleColor;
		std_msgs::Int32MultiArray msgSim_changeShuttleColor;
		ros::Subscriber subSim_changeShuttleColor;
		bool repSim_changeShuttleColor;
		
		// StartSimulation
		ros::Publisher pubSim_startSimulation;
		std_msgs::Byte msgSim_startSimulation;
		ros::Subscriber subSim_startSimulation;
		bool repSim_startSimulation;
		
		// PauseSimulation
		ros::Publisher pubSim_pauseSimulation;
		std_msgs::Byte msgSim_pauseSimulation;
		ros::Subscriber subSim_pauseSimulation;
		bool repSim_pauseSimulation;
		
		// LoadModel
		ros::Publisher pubSim_loadModel;
		std_msgs::String msgSim_loadModel;
		ros::Subscriber subSim_loadModel;
		bool repSim_loadModel;
		int valueSim_loadModel;

		// RemoveModel
		ros::Publisher pubSim_removeModel;
		std_msgs::Int32 msgSim_removeModel;
		ros::Subscriber subSim_removeModel;
		bool repSim_removeModel;
		int valueSim_removeModel;

		// GetObjectHandle
		ros::Publisher pubSim_getObjectHandle;
		std_msgs::String msgSim_getObjectHandle;
		ros::Subscriber subSim_getObjectHandle;
		bool repSim_getObjectHandle;
		int valueSim_getObjectHandle;
		

		// Gestion de navettes
		ros::Publisher pub_Shuttle_Handle;
		aiguillages::ExchangeSh Sh_Handle;
		
		ros::Subscriber DeleteShuttle;
		ros::Publisher createShuttle;
		shuttles::msgShuttleCreate Cr_Shuttle;

		ros::Subscriber ChangeColor;
		
		int nShuttleF;
		std::string SimulationFileName;

		ros::Rate* loop_rate;
	public:
		vrepController ();
		void init(ros::NodeHandle n,std::string executionPath, std::string simulationFileName);
		void close();
		void loadModelInit(char shuttleNumber);
		void removeModel(int32_t handle);
		int32_t getObjectHandle(std::string);
		void waitVrep();
		void play();
		void pause();
		void deleteShuttleCallBack(const aiguillages::ExchangeSh::ConstPtr& msg);

		int LoadShuttle(char shuttleNumber, int product, int firstDestination);
		void setSimulationFile(std::string);
		void addNewShuttle(int handle_navette, int handle_plateforme, int type, int destination); 
		void ColorCallBack(const commande_locale::Msg_Color::ConstPtr& msg);

		// Callbacks pour V-Rep
		void simGetColorCallback(const std_msgs::Int32::ConstPtr& msg);
		void simChangeColorCallback(const std_msgs::Byte::ConstPtr& msg);
		void simChangeShuttleColorCallback(const std_msgs::Byte::ConstPtr& msg);
		void simStartSimulationCallback(const std_msgs::Byte::ConstPtr& msg);
		void simPauseSimulationCallback(const std_msgs::Byte::ConstPtr& msg);
		void simLoadModelCallback(const std_msgs::Int32::ConstPtr& msg);
		void simRemoveModelCallback(const std_msgs::Int32::ConstPtr& msg);
		void simGetObjectHandleCallback(const std_msgs::Int32::ConstPtr& msg);
};	

#endif
