/**** Maxime Maurin - Projet Long N7 2017 ****/

#include <ros/ros.h>
#include "scheduler.h"

// Construteur
Scheduler::Scheduler()
{
	// Initilisation de quelques valeurs par défaut
	numberOfProduct = 0;
	nextCount = 0;
	lastLaunchDate = 0;
	nbLoop = 1;
	deltaLoop = 2;
	finEnvoi = 0;
	productNumber = 1;
	tableDispo = true;
	init_var = true;
	compteur = 0;
	for(int i=0; i<100; i++) {
		nom_produits[i]= "NULL";
	}
}

// Initialisation de l'objet 
bool Scheduler::init(ros::NodeHandle nh, std::string executionPath)
{
	// Services Initialisation
	client_simRosGetInfo = nh.serviceClient<vrep_common::simRosGetInfo>("/vrep/simRosGetInfo");
	client_GetShuttleStatus = nh.serviceClient<shuttles::srvGetShuttleStatus>("/commande_navette/srvGetShuttleStatus");

	// Subscribers Initialisation
	subEndOfProductManufacture = nh.subscribe("/A3/ShuttleOut", 10 , &Scheduler::productOutCallBack,this);
	subProduitsPris = nh.subscribe("/Table2/ProduitPris", 10 , &Scheduler::tableUpdateCallback,this);
	sub_DemandeNbrSh = nh.subscribe("/A10/DemandeNbrSh", 10 , &Scheduler::SendNbrShuttleCallback,this);

	// Publishers Initialisation
	pubCreateShuttle = nh.advertise<scheduling::Msg_LoadShuttle>("/scheduling/NextProduct",10);
	pubDelShuttle = nh.advertise<std_msgs::Int32>("/commande_navette/DelShuttle",10);
	pubProductAddTable = nh.advertise<std_msgs::Int32>("/Table2/ManualProduct",10);
	pubNbShuttle = nh.advertise<std_msgs::Int32>("/A10/NombreShuttle",10);

	pubNombreDeProduits = nh.advertise<std_msgs::Int32>("/ordonnancement/NombreDeProduits",10);
	pubNomProduits = nh.advertise<std_msgs::String>("/ordonnancement/NomProduits",100);

	// Récupération du chemin vers le Working_Folder, permet de travailler en chemin relatif
	int count = 0 ;
	int pos = 0 ;
	while (count < 4) // le chemin sous linux est normalement standard d'où la présence de la constante 4, il est possible qu'il faille faire évoluer ce point si ce n'est plus le cas
		{
		if(executionPath[pos] == '/') count++;  // on cherche dans la chaine le 4ème '/' qui permet de récupérer le chemin absolu du Working Folder
		pos++;					// Cela est normalement garanti par le faite que le setup install toujours dans le dossier racine de l'utilisateur
		}
	
	std::string Working_Folder = executionPath.substr(0,pos);
	ROS_INFO ("$%s$", Working_Folder.c_str()) ;

	
	//Initialisation des produits à l'aide du fichier de configuration

	//Définition du chemin du fichier de config et log
	configFile = Working_Folder + "/ProductConfiguration.config";
	logFile = Working_Folder + "/Statistic.txt";

	std::ifstream streamConfigFile(configFile.c_str(), std::ios::in); 	// Ouverture en lecture seule du fichier de configuration
	
	// Pour comprendre la suite de la lecture du fichier, il peut etre utile de se référer à http://www.cplusplus.com/reference/iolibrary/
	
	if (streamConfigFile)	//Si l'ouverture à reussi
	{
		std::string pNameFF,destinationPart,launchDatePart,jobTimePart,nbLoopPos,contents,deltaLoopPos,nbNavettesPos; // string servant à l'extraction d'information

		//saut des lignes d'entêtes, repèrage du start (on passe toutes les lignes tant que le mot Start n'y figure pas)
	
		while(1)
		{
			std::getline(streamConfigFile,contents);
			std::size_t found = contents.find("Start"); 
  			if (found!=std::string::npos)
			{
				break;
			}
		}

		//Configuration nombre de navettes
	
		std::getline(streamConfigFile,contents);
		std::size_t pos11 = contents.find(":");   // repérage caractère ':' 
		nbNavettesPos = contents.substr(pos11+1); // on récupère le string qui se trouve après ':'
		nbNavettes = atoi( nbNavettesPos.c_str()); // atoi = conversion vers integer !
		
		msgShuttle.data = nbNavettes;
		pubNbShuttle.publish(msgShuttle);
		ROS_INFO("nbNavettes = %d",nbNavettes);	
	
		//Configuration nombre de boucle
	
		std::getline(streamConfigFile,contents);
		std::size_t pos0 = contents.find(":");   // repérage caractère ':' 
		nbLoopPos = contents.substr(pos0+1); // on récupère le string qui se trouve après ':'
		nbLoop = atoi( nbLoopPos.c_str()); // atoi = conversion vers integer !
		ROS_INFO("nbLoop = %d",nbLoop);

		//Configuration temps entre les boucles
	
		std::getline(streamConfigFile,contents);
		std::size_t pos10 = contents.find(":");   // repérage caractère ':' 
		deltaLoopPos = contents.substr(pos10+1); // on récupère le string qui se trouve après ':'
		deltaLoop = atoi( deltaLoopPos.c_str()); // atoi = conversion vers integer !
		ROS_INFO("deltaLoop = %d",deltaLoop);

		//Configuration temps entre lancement
	
		std::getline(streamConfigFile,contents);
		std::size_t pos1 = contents.find(":"); // repérage caractère ':' 
		launchDatePart = contents.substr(pos1+1); // on récupère le string qui se trouve après ':'
	
		char * cstr1 = new char [launchDatePart.length()+1]; // 
  		std::strcpy (cstr1, launchDatePart.c_str());	// création objet cstring

  		// cstr now contains a c-string copy of str
		int n1 = 0; //compteur sur les dates de lancement 
 	 	char * p1 = std::strtok (cstr1," "); // cf strtok sur cplusplus.com, permet un découpage du cstring fonction de l'espace
  		while (p1!=0)
  		{
			scheduledLaunchDate[n1++] = atoi(p1);		
			ROS_INFO ("launch time entre %d et %d = %d",n1,n1+1,scheduledLaunchDate[n1-1]);
  			p1 = std::strtok(NULL," ");
  		}
  		delete[] cstr1; // comme la création est dynamique, on supprime l'objet pour libèrer la mémoire
	
		//Configuration Produits

		int incrementation = 0;
		while (std::getline(streamConfigFile, contents))
		{
			if (contents.find(':') != std::string::npos )   // Per ne prendre en considération que les lignes renseignés, ( dans les faits qui contiennent au moins un ':')
			{
			std::size_t pos2 = contents.find(":"); // même idée que précedemment 
			std::size_t pos3 = contents.find_last_of(":"); // même idée que précedemment 
		
			pNameFF = contents.substr(0,pos2);	// Recupération nom produit
			ROS_INFO("Product %s",pNameFF.c_str());
			destinationPart = contents.substr(pos2+1,pos3-pos2-1);  //recupération du string correspondant aux destinations sur la ligne produit
			jobTimePart = contents.substr(pos3+1); // récupération du string correspondant aux temps des taches sur la ligne produit
			int destination[10]; // initialisation des tableaux locaux destination et Jobtime, il serviront a crée l'objet Produit
			int jobTime[10];
			int manRSize = 0; //manufacturing range size of the produit = number of operation

			char * cstr2 = new char [destinationPart.length()+1]; 
	  		std::strcpy (cstr2, destinationPart.c_str());	// création objet cstring
			char * cstr3 = new char [jobTimePart.length()+1]; 
	  		std::strcpy (cstr3, jobTimePart.c_str()); 	

	  		// cstr now contains a c-string copy of str
			int n2 = 0; //compteur sur les destinations
			int n3 = 0; //compteur sur les temps de fabrications
	 	 	
			// même principe que plus haut
			char * p2 = std::strtok (cstr2," ");
	  		while (p2!=NULL)
	  		{
	    			destination[n2++] = atoi(p2);
				manRSize++ ;
				ROS_INFO ("destination %d = %d",n2-1,destination[n2-1]);
	  			p2 = std::strtok(NULL," ");
			}

			char * p3 = std::strtok (cstr3," ");
			while (p3!=NULL)
	  		{
				jobTime[n3++] = atoi(p3);
				ROS_INFO ("jobTime %d = %d",n3-1,jobTime[n3-1]);
	  			p3 = std::strtok(NULL," ");
	  		}

	  		delete[] cstr2;
			delete[] cstr3; 


			char charName;
			charName = char(pNameFF.c_str()[0]-16);
			int pColor = atoi(&charName) * 10 ; 
			numberOfProduct++; // on compte le nombre de produit

			initProduct(pNameFF,destination[0], pColor, manRSize, productNumber);  // initialisation de l'objet produit
			nom_produits[incrementation]=pNameFF.c_str();
			incrementation++;
			}
		}
	}
	
	// Fin config produit 
	
	ROS_INFO("Number of Product = %d", numberOfProduct);
	iteratorPMap = ProductsMap.begin(); // initilise l'iterateur sur la collection de produit
	nextCount = numberOfProduct-1; // Permet de bien placer les delays dans launch next schedule, on va effectuer le délays entre le dernier et le premier produit avant la lancement du premier produit
	client_simRosGetInfo.call(srv_GetInfoVREP); //Recupération des infos sur la simulation Vrep ( temps de simulation notamment)
	lastLaunchDate = srv_GetInfoVREP.response.simulationTime; //Initialise le temps de simulation
	
	streamConfigFile.close(); //fermeture du fichier ProductConfiguration.txt ouvert en lecture//
}

// Destructor
Scheduler::~Scheduler()
{
}


// Scheduling Function
void Scheduler::launchNextSchedule(){

	if (init_var)
	{
		std_msgs::Int32 NbMsg;
		NbMsg.data = numberOfProduct;
		pubNombreDeProduits.publish(NbMsg);
		std_msgs::String NomMsg;
		for (int j=0; j<100; j++)
		{
			if (nom_produits[j] != "NULL")
			{
				NomMsg.data = nom_produits[j];
				pubNomProduits.publish(NomMsg);
			}
		}
		init_var = false;
	}

	int nextDelay = scheduledLaunchDate[nextCount]; // définition next delays 

	// Récupération temps VREP ////////////// SERVICE simRosGetInfo de VREP ////////////

	client_simRosGetInfo.call(srv_GetInfoVREP);
	float timeDelta = srv_GetInfoVREP.response.simulationTime - lastLaunchDate;
		
	///////////////////////////////////////////////////////////////////////////////////	

	if (finEnvoi!=nbLoop)
	{
		if (compteur == numberOfProduct) // permet de replacer l'iterateur en début de map si celui-ci se trouve en end ( après le dernier élément)
		{
			if (timeDelta > deltaLoop)
			{
				iteratorPMap = ProductsMap.begin();
				finEnvoi++;
				compteur=0;
				lastLaunchDate = srv_GetInfoVREP.response.simulationTime;// récupération temps de simulation
				client_simRosGetInfo.call(srv_GetInfoVREP);
				timeDelta = srv_GetInfoVREP.response.simulationTime - lastLaunchDate;
				ROS_INFO("Temps attente Loop ok");
			}
			if (finEnvoi==nbLoop && compteur == numberOfProduct)
			{
				ROS_INFO("Fin de l'envoi des produits");
			}
		}

		if (timeDelta > nextDelay && finEnvoi!=nbLoop && tableDispo==true && compteur < numberOfProduct)
		{
			compteur++;
			lastLaunchDate = srv_GetInfoVREP.response.simulationTime;// récupération temps de simulation
			// Récupération des information sur le produit
			Product* productPointer;
			productPointer = iteratorPMap->second;	
		
			//Creation du message de lancement de produit destiné à la commande_locale
			std_msgs::Int32 msgO;
			msgO.data = productPointer->productColor;
			pubProductAddTable.publish(msgO);

			ROS_INFO("ORDO Add product %s color = %d number = %d",productPointer->name.c_str(), productPointer->productColor, productPointer->productNumber);
			iteratorPMap++;		// avance dans la map produit
			nextCount++; 		// permet d'être en phase vis à vis du tableau des temps entre les lancements des produits
			nextCount = nextCount%numberOfProduct; //permet de revenir à la premier case quand on a fini le tableau 
			nextDelay = scheduledLaunchDate[nextCount]; // juste pour l'affichage...
			ROS_INFO("Je vais attendre = %d s VREP avant le lancement du prochain produit",nextDelay);
			tableDispo=false;
		}
	}
}

// Création de produit
void Scheduler::initProduct(std::string pName, int pFirstDestination, int initProductColor, int pManRSize, int order)
{
	ROS_INFO("Creation Produit %s, first destination = %d, ColorNumber = %d , taille gamme = %d , ProductNumber = %d", pName.c_str(), pFirstDestination,initProductColor, pManRSize, productNumber);

		// Création dynamique de l'object product
	Product* newProduct = new Product(pName,pFirstDestination,initProductColor, pManRSize ,productNumber);
	productNumber++;
		// Insertion dans le map de la classe de la paire <key=handle,T=Product*>

	std::pair<std::map<int,Product*>::iterator,bool> ret;			// ret permet d'avoir un retour de la fonction insert, il est faux si la key existe dèjà dans la map
	ret = ProductsMap.insert(std::pair<int,Product*>(order,newProduct));
	//ROS_INFO("Product pointer : %p ", newProduct);

}

// Subscribers Callback (Product end of manifacture)

void Scheduler::productOutCallBack(const std_msgs::Int32::ConstPtr& msg) // on recoit le handle de la navette qui sort de la cellule
{
		srv_GetShuttleStatus.request.handle = msg->data;	// 
		client_GetShuttleStatus.call(srv_GetShuttleStatus);	// recupération des infos sur la navette ( service du noeud shuttles)
		client_simRosGetInfo.call(srv_GetInfoVREP);		// récupération info sur la simulation Vrep

		std::string finalProductName;	// nom produit fini
		Product* productPointer;	// pointer pour recherche dans la collection
			
	for (iteratorPMapOut=ProductsMap.begin(); iteratorPMapOut!=ProductsMap.end(); ++iteratorPMapOut)	// On parcours l'ensemble de la collection
	{
		productPointer = iteratorPMapOut->second;
		if ( productPointer->productNumber == (srv_GetShuttleStatus.response.product - productPointer->manRangeSize)) // Trouve le nom du produit fini
		{
			finalProductName = productPointer->name;
		}
	}

}

void Scheduler::tableUpdateCallback(const std_msgs::Bool::ConstPtr& msg)
{
	tableDispo=msg->data;
}

void Scheduler::SendNbrShuttleCallback(const std_msgs::Int32::ConstPtr& msg)
{
	pubNbShuttle.publish(msgShuttle);
}
	
