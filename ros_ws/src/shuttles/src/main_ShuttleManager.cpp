#include <ros/ros.h>
#include <unistd.h>
#include <queue>
#include "FileAttente.h"
#include "capteurs.h"

#include <iostream>
#include "commande/srv_recup_capteur_info.h"

using namespace std;

int main(int argc, char **argv)
{
	ros::init(argc, argv, "Shuttle_manager");
	ros::NodeHandle noeud;

	Capteurs capteur(noeud);
	queue<char> queue_vide;
	ros::Rate loop_rate(25); //fréquence de la boucle

	vector<FileAttente*> liste_file;

	//liste_file.push_back(FileAttente FileAttentePS(id_aiguillage,successeur_droite,successeur_gauche,queue));
	liste_file.push_back(new FileAttente(1,1+24,2+24,queue_vide));//filePS1
	liste_file.push_back(new FileAttente(0,3,3,queue_vide));//PS2
	liste_file.push_back(new FileAttente(0,5,5,queue_vide));
	liste_file.push_back(new FileAttente(2,0,6,queue_vide));
	liste_file.push_back(new FileAttente(2,6,0,queue_vide));
	liste_file.push_back(new FileAttente(3,20,3+24,queue_vide)); // Deux aiguillages ??
	liste_file.push_back(new FileAttente(4,8,0,queue_vide));
	liste_file.push_back(new FileAttente(5,4+24,5+24,queue_vide));
	liste_file.push_back(new FileAttente(0,10,10,queue_vide));
	liste_file.push_back(new FileAttente(0,12,12,queue_vide));
	liste_file.push_back(new FileAttente(6,0,13,queue_vide));
	liste_file.push_back(new FileAttente(6,13,0,queue_vide));
	liste_file.push_back(new FileAttente(7,6+24,7+24,queue_vide));
	liste_file.push_back(new FileAttente(0,15,15,queue_vide));
	liste_file.push_back(new FileAttente(0,17,17,queue_vide));
	liste_file.push_back(new FileAttente(8,0,18,queue_vide));
	liste_file.push_back(new FileAttente(8,18,0,queue_vide));
	liste_file.push_back(new FileAttente(9,8+24,8,queue_vide));
	liste_file.push_back(new FileAttente(10,20,0,queue_vide));
	liste_file.push_back(new FileAttente(11,9+24,10+24,queue_vide));
	liste_file.push_back(new FileAttente(0,22,22,queue_vide));
	liste_file.push_back(new FileAttente(0,24,24,queue_vide));
	liste_file.push_back(new FileAttente(12,0,1,queue_vide));
	liste_file.push_back(new FileAttente(12,1,0,queue_vide));

	liste_file.push_back(new FileAttente(0,2,2,queue_vide)); //CP1
	liste_file.push_back(new FileAttente(0,4,4,queue_vide)); //CP2
	liste_file.push_back(new FileAttente(0,7,7,queue_vide));
	liste_file.push_back(new FileAttente(0,9,9,queue_vide));
	liste_file.push_back(new FileAttente(0,11,11,queue_vide));
	liste_file.push_back(new FileAttente(0,14,14,queue_vide));
	liste_file.push_back(new FileAttente(0,16,16,queue_vide));
	liste_file.push_back(new FileAttente(0,19,19,queue_vide));
	liste_file.push_back(new FileAttente(0,21,21,queue_vide));
	liste_file.push_back(new FileAttente(0,23,23,queue_vide));

    //ros::ServiceClient client = noeud.serviceClient<commande::srv_recup_capteur_info>("srv_recup_capteur_info");
    //commande::srv_recup_capteur_info srv;
	while (ros::ok())
	{
		/*ROS_INFO("Je boucle wesh");
        srv.request.a = 42;
        if(client.call(srv))
        {
            ROS_INFO("J'ai une réponse ");
            ROS_INFO("c'est %d", srv.response.b);
        }
		else
		{
			ROS_ERROR("Fail to call ZE service");
		}*/



		/*if(modif)
		{

			cout<<"D0 : M[0]="<<M[0]<<endl<<endl;
			cout<<"Robot 1 libre : M[50]="<<M[50]<<endl<<endl;
			cout<<"Robot 2 libre : M[250]="<<M[250]<<endl<<endl;

			for (int i=1;i<=Nb_Place_T1;i++)
			{
				if(M[i]!=0)
				{
					cout<<BOLDRED<<"M["<<i<<"] = "<<M[i]<<RESET<<", ";
				}else
				{
					cout<<"M["<<i<<"] = "<<M[i]<<", ";
				}
			}
			cout<<endl<<endl;

			for (int i=1;i<=Nb_Place_T2;i++)
			{
				if(M[i+100]!=0)
				{
					cout<<BOLDRED<<"M["<<i+100<<"] = "<<M[i+100]<<RESET<<", ";
				}else
				{
					cout<<"M["<<i+100<<"] = "<<M[i+100]<<", ";
				}
			}
			cout<<endl<<endl;

			for (int i=1;i<=Nb_Place_T3;i++)
			{
				if(M[i+200]!=0)
				{
					cout<<BOLDRED<<"M["<<i+200<<"] = "<<M[i+200]<<RESET<<", ";
				}else
				{
					cout<<"M["<<i+200<<"] = "<<M[i+200]<<", ";
				}
			}
			cout<<endl<<endl;

			for (int i=1;i<=Nb_Place_T4;i++)
			{
				if(M[i+300]!=0)
				{
					cout<<BOLDRED<<"M["<<i+300<<"] = "<<M[i+300]<<RESET<<", ";
				}else
				{
					cout<<"M["<<i+300<<"] = "<<M[i+300]<<", ";
				}
			}
			cout<<endl<<endl;
		}
		modif=0;
        */
		ros::spinOnce(); //permet aux fonction callback de ros dans les objets d'êtres appelées
		loop_rate.sleep(); //permet de synchroniser la boucle while. Il attend le temps qu'il reste pour faire le 25Hz (ou la fréquence indiquée dans le loop_rate)
	}

	return 0;
}
