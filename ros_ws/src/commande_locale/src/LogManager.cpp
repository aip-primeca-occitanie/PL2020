#include <ros/ros.h>
#include <ros/package.h>
#include <std_msgs/Int32MultiArray.h>
#include "commande_locale/Msg_AddProduct.h"
#include <iostream>
#include <fstream>

using namespace std;
ofstream monFlux("/home/etudiant/log.txt",  ios::app);  //On essaye d'ouvrir le fichier
void ProduitEvac(std_msgs::Int32MultiArray msg)
{

    int couleur[4];
    if(monFlux)  //On teste si tout est OK
    {
        monFlux<<"Sortie";
        for (int i=0;i<4;i++)
        {
                couleur[i]=msg.data[i];
                monFlux<<" :" <<couleur[i];
        }
        monFlux<<endl;
    }
    else
    {
        cout << "ERREUR: Impossible d'ouvrir le fichier log." << endl;
    }

    //a supprimer plus tard
    ROS_INFO("Evacuation TionTion");
    for (int i=0;i<4;i++)
    {
            couleur[i]=msg.data[i];
            ROS_INFO("Etage %d : %d",i,couleur[i]);
    }
}
void NewProduct(commande_locale::Msg_AddProduct msg)
{
    if(monFlux)  //On teste si tout est OK
    {
        monFlux<<"NewProduct: ";
        monFlux<<msg.num_produit;
        monFlux<<endl;
    }
    else
    {
        cout << "ERREUR: Impossible d'ouvrir le fichier log." << endl;
    }

    //a supprimer plus tard
    ROS_INFO("Add Product tucttuct");
    ROS_INFO("NewProduct: %d",msg.num_produit);

}


int main(int argc, char **argv)
{
    ofstream monFlux2("/home/etudiant/log.txt");

    monFlux2.close();

	ros::init(argc, argv, "log_manager");
	ros::NodeHandle nh;

    //ros::Subscriber subTacheFinie;
    ros::Subscriber subProduitEvac;
    ros::Subscriber subNewProduit;
    //ros::Subscriber subErreurTraitement;

    //subTacheFinie = nh.subscribe("", 1, &Tachefinie);
    subNewProduit = nh.subscribe("/commande_locale/AddProduct", 1, &NewProduct);
    subProduitEvac = nh.subscribe("/commande/Simulation/produitEvac", 1, &ProduitEvac);
    //subErreurTraitement = nh.subscribe("", 1, &ErreurTraitement);

    sleep(2);

	ros::Rate loop_rate(25); //fréquence de la boucle

	while (ros::ok())
	{
		ros::spinOnce(); //permet aux fonction callback de ros dans les objets d'êtres appelées
		loop_rate.sleep(); //permet de synchroniser la boucle while. Il attend le temps qu'il reste pour faire le 25Hz (ou la fréquence indiquée dans le loop_rate)
	}

	return 0;
}
