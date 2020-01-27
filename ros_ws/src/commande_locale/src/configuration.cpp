/**** Maxime Maurin - Projet Long N7 2017 ****/

#include <ros/ros.h>
#include "configuration.h"

// Construteur
Configuration::Configuration(vrepController* VREPC)
{
	vrepCAcces = VREPC;
	numberOfProduct = 0;
	for ( int i = 1 ; i < 7 ; i++) activeProduct[i]=false;
	activeProduct[0] = true;
	NombreNavettes = 2;
	nbLoop = 1;
	deltaLoop = 10;
}

// Initialisation de l'objet 
bool Configuration::init(ros::NodeHandle nh, std::string executionPath)
{
	// Services Initialisation
	client_simRosGetInfo = nh.serviceClient<vrep_common::simRosGetInfo>("/vrep/simRosGetInfo");

	// Publishers Initialisation 
	pubManualProduct = nh.advertise<std_msgs::Bool>("/scheduling/ManualLaunch",10);
	pubProductAddTable = nh.advertise<std_msgs::Int32>("/Table2/ManualProduct",10);

// Récupération du chemin vers le Working_Folder
int count = 0 ;
int pos = 0 ;
while (count < 4)
	{
	if(executionPath[pos] == '/') count++;
	pos++;
	}
std::string Working_Folder = executionPath.substr(0,pos);
ROS_INFO ("$%s$", Working_Folder.c_str()) ;

	
//Initialisation des produits à l'aide du fichier de configuration

	//Définition du chemin du fichier de config et log
configFile = Working_Folder + "/ProductConfiguration.config";

std::ifstream streamConfigFile(configFile.c_str(), std::ios::in);
	
if (streamConfigFile)	//Si l'ouverture à reussi
{
	std::string pNameFF,destinationPart,launchDatePart,jobTimePart,nbLoopPos,contents,deltaLoopPos,nbNavettesPos;

	//saut des lignes d'entêtes, repèrage du start.
	while(1){
		std::getline(streamConfigFile,contents);
		std::size_t found = contents.find("Start");
  		if (found!=std::string::npos)
		{
		break;
		}
	}

	//Configuration nombre de navettes
	
	std::getline(streamConfigFile,contents);
	std::size_t pos0 = contents.find(":");   // repérage caractère ':' 
	nbNavettesPos = contents.substr(pos0+1); // on récupère le string qui se trouve après ':'
	NombreNavettes = atoi( nbNavettesPos.c_str()); // atoi = conversion vers integer !
	ROS_INFO("nbNavettes = %d",NombreNavettes);

	//Configuration nombre de boucle
	
	std::getline(streamConfigFile,contents);
	std::size_t pos1 = contents.find(":");   // repérage caractère ':' 
	nbLoopPos = contents.substr(pos1+1); // on récupère le string qui se trouve après ':'
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
	
	while (std::getline(streamConfigFile, contents))
		{

		//ROS_INFO("%s",contents.c_str())	;
		std::size_t pos2 = contents.find(":");
		std::size_t pos3 = contents.find_last_of(":");
		
		pNameFF = contents.substr(0,pos2);
		ROS_INFO("Product %s",pNameFF.c_str());
		destinationPart = contents.substr(pos2+1,pos3-pos2-1);
		
		int destination[10];
		int jobTime[10];
		int manRSize = 0; //manufacturing range size of the product = number of operation

		char * cstr2 = new char [destinationPart.length()+1]; 
  		std::strcpy (cstr2, destinationPart.c_str());	// création objet cstring

  		// cstr now contains a c-string copy of str
		int n2 = 0; //compteur sur les destinations
 	 	

		char * p2 = std::strtok (cstr2," ");
  		while (p2!=NULL)
  			{
    			//ROS_INFO ("p2 = %s",p2) ;
			destination[n2++] = atoi(p2);
			manRSize++ ;
			ROS_INFO ("destination %d = %d",n2-1,destination[n2-1]);
  			p2 = std::strtok(NULL," ");
			//ROS_INFO ("p2 = %s",p2) ;
  			}

  		delete[] cstr2;

		char charName;
		charName = char(pNameFF.c_str()[0]-16);
		
		int index = atoi(&charName) ;
		int pNumber = index * 10 ;
		activeProduct[index] = true;
		
		initProduct(pNameFF,destination[0], pNumber, manRSize);
		numberOfProduct++;
	}	

}
else ROS_ERROR("Impossible d'ouvrir le fichier ProductConfiguration.txt !");
return false;
}

// Destructor
Configuration::~Configuration()
{
}

//Faire apparaitre le produit sur la table 2 (est appelé dans UI)
void Configuration::ProductAddTable(int typeProduct){
	client_simRosGetInfo.call(srv_GetInfoVREP);
	std::string ProductName;
	Product* productPointer;
	bool ok = false;
	for (iteratorPMap=ProductsMap.begin(); iteratorPMap!=ProductsMap.end(); ++iteratorPMap)	// On parcours l'ensemble de la collection
	{
		productPointer = iteratorPMap->second;
		if ( productPointer->productNumber == typeProduct ) // Trouve le nom du produit fini
		{
			ProductName = productPointer->name;
			ok = true;
		}
	}
	iteratorPMap=ProductsMap.begin();
	if (ok)
	{
		std_msgs::Int32 msgO;
		msgO.data = typeProduct;
		pubProductAddTable.publish(msgO);
	}
}

// Création de produit
void Configuration::initProduct(std::string pName, int pFirstDestination, int initProductNumber, int pManRSize)
{
	ROS_INFO("Creation Produit %s, first destination = %d, numero produit = %d , taille gamme = %d", pName.c_str(), pFirstDestination,initProductNumber, pManRSize);

	// Création dynamique de l'object product
	Product* newProduct = new Product(pName,pFirstDestination,initProductNumber, pManRSize);

	// Insertion dans le map de la classe de la paire <key=handle,T=Product*>
	std::pair<std::map<std::string,Product*>::iterator,bool> ret;			// ret permet d'avoir un retour de la fonction insert, il est faux si la key existe dèjà dans la map
	ret = ProductsMap.insert(std::pair<std::string,Product*>(newProduct->name,newProduct));
	//ROS_INFO("Product pointer : %p ", newProduct);

} 


int Configuration::getNbNavettes()
{
	return NombreNavettes;
}
