/*
 * ************************************* *
 * 	  Projet Long N7 2017  	         *
 * ************************************* *
*/

#include "Poste3.h"  

using namespace std;

//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////Constructeur////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
Poste3::Poste3(ros::NodeHandle nh,  std::string executionPath)
{
	cout<<"Initialisation du poste 3"<<endl;
	

	pub_updateTache= nh.advertise<std_msgs::Int32>("/P3_Tache/ProduitDone", 10);

	getHandle = nh.subscribe("/HandleExchange/SendHandleShToPoste3", 1000, &Poste3::getHandleCallback, this);

	SendHandleShtoPoste4 = nh.advertise<std_msgs::Int32>("/HandleExchange/SendHandleShToPoste4", 1000);

//Récupération de la position du robot

	robotPosition = nh.subscribe("/robot/PositionRobot2", 10, &Poste3::PositionRobotCallback, this);


// Pour connaitre l'état des capteurs
	VREPsubStopSensor = nh.subscribe("vrep/StopSensor", 10, &Poste3::StopSensorCallback, this);

// Pour arréter et démarrer les navettes
	ShStop = nh.advertise<std_msgs::Int32>("/commande/ArretNavette", 20);
	ShStart = nh.advertise<std_msgs::Int32>("/commande/DemarrerNavette", 20);

// Pour gérer les ergots
	pubPinOn = nh.advertise<std_msgs::Int32>("/Poste_Cmde/SortirErgots", 10);
	pubPinOff = nh.advertise<std_msgs::Int32>("/Poste_Cmde/RentrerErgots", 10);

// Pour recevoir et envoyer l'etat des navettes	
	subReceiveShuttle = nh.subscribe("/IOShuttle/A1_P3", 10, &Poste3::NewShuttleCallback, this);
	pubSendShuttle = nh.advertise<std_msgs::Int32>("/IOShuttle/P3_P4", 10);

// Communication avec Commande
	subStartShuttle = nh.subscribe("/Cmde_P3/Demarrage", 10, &Poste3::StartShuttleCallback, this);
	subProductPutOnShuttle = nh.subscribe("/Cmde_P3/ProduitPose", 10, &Poste3::ProductPutOnShuttleCallback, this);
	subProductTakenByRobot = nh.subscribe("/Cmde_P3/ProduitRecupere", 10, &Poste3::ProductTakenByRobotCallback, this);

	ProductFromPost = nh.advertise<std_msgs::Int32>("/Cmde_P3/ProduitPrise", 20);

	pubEmptyShuttleReady= nh.advertise<std_msgs::Int32>("/Cmde_P3/NavetteVidePrete", 10);
	pubProductInPost= nh.advertise<std_msgs::Int32>("/Cmde_P3/ProduitPresentP3", 10);

// Communication avec Tâche
	subTaskFinished = nh.subscribe("/Tache_P3/Tache3Finie", 10, &Poste3::TaskFinishedCallback, this);
	pubProductToTask= nh.advertise<std_msgs::Int32>("/P3_Tache/ProduitATraiter", 10);
	
// Pour communiquer avec le noeud de navettes
	clientGetShuttleStatus = nh.serviceClient<shuttles::srvGetShuttleStatus>("/commande_navette/srvGetShuttleStatus");

	pubProductChange = nh.advertise<shuttles::msgShuttleChange>("/commande_navette/ShuttlePChange", 10);
	pubDestinationChange = nh.advertise<shuttles::msgShuttleChange>("/commande_navette/ShuttleDChange", 10);

// Pour gérer les couleurs des plateformes
	pubChangeColor = nh.advertise<commande_locale::Msg_Color>("/commande_locale/Change_Color", 10);

// Pour initialiser la gestion des capteurs
	PS2_past=0;

	PS_mem=0;
// Pour initialiser les postes
	tache=0;
	poste_libre=1;
	prod_recup=0;	//variable à 1 quand le robot récupère un produit sur une navette pour la traiter

	msg_traite.data=1;
	Prise_msg.data=1;

	num_capt_stop=2;
	num_STOP.data=num_capt_stop;

	num_ergot=1;
	num_PIN.data=num_ergot;

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
	std::string pNameFF,destinationPart,jobTimePart,contents;

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
			ROS_INFO("Product %s",pNameFF.c_str());
			destinationPart = contents.substr(pos2+1,pos3-pos2-1);
			ROS_INFO("destination part =%s",destinationPart.c_str());
			jobTimePart = contents.substr(pos3+1);
			int destination[10];
			int jobTime[10];
			int manRSize = 0; //manufacturing range size of the produit = number of operation

			char * cstr2 = new char [destinationPart.length()+1]; 
	  		std::strcpy (cstr2, destinationPart.c_str());	// création objet cstring
			char * cstr3 = new char [jobTimePart.length()+1]; 
	  		std::strcpy (cstr3, jobTimePart.c_str());

	  		// cstr now contains a c-string copy of str
			int n2 = 0; //compteur sur les destinations
			int n3 = 0; //compteur sur les temps de fabrications
	 	 	

			char * p2 = std::strtok (cstr2," ");
	  		while (p2!=NULL)
	  			{
				destination[n2++] = atoi(p2);
				manRSize++ ;
	  			p2 = std::strtok(NULL," ");
	  			}

			char * p3 = std::strtok (cstr3," ");
			while (p3!=NULL)
	  			{
				jobTime[n3++] = atoi(p3);
	  			p3 = std::strtok(NULL," ");
	  			}

	  		delete[] cstr2;
			delete[] cstr3; 


			char charName;
			charName = char(pNameFF.c_str()[0]-16);
			int pNumberBase = atoi(&charName) * 10 ; 
			int pNumber;
			int nextDestination;


	//  Pour gérer le n° de la prochaine destination d'un produit
			for (int i = 0; i < manRSize; i++)	
				{

				if(destination[i] == 3)		
					{
					pNumber = pNumberBase + i; 		
					if ( i+1 == manRSize )  // Si i+1 est égal au nombre d'étapes de fabrication du produit 
						{
						nextDestination = 5; 	// Destination qui correspond à la sortie 
						}
					else 	nextDestination = destination[i+1]; // On affecte la prochaine destination définie par le gamme
					initProduct(nextDestination,pNumber);
					}
	
				}
		
			}
		}

	streamConfigFile.close(); //fermeture du fichier ProductConfiguration.txt ouvert en lecture//

}
else ROS_ERROR("Impossible d'ouvrir le fichier ProductConfiguration.txt !");


}
// Fin de l'initialisation


//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////Callback////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////


void Poste3::PositionRobotCallback(const std_msgs::Int32::ConstPtr& msg)
{
	position = msg->data;
}

void Poste3::getHandleCallback(const std_msgs::Int32::ConstPtr& msg)
{
	listeHandle.push_back(msg->data);
}

//StopSensor
void Poste3::StopSensorCallback(const std_msgs::Int32::ConstPtr& msg)
{
	//Récupération de la valeur de PS2
	 	PS2 = (msg->data & (int32_t)pow(2,1)) > 0;
	if (PS2==0 && PS_mem==1)
	{

		if (listeHandle.size()!=0)
		{
			HandleSh_msg.data=listeHandle[0];
			//cout << "envoie handle de A2 à poste 4 " << HandleSh_msg.data << endl;
			SendHandleShtoPoste4.publish(HandleSh_msg);
			listeHandle.erase(listeHandle.begin());
		}

	}
	PS_mem=PS2;

}

//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////   Commande Poste  /////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////

// Fonction Callback pour remplir une liste (FIFO) avec les handles des navettes créées
void Poste3::NewShuttleCallback (const aiguillages::ExchangeSh::ConstPtr& msg) {

	int shuttleNew =  msg->handle;
	int destination;
	int produit;

		ROS_INFO("Recuperation via service info navette d'handle %d",shuttleNew );
		ShuttleInfo.request.handle = shuttleNew;  	

	if (clientGetShuttleStatus.call(ShuttleInfo)){
		ROS_INFO("%s , destination %d , produit %d, handle de la plateforme %d, zone %d" ,ShuttleInfo.response.name.c_str(),ShuttleInfo.response.destination ,ShuttleInfo.response.product, ShuttleInfo.response.handlePlatform, ShuttleInfo.response.zone);
		
		handlesShuttles.push_back(shuttleNew);	//Pour remplir la liste par le bas
		handlesShuttlesMain.push_back(shuttleNew);	//Pour remplir la liste par le bas

		destination = ShuttleInfo.response.destination;
		destinationsShuttles.push_back(destination);	
		destinationsShuttlesMain.push_back(destination);	

		produit = ShuttleInfo.response.product;
		productShuttles.push_back(produit);
		productShuttlesMain.push_back(produit);

		//Pour afficher le contenu de la liste
 		std::cout << "Contenu de la liste des handles: "<<std::endl;
  		list<int>::iterator p = handlesShuttles.begin();

  		while(p != handlesShuttles.end()) {
   			std::cout << *p << " "<<std::endl;
    			p++;
		}

 		std::cout << "Contenu de la liste des destinations: "<<std::endl;
  		list<int>::iterator m = destinationsShuttles.begin();

  		while(m != destinationsShuttles.end()) {
   			std::cout << *m << " "<<std::endl;
    			m++;
		}

 		std::cout << "Contenu de la liste des produits: "<<std::endl;
  		list<int>::iterator n = productShuttles.begin();

  		while(n != productShuttles.end()) {
   			std::cout << *n << " "<<std::endl;
    			n++;
		}
	}

	else{
		ROS_WARN("Cette navette n'existe pas");
	}
}

// Fonction Callback si la commande veut redémarrer la navette
void Poste3::StartShuttleCallback(const std_msgs::Int32::ConstPtr& msg){



	std_msgs::Int32 num_handle;

	pubPinOff.publish(num_PIN);
	ShStart.publish(num_STOP);

	ROS_INFO("Redemarrage de la navette");

	while(PS2_past && PS2){
		ros::spinOnce();
	}

	num_handle.data = handlesShuttles.front();
			
	PS2_past = PS2;
	poste_libre = 1;
	
	pubSendShuttle.publish(num_handle); // Pour envoyer le numéro du handle de la navette à P2

	// Pour supprimer la première ligne des listes
	handlesShuttles.pop_front();
	destinationsShuttles.pop_front();
	productShuttles.pop_front();		
}

//Fonction Callback si un produit est posé sur la navette
void Poste3::ProductPutOnShuttleCallback(const std_msgs::Int32::ConstPtr& msg){

	cout << "position robot 2 : " << position << endl;

	if (position==1)
	{
		cout << "condition depose sur table ok" << endl;
		pubProductToTask.publish(msg_traite);
	}

	if (position==2)
	{
		PlatformColor.Color = nouveau_produit; // On remplie le message PlatformColor avec le n° du produit
		PlatformColor.Handle = listeHandle[0];	// On remplie le message PlatformColor avec le n° de handle de la navette
		cout << HandleSh_msg.data << endl;
		pubChangeColor.publish(PlatformColor);	// On publie à la commande locale le message

		ROS_INFO("Un produit est pose sur la navette");

		NewProduct.handle = listeHandle[0];	// On remplie le message Newproduct avec le n° de handle de la navette
		NewProduct.argument = nouveau_produit;  // On remplie le message Newproduct avec le n° de produit
		pubProductChange.publish(NewProduct);	// Renseigne le nouveau produit au noeud Shuttle
		tache=0;
		tachedone_msg.data=tache;
		pub_updateTache.publish(tachedone_msg);
	}
}


//Fonction Callback si un produit est récupéré par le robot
void Poste3::ProductTakenByRobotCallback(const std_msgs::Int32::ConstPtr& msg){

	cout << "position robot 2 : " << position << endl;
	
	if (position==2)
	{
	std_msgs::Int32 num_handle;
	int nouvelle_destination;
	cout << "DECOLORATION" <<endl;
	prod_recup = 1; // Pour dire qu'un produit est en cours de traitement
	//num_handle.data = handlesShuttles.front();	

	ROS_INFO("Le produit est recupere - Navette vide");

	nouveau_produit = 0;	// On attribue par défaut la valeur 0 au produit pour dire qu'on a une navette vide  
	nouvelle_destination = 0;	// On attribue par défaut la valeur 0 à la destination d'une navette vide

	// Pour changer la couleur de la plateforme
	PlatformColor.Color = nouveau_produit;
	PlatformColor.Handle = listeHandle[0];
	pubChangeColor.publish(PlatformColor);

	ShuttleInfo.request.handle = listeHandle[0];
	clientGetShuttleStatus.call(ShuttleInfo);
	msg_traite.data=ShuttleInfo.response.product;
	}

	if (position==1)
	{
	cout << "lancement" <<endl;
	ProductFromPost.publish(Prise_msg);
	}
}


// Fonction Callback pour savoir quand la tâche est finie
void Poste3::TaskFinishedCallback(const std_msgs::Int32::ConstPtr& msg){

		nouveau_produit = msg->data;
		ROS_INFO("Tache terminee");
		tache = 1;
}


// Fonction Init pour créer un produit
void Poste3::initProduct(int nDestination, int pNumber)
{

	ROS_INFO("Creation Produit, next destination = %d, numero produit = %d ", nDestination,pNumber);

		// Création dynamique de l'object product
	ProductPost* newProductPost = new ProductPost(nDestination,pNumber);

		// Insertion dans le map de la classe de la paire <key=pNumber,T=Product*>

	std::pair<std::map<int,ProductPost*>::iterator,bool> ret;			// ret permet d'avoir un retour de la fonction insert, il est faux si la key existe dèjà dans la map
	ret = ProductsMap.insert(std::pair<int,ProductPost*>(newProductPost->productNumber,newProductPost));

	if (ret.second==false)	// Si un produit avec le même nom existe dèjà, celui-ci n'est pas ajouté à la collection
	{
    		ROS_WARN("Ordonnanceur : Un Produit de ce nom existe dèjà !"); 
  	} 

}

	
