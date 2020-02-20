/**** Maxime Maurin - Projet Long N7 2017 ****/

#include <ros/ros.h>
#include "configuration.h"
#include "commande_locale/Msg_AddProduct.h"
#include <unistd.h>

// Construteur
Configuration::Configuration(vrepController* VREPC)
{
	vrepCAcces = VREPC;
}

// Initialisation de l'objet
bool Configuration::init(ros::NodeHandle nh, std::string executionPath)
{
	pubProductAdd= nh.advertise<commande_locale::Msg_AddProduct>("/commande_locale/AddProduct",10);
}

void Configuration::ProductAddTable(int typeProduct, int poste)
{
	commande_locale::Msg_AddProduct msg0;
	msg0.num_poste = poste;
	msg0.num_produit = typeProduct*10+4;
	pubProductAdd.publish(msg0);
}
