/*
 * ********************************* *
 * Copyright 2016, STEC Projet Long. *
 * All rights reserved.  	     *
 * ********************************* *
 * Mise à jour par le Projet Long    *
 * ENSEEIHT 2017		     *
 * ********************************* *
*/


using namespace std; 

#include <iostream>
#include "vrepController.h"  
#include "UI.h"
#include "inOutController.h"
#include "configuration.h"


#include <ros/ros.h>

int main(int argc, char **argv)
{	
			//Initialisation du noeud ROS
			ros::init(argc, argv, "commande_locale");
			ros::NodeHandle nh;
			
			ROS_INFO("Simulation file: %s \n", argv[1]);

			// VREP CONTROLLER
			vrepController VREPController;	
			VREPController.init(nh,argv[0], argv[1]);
			
			// CONFIGURATION
			Configuration config(&VREPController);
			config.init(nh, argv[0]);

			// USER INTERFACE
			UI userInterface(&VREPController,&config);
			userInterface.init(nh);

			// IN & OUT CONTROLLER
			inOutController IOController(&userInterface, &VREPController, &config);
			IOController.init(nh);

			// Demarrage de la simu pour avoir la premiere image
			VREPController.play();
			
			//Creation des navettes
			
			int nbNavettes;
			nbNavettes = config.getNbNavettes();
			ros::Publisher pubCreateShuttle = nh.advertise<scheduling::Msg_LoadShuttle>("/scheduling/NextProduct",10);
			scheduling::Msg_LoadShuttle mymsg;

			if (nbNavettes>=1)
			{
			mymsg.shuttleType = 'A';
			mymsg.product = 0;
			pubCreateShuttle.publish(mymsg);
			std::cout << "Creation navette 1" << endl;
			}
			
			if (nbNavettes>=2)
			{
			mymsg.shuttleType = 'B';
			mymsg.product = 0;
			pubCreateShuttle.publish(mymsg);
			std::cout << "Creation navette 2" << endl;
			}

			if (nbNavettes>=3)
			{
			mymsg.shuttleType = 'C';
			mymsg.product = 0;
			pubCreateShuttle.publish(mymsg);
			std::cout << "Creation navette 3" << endl;
			}

			if (nbNavettes>=4)
			{
			mymsg.shuttleType = 'D';
			mymsg.product = 0;
			pubCreateShuttle.publish(mymsg);
			std::cout << "Creation navette 4" << endl;
			}

			if (nbNavettes>=5)
			{
			mymsg.shuttleType = 'E';
			mymsg.product = 0;
			pubCreateShuttle.publish(mymsg);
			std::cout << "Creation navette 5" << endl;
			}

			if (nbNavettes>=6)
			{
			mymsg.shuttleType = 'F';
			mymsg.product = 0;
			pubCreateShuttle.publish(mymsg);
			std::cout << "Creation navette 6" << endl;
			}

			// Pause pour laisser à l'utilisateur le soin de lancer la simu avec le boutton Play
			VREPController.pause();

			ros::Rate loop_rate(30);

			while (ros::ok()) // Tant que ROS est actif,
			{
				ros::spinOnce(); // on lance les callback correspondant aux messages entrants.
				loop_rate.sleep();
			}				
				
			userInterface.close(); // Si Ctrl+C -> On ferme la fenetre de l'UI et on quitte la simulation
			VREPController.close();
			return 0;
}
