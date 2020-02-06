/**** Maxime Maurin - Projet Long N7 2017 ****/

#include <ros/ros.h>
#include "scheduler.h"

#include <std_msgs/Bool.h>


bool on = false ;  //Variable globale pour servant à la mise en marche du mode auto par l'interface utilisateur
int sth = 0 ; 

//Callback du message recu par l'ordonnacement servant à mettre à jour la variable qui commande l'activation du noeud
void changeModeCallBack(const std_msgs::Bool::ConstPtr& msg)
{
on = msg->data;						
//ROS_INFO(" reception message : %d ",msg->data); 
}

int main (int argc, char **argv)
{

// Initialisation du noeud
ros::init(argc, argv, "ordonnancement");	
ros::NodeHandle nh;

// Définition de la fréquence
ros::Rate loop_rate(10);
std::string name = ros::this_node::getName();

// Souscription 
ros::Subscriber subMode = nh.subscribe("/ordonnancement/On_Off",10,changeModeCallBack); //abonnement au topic qui controle la mise en route de l'ordonnanceur
	//Le publisher qui publie sur ce topic se trouve dans le noeud controle_locale
	
// Initialisation de "l'ordonnanceur"
Scheduler myScheduler;
if(myScheduler.init(nh,argv[0]))
	{

	ROS_INFO("Noeud ordonnancement initialise : %s", name.c_str());

	sleep(1); // sert à laisser le temps au rosmaster d'initialiser correctement l'ensemble des publishers et subscribers. Sans ca il y a un risque de ne pas publier ou recevoir le premier message

  	while (ros::ok())
  		{
		
		if (on) //Mode Auto Activé
			{
			//ROS_INFO(" Ordo Start ");			
			myScheduler.launchNextSchedule();
			ros::spinOnce();
			loop_rate.sleep();
			}

		else	// Mode Auto Arreté, // On reste a l'écoute...
			{ 
			//ROS_INFO(" Ordo Stop ");
			ros::spinOnce();
			loop_rate.sleep(); 
			}
  		}
	}
else
	{
	ROS_ERROR("Probleme lors de l'initialisation du noeud d'ordonnancement, verifier le fichier de configuration ! Lancement Annule");  
	}

return 0;

}



