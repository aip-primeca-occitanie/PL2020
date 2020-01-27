/*
 * ************************************* *
 * 	  Projet Long N7 2017  	         *
 * ************************************* *
*///porkiemsky

// PARTIE A GARDER POUR LES MODIFICATION DU RDP //
//HELLO


#include "capteurs.h" 
#include "actionneurs.h" 
#include "commande.h"
#include "robots.h"
#include <ros/ros.h>
#include <unistd.h>
#include <shuttles/srvGetEmptyShuttles.h>


#include "aiguillages/A1.h"
#include "aiguillages/A2.h"
#include "aiguillages/A3.h"
#include "aiguillages/A10.h"
#include "aiguillages/A11.h"
#include "aiguillages/A12.h"




#include "Capteur_Poste.h"


using namespace std;

#define NOMBRE_PLACE 100
#define RESET   "\033[0m"
#define BOLDRED     "\033[1m\033[31m"      /* Bold Red */
#define BOLDGREEN   "\033[1m\033[32m"      /* Bold Green */
#define Nb_Place 1000

int main(int argc, char **argv)
{	

//initialisation du noeud ros et création d'un handle associé au noeud
	ros::init(argc, argv, "commande");	
	ros::NodeHandle noeud;

//création et initialisation des objets Capteur et Actionneurs

	Commande cmd(noeud,argv[0]);	
	
	Robots Robots(noeud);

	A1 A1(noeud);
	A2 A2(noeud);
	A3 A3(noeud);
	A10 A10(noeud);
	A11 A11(noeud);
	A12 A12(noeud);

	Capteur_Poste Capteur_Poste(noeud);

	//Poste1 Poste1(noeud,argv[0]);
	//Poste2 Poste2(noeud,argv[0]);
	//Poste3 Poste3(noeud,argv[0]);
	//Poste4 Poste4(noeud,argv[0]);

	ros::Rate loop_rate(25); //fréquence de la boucle 

	//Services shuttle
	//ros::ServiceClient client_GetEmptyShuttle;
	//shuttles::srvGetEmptyShuttles srv_GetEmptyShuttle;

	//client_GetEmptyShuttle = noeud.serviceClient<shuttles::srvGetEmptyShuttles>("/commande_navette/srvGetEmptyShuttles");

// Initialisation variables //

	cmd.Initialisation();

	int M[Nb_Place];
	int Nb_Place_T1,Nb_Place_T2,Nb_Place_T3,Nb_Place_T4;
	Nb_Place_T1 = 100;
	
	for(int i=0;i<Nb_Place;i++) M[i]=0;
	
	//Capteur_Poste.StopShuttle(1);
	//Capteur_Poste.StopShuttle(2);
	//Capteur_Poste.StopShuttle(3);
	//Capteur_Poste.StopShuttle(4);

	int done1=0;
	int done2=0;
	int done3=0;
	int done4=0;
	int done5=0;


	M[50]=1;//Robot 1 libre
	M[250]=1;//Robot 2 libre

	bool modif=1;

	M[1]++;//Jeton initialisation
	A1.STOP();
	A2.STOP_DROIT();
	A2.STOP_VIRAGE();
	A3.STOP();
	A10.STOP();
	A11.STOP();
	A12.STOP_DROIT();
	A12.STOP_VIRAGE();

	while (ros::ok())
	{
		if (Capteur_Poste.get_PS21()==1 && done1==0)
		{
		cout << "PS21: " << Capteur_Poste.get_PS21() << endl;
		done1=1;
		}		

		if (Capteur_Poste.get_PS22()==1 && done2==0)
		{
		cout << "PS22: " << Capteur_Poste.get_PS22() << endl;
		done2=1;
		}

		if (Capteur_Poste.get_PS2()==1 && done3==0)
		{
		cout << "PS2: " << Capteur_Poste.get_PS2() << endl;
		done3=1;
		}

		if (Capteur_Poste.get_PS3()==1 && done4==0)
		{
		cout << "PS3: " << Capteur_Poste.get_PS3() << endl;
		done4=1;
		}

		if (A3.get_CP3()==1 && done5==0)
		{
		cout << "CP3: " << A3.get_CP3() << endl;
		done5=1;
		}



////////////////////Là où les étudiants commencent////////////////////


		if(M[1]!=0)
		{
			//On mange les jetons
			M[1]--;
			//Actions
			modif=1;
			A10.Droit();
			A11.Virage();
			//On donne les jetons
			M[2]++;			
		}

		if(M[2]!=0 && A10.get_posAig_Droit()==1)
		{
			//On mange les jetons
			M[2]--;
			//Actions
			modif=1;
			A10.START();
			//On donne les jetons
			M[3]++;			
		}

		if(M[3]!=0 && A11.get_posAig_Virage()==1)
		{
			//On mange les jetons
			M[3]--;
			//Actions
			modif=1;
			A11.START();
			Capteur_Poste.StopShuttle(2);
			//On donne les jetons
			M[4]++;			
		}

		if(M[4]!=0 && Capteur_Poste.get_PS22()==1)
		{
			//On mange les jetons
			M[4]--;
			//Actions
			modif=1;
			Robots.EnvoyerPosition(1,4);
			//On donne les jetons
			M[5]++;			
		}

		if(M[5]!=0 && Robots.RobotEnPosition(1)==1)
		{
			M[5]--;			
			modif=1;
			cout << "prise de la piece sur le poste " << endl;
			cmd.PiecePrise(2);
			usleep(100000);
			Robots.EnvoyerPosition(1,3);
			M[6]++;
			
		}

		if(M[6]!=0 && Robots.RobotEnPosition(1)==1)
		{
			M[6]--;			
			modif=1;
			cmd.PieceDeposee(2);
			M[7]++;
			
		}

		if(M[7]!=0)
		{
			M[7]--;
			modif=1;
			Capteur_Poste.StartShuttle(2);
			M[8]++;
		}

		if(M[8]!=0)
		{
			M[8]--;
			modif=1;
			Capteur_Poste.StopShuttle(2);
			M[9]++;
			M[4]++;
		}

		if(M[9]!=0)
		{
			M[9]--;
			modif=1;
			A12.START_DROIT();
			A1.Droit();
			M[10]++;
		}

		if(M[10]!=0 && A1.get_posAig_Droit()==1)
		{
			M[10]--;
			modif=1;
			A1.START();
			M[11]++;
		}

		if(M[11]!=0)
		{
			M[11]--;
			modif=1;
			Capteur_Poste.StopShuttle(3);
			M[12]++;
		}

		if(M[12]!=0 && Capteur_Poste.get_PS2()==1)
		{
			//On mange les jetons
			M[12]--;
			//Actions
			modif=1;
			Robots.EnvoyerPosition(2,2);
			//On donne les jetons
			M[13]++;			
		}

		if(M[13]!=0 && Robots.RobotEnPosition(2)==1)
		{
			M[13]--;			
			modif=1;
			cmd.PiecePrise(3);
			usleep(100000);
			Robots.EnvoyerPosition(2,1);
			usleep(100000);
			cout << Robots.RobotEnPosition(2) << endl;
			M[14]++;
			
		}

		if(M[14]!=0 && Robots.RobotEnPosition(2)==1)
		{
			M[14]--;			
			modif=1;
			//usleep(100000);
			cmd.PieceDeposee(3);
			cmd.lancementTache(3,5);
			M[15]++;
			
		}

		if(M[15]!=0 && cmd.get_tache3done()==1)
		{
			M[15]--;
			modif=1;
			Robots.EnvoyerPosition(2,1);
			M[16]++;
		}

		if(M[16]!=0 && Robots.RobotEnPosition(2)==1)
		{
			M[16]--;			
			modif=1;
			cmd.PiecePrise(3);
			usleep(100000);
			Robots.EnvoyerPosition(2,2);
			M[17]++;	
		}

		if(M[17]!=0 && Robots.RobotEnPosition(2)==1)
		{
			M[17]--;			
			modif=1;
			cmd.PieceDeposee(3);
			M[18]++;	
		}

		if(M[18]!=0)
		{
			M[18]--;			
			modif=1;
			Capteur_Poste.StartShuttle(3);
			M[19]++;	
		}




////////////////////Là où les étudiants finnissent////////////////////////////////////////

		if(modif)
		{

			//cout<<"D0 : M[0]="<<M[0]<<endl<<endl;
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

		ros::spinOnce(); //permet aux fonction callback de ros dans les objets d'êtres appelées
		loop_rate.sleep(); //permet de synchroniser la boucle while. Il attend le temps qu'il reste pour faire le 25Hz (ou la fréquence indiquée dans le loop_rate)
	}

	return 0;


}
