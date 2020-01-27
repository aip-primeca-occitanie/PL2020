/*
 * ************************************* *
 * 	  Projet Long N7 2017  	         *
 * ************************************* *
*/

#include "commande.h"
#include <ros/ros.h>
#include <iostream>
#include <std_msgs/Int32.h>

using namespace std;

//the following are UBUNTU/LINUX ONLY terminal color codes.
#define RESET   "\033[0m"
#define BLACK   "\033[30m"      /* Black */
#define RED     "\033[31m"      /* Red */
#define GREEN   "\033[32m"      /* Green */
#define YELLOW  "\033[33m"      /* Yellow */
#define BLUE    "\033[34m"      /* Blue */
#define MAGENTA "\033[35m"      /* Magenta */
#define CYAN    "\033[36m"      /* Cyan */
#define WHITE   "\033[37m"      /* White */
#define BOLDBLACK   "\033[1m\033[30m"      /* Bold Black */
#define BOLDRED     "\033[1m\033[31m"      /* Bold Red */
#define BOLDGREEN   "\033[1m\033[32m"      /* Bold Green */
#define BOLDYELLOW  "\033[1m\033[33m"      /* Bold Yellow */
#define BOLDBLUE    "\033[1m\033[34m"      /* Bold Blue */
#define BOLDMAGENTA "\033[1m\033[35m"      /* Bold Magenta */
#define BOLDCYAN    "\033[1m\033[36m"      /* Bold Cyan */
#define BOLDWHITE   "\033[1m\033[37m"      /* Bold White */



Commande::Commande(ros::NodeHandle noeud, std::string executionPath)
{

	// Commande Stop/Start Navettes
	SubArretNavette = noeud.subscribe("/commande/ArretNavette", 1000, &Commande::ArretNavette, this);
	SubDemarreNavette = noeud.subscribe("/commande/DemarrerNavette", 1000, &Commande::DemarreNavette, this);

	//Commande aiguillages
	SubDeverouilleAiguillages = noeud.subscribe("/commande/DeverouilleAiguillage", 1000, &Commande::DeverouilleAiguillages, this);
	SubVerouilleAiguillages = noeud.subscribe("/commande/VerouilleAiguillage", 1000, &Commande::VerouilleAiguillages, this);
	SubAiguillagesGauches = noeud.subscribe("/commande/AiguillageGauche", 1000, &Commande::Aiguillagesgauches, this);
	SubAiguillagesDroits = noeud.subscribe("/commande/AiguillageDroit", 1000, &Commande::Aiguillagesdroits, this);

	// Publishers messages actionneurs
	cout<<"Initialisation publisher"<<endl;
	pub_navettes_stops = noeud.advertise<commande_locale::Msg_StopControl>("/commande/Simulation/Actionneurs_stops", 1);
	pub_actionneurs_simu_aiguillages = noeud.advertise<commande_locale::Msg_SwitchControl>("/commande/Simulation/Actionneurs_aiguillages", 1);
        pub_actionneurs_simu_pins = noeud.advertise<commande_locale::Msg_PinControl>("/commande/Simulation/Actionneurs_pins", 10);


	// Publisher tempts de traitement des 3 différentes taches

	pub_lancementTache1 = noeud.advertise<std_msgs::Int32>("/Cmde_tache1/lancementTache1", 10);
	pub_lancementTache3 = noeud.advertise<std_msgs::Int32>("/Cmde_tache3/lancementTache3", 10);
	pub_lancementTache4 = noeud.advertise<std_msgs::Int32>("/Cmde_tache4/lancementTache4", 10);
	
	// Publishers vers Postes - produit récupéré par robot
	pubProductTakenByRobotP1 = noeud.advertise<std_msgs::Int32>("/Cmde_P1/ProduitRecupere", 10);
	pubProductTakenByRobotP2 = noeud.advertise<std_msgs::Int32>("/Cmde_P2/ProduitRecupere", 10);
	pubProductTakenByRobotP3 = noeud.advertise<std_msgs::Int32>("/Cmde_P3/ProduitRecupere", 10);
	pubProductTakenByRobotP4 = noeud.advertise<std_msgs::Int32>("/Cmde_P4/ProduitRecupere", 10);

	// Publishers vers Postes - produit posé sur navette
	pubProductPutOnShuttleP1 = noeud.advertise<std_msgs::Int32>("/Cmde_P1/ProduitPose", 10);
	pubProductPutOnShuttleP2 = noeud.advertise<std_msgs::Int32>("/Cmde_P2/ProduitPose", 10);
	pubProductPutOnShuttleP3 = noeud.advertise<std_msgs::Int32>("/Cmde_P3/ProduitPose", 10);
	pubProductPutOnShuttleP4 = noeud.advertise<std_msgs::Int32>("/Cmde_P4/ProduitPose", 10);

	// Actionner ergots
	subPinOn = noeud.subscribe("/Poste_Cmde/SortirErgots", 10, &Commande::SortirErgotCallback, this);
	subPinOff = noeud.subscribe("/Poste_Cmde/RentrerErgots", 10, &Commande::RentrerErgotCallback, this);

	//pour savoir si les taches sont finis

	sub_tache1done= noeud.subscribe("/commande/Simulation/ProduitTraitement1fini", 10, &Commande::Tache1DoneCallback, this);
	sub_tache3done= noeud.subscribe("/commande/Simulation/ProduitTraitement3fini", 10, &Commande::Tache3DoneCallback, this);
	sub_tache4done= noeud.subscribe("/commande/Simulation/ProduitTraitement4fini", 10, &Commande::Tache4DoneCallback, this);


	tache1done=0;
	tache3done=0;
	tache4done=0;

	Dt=2;

	// Initialisation des Actionneurs

	for(int i=1;i<=24;i++) actionneurs_simulation_Stop.STOP[i] = 1;
	for(int i=1;i<=24;i++) actionneurs_simulation_Stop.GO[i] = 0;

	for(int i=1;i<=12;i++) actionneurs_simulation_Aiguillages.LOCK[i] = 0;
	for(int i=1;i<=12;i++) actionneurs_simulation_Aiguillages.RD[i] = 0;
	for(int i=1;i<=12;i++) actionneurs_simulation_Aiguillages.RG[i] = 0;

        for(int i=1;i<=8;i++) actionneurs_simulation_Pin.PINON[i] = 0;
        for(int i=1;i<=8;i++) actionneurs_simulation_Pin.PINOFF[i] = 1;


	
	



////////////////////////////////////////////////////////////////////
//Initialisation des produits à l'aide d'un fichier de configuration
////////////////////////////////////////////////////////////////////


// Récupération du chemin vers le Working_Folder
int count = 0 ;
int pos = 0 ;
while (count < 4)
	{
	if(executionPath[pos] == '/') count++;
	pos++;
	}
std::string Working_Folder = executionPath.substr(0,pos);

	
//Initialisation des produits à l'aide du fichier de configuration


	//Définition du chemin du fichier de config
std::string configFile = Working_Folder + "/ProductConfiguration.config";

std::ifstream streamConfigFile(configFile.c_str(), std::ios::in);
	
	if (streamConfigFile)
	{
	std::string pNameFF,destinationPart,contents;

	//saut des lignes d'entêtes, repèrage du start.
	while(1){
	std::getline(streamConfigFile,contents);
	std::size_t found = contents.find("Start");
  	if (found!=std::string::npos)
		{
		break;
		}
	}
	//Configuration nombre max de shuttle
	std::getline(streamConfigFile,contents);

	//Configuration temps entre lancement
	std::getline(streamConfigFile,contents);
	
	//GAMME/TEMPS
	while (std::getline(streamConfigFile, contents))
		{
		if (contents.find(':') != std::string::npos )
			{
			//ROS_INFO("%s",contents.c_str())	;
			std::size_t pos2 = contents.find(":");
			std::size_t pos3 = contents.find_last_of(":");
		
			pNameFF = contents.substr(0,pos2);
			destinationPart = contents.substr(pos2+1,pos3-pos2-1);
	
			int destination[10];
		
			int manRSize = 0; //manufacturing range size of the produit = number of operation

			char * cstr2 = new char [destinationPart.length()+1]; 
	  		std::strcpy (cstr2, destinationPart.c_str());	// création objet cstring

	  		// cstr now contains a c-string copy of str
			int n2 = 0; //compteur sur les destinations

			char * p2 = std::strtok (cstr2," ");
	  		while (p2!=NULL)
	  			{
				destination[n2++] = atoi(p2);
				manRSize++ ;
	  			p2 = std::strtok(NULL," ");
	  			}

	  		delete[] cstr2;


			char charName;
			charName = char(pNameFF.c_str()[0]-16);
			int pNumberBase = atoi(&charName) * 10 ; 
			int pNumber;
			int nextDestination;


	//  Pour gérer le n° de la prochaine destination d'un produit
			for (int i = 0; i < manRSize+1; i++)	
				{	
				pNumber = pNumberBase + i; 		
				if ( i+1 == manRSize+1 )  // Si i+1 est égal au nombre d'étapes de fabrication du produit 
					{
					nextDestination = 5; 	// Destination qui correspond à la sortie 
					}
				else 	nextDestination = destination[i];
				//ROS_INFO("pNumber : %d, nextdestination : %d",pNumber, nextDestination);
				initProduct(nextDestination,pNumber);
				}
			}
		}
	streamConfigFile.close(); //fermeture du fichier ProductConfiguration.txt ouvert en lecture//

	}

}

void Commande::lancementTache(int num_tache, float temps)
{
	std_msgs::Int32 msg;
	msg.data = temps;
	switch(num_tache)
	{
		default:
			cout << "Numéro de tache erroné en argument de lancementTache" << endl;
		break;

		case 1:
			pub_lancementTache1.publish(msg);
		break;

		case 3:
			pub_lancementTache3.publish(msg);
		break;

		case 4:
			pub_lancementTache4.publish(msg);
		break;	
	}
}

void Commande::Initialisation()
{
	for(int i=0;i<25;i++) PSx[i]=0;
	for(int i=0;i<13;i++) DxD[i]=0;
	for(int i=0;i<13;i++) DxG[i]=0;
	for(int i=0;i<11;i++) CPx[i]=0;
	for(int i=0;i<9;i++) CPIx[i]=0;

	for(int i=0;i<25;i++) STx[i]=1;
	for(int i=0;i<13;i++) RxD[i]=0;
	for(int i=0;i<13;i++) RxG[i]=0;

	for(int i=0;i<13;i++) Vx[i]=0;
	for(int i=0;i<13;i++) Dx[i]=0;
	for(int i=0;i<9;i++) PIx[i]=0;

	/*Dx[11]=1;
	Vx[11]=0;
	//RxG[11]=1;
	
	
	RxD[11]=1;

	for(int i=0;i<13;i++) actionneurs_simulation_Aiguillages.RD[i] = RxD[i];
	for(int i=0;i<13;i++) actionneurs_simulation_Aiguillages.RG[i] = RxG[i];
	for(int i=0;i<13;i++) actionneurs_simulation_Aiguillages.LOCK[i] = !Vx[i] && Dx[i];
	pub_actionneurs_simu_aiguillages.publish(actionneurs_simulation_Aiguillages);

	RxD[11]=0;

	//RxG[11]=0;


	Dx[11]=0;
	Vx[11]=1;*/
}

//////////////////////////////
// 	Callback Stop/Start
//////////////////////////////
void Commande::ArretNavette(const std_msgs::Int32::ConstPtr& msg)
{
	STx[msg->data]=0;
	for(int i=0;i<25;i++) actionneurs_simulation_Stop.STOP[i]=!STx[i];
	for(int i=0;i<25;i++) actionneurs_simulation_Stop.GO[i]=STx[i];
	pub_navettes_stops.publish(actionneurs_simulation_Stop);
	/*this->wait_vrep(2);
	STx[msg->data]=1;
	for(int i=0;i<25;i++) actionneurs_simulation_Stop.STOP[i]=!STx[i];
	for(int i=0;i<25;i++) actionneurs_simulation_Stop.GO[i]=STx[i];
	pub_navettes_stops.publish(actionneurs_simulation_Stop);*/
	
}

void Commande::DemarreNavette(const std_msgs::Int32::ConstPtr& msg)
{
	STx[msg->data]=1;
	for(int i=0;i<25;i++) actionneurs_simulation_Stop.STOP[i]=!STx[i];
	for(int i=0;i<25;i++) actionneurs_simulation_Stop.GO[i]=STx[i];
	pub_navettes_stops.publish(actionneurs_simulation_Stop);
	
}

//////////////////////////////
// 	Callback Aiguillages
//////////////////////////////
void Commande::DeverouilleAiguillages(const std_msgs::Int32::ConstPtr& msg)
{
	Dx[msg->data]=1;
	Vx[msg->data]=0;
}

void Commande::VerouilleAiguillages(const std_msgs::Int32::ConstPtr& msg)
{
	Dx[msg->data]=0;
	Vx[msg->data]=1;
}

void Commande::Aiguillagesgauches(const std_msgs::Int32::ConstPtr& msg)
{
	RxG[msg->data]=1;

	for(int i=0;i<13;i++) actionneurs_simulation_Aiguillages.RG[i] = RxG[i];
	for(int i=0;i<13;i++) actionneurs_simulation_Aiguillages.RD[i] = RxD[i];
	for(int i=0;i<13;i++) actionneurs_simulation_Aiguillages.LOCK[i] = !Vx[i] && Dx[i];
	pub_actionneurs_simu_aiguillages.publish(actionneurs_simulation_Aiguillages);

	RxG[msg->data]=0;
}

void Commande::Aiguillagesdroits(const std_msgs::Int32::ConstPtr& msg)
{
	RxD[msg->data]=1;

	for(int i=0;i<13;i++) actionneurs_simulation_Aiguillages.RD[i] = RxD[i];
	for(int i=0;i<13;i++) actionneurs_simulation_Aiguillages.RG[i] = RxG[i];
	for(int i=0;i<13;i++) actionneurs_simulation_Aiguillages.LOCK[i] = !Vx[i] && Dx[i];
	pub_actionneurs_simu_aiguillages.publish(actionneurs_simulation_Aiguillages);

	RxD[msg->data]=0;
}

///////////////////////////
// 	Callback Ergots
//////////////////////////
void Commande::SortirErgotCallback(const std_msgs::Int32::ConstPtr& msg)
{
	PIx[msg->data]=1;
        for(int i=1;i<=8;i++) actionneurs_simulation_Pin.PINON[i] = PIx[i];
        for(int i=1;i<=8;i++) actionneurs_simulation_Pin.PINOFF[i] = !PIx[i];

	pub_actionneurs_simu_pins.publish(actionneurs_simulation_Pin);
}

void Commande::RentrerErgotCallback(const std_msgs::Int32::ConstPtr& msg)
{
	PIx[msg->data]=0;
        for(int i=1;i<=8;i++) actionneurs_simulation_Pin.PINON[i] = PIx[i];
        for(int i=1;i<=8;i++) actionneurs_simulation_Pin.PINOFF[i] = !PIx[i];

	pub_actionneurs_simu_pins.publish(actionneurs_simulation_Pin);
}



//Fonction init du fichier configuration pour créer un produit
void Commande::initProduct(int nDestination, int pNumber)
{
		// Création dynamique de l'object product
	ProductPost* newProductPost = new ProductPost(nDestination,pNumber);

		// Insertion dans le map de la classe de la paire <key=pNumber,T=Product*>

	std::pair<std::map<int,ProductPost*>::iterator,bool> ret;			// ret permet d'avoir un retour de la fonction insert, il est faux si la key existe déjà dans la map
	ret = ProductsMap.insert(std::pair<int,ProductPost*>(newProductPost->productNumber,newProductPost));

	if (ret.second==false)	// Si un produit avec le même nom existe dèjà, celui-ci n'est pas ajouté à la collection
	{
    		ROS_WARN("Ordonnanceur : Un Produit de ce nom existe dèjà !"); 
  	} 

}

void Commande::Tache1DoneCallback(const std_msgs::Int32::ConstPtr& msg)
{
	tache1done=msg->data;
}

int Commande::get_tache1done()
{
	return tache1done;
}

void Commande::Tache3DoneCallback(const std_msgs::Int32::ConstPtr& msg)
{
	tache3done=msg->data;
}

int Commande::get_tache3done()
{
	return tache3done;
}

void Commande::Tache4DoneCallback(const std_msgs::Int32::ConstPtr& msg)
{
	tache4done=msg->data;
}

int Commande::get_tache4done()
{
	return tache4done;
}
//////////////////////////////////////////////////
// Fonctions haut niveau
//////////////////////////////////////////////////

//Fonction pour savoir si le produit a été pris par le robot
void Commande::PiecePrise(int numPoste)
{
	std_msgs::Int32 msg;
	msg.data = 1;
	switch(numPoste)
	{
		case 1:
			//ProduitEnP1=0;
			pubProductTakenByRobotP1.publish(msg);
		break;

		case 2:
			//ProduitEnP2=0;
			pubProductTakenByRobotP2.publish(msg);
		break;

		case 3:
			//ProduitEnP3=0;
			pubProductTakenByRobotP3.publish(msg);
		break;

		case 4:
			//ProduitEnP4=0;
			pubProductTakenByRobotP4.publish(msg);
		break;	
	}	
}


//Fonction pour savoir si le robot a posé un produit
void Commande::PieceDeposee(int numPoste)
{
	std_msgs::Int32 msg;
	msg.data = 1;
	switch(numPoste)
	{
		case 1:
			pubProductPutOnShuttleP1.publish(msg);
		break;

		case 2:
			pubProductPutOnShuttleP2.publish(msg);
		break;

		case 3:
			pubProductPutOnShuttleP3.publish(msg);
		break;

		case 4:
			pubProductPutOnShuttleP4.publish(msg);
		break;	
	}	
}

void Commande::wait_vrep(float dt)
{
	float t=this->get_time();
	while(this->get_time()-t<dt)
	{
		ros::spinOnce();
	}

}

float Commande::get_time()
{
	client_get_vrep_time.call(srv_get_time);
	return (float) srv_get_time.response.simulationTime;


}

