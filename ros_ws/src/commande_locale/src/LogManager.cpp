#include <ros/ros.h>
#include <ros/package.h>
#include <std_msgs/Int32MultiArray.h>
#include <iostream>
#include <fstream>

using namespace std;

void ProduitEvac(std_msgs::Int32MultiArray msg)
{
    ofstream monFlux("/home/etudiant/log.txt",  ios::app);  //On essaye d'ouvrir le fichier
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


int main(int argc, char **argv)
{
    ofstream monFlux("/home/etudiant/log.txt");

	ros::init(argc, argv, "log_manager");
	ros::NodeHandle nh;

    //ros::Subscriber subTacheFinie;
    ros::Subscriber subProduitEvac;
    //ros::Subscriber subErreurTraitement;

    //subTacheFinie = nh.subscribe("", 1, &Tachefinie);
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
