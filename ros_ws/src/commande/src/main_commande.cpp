#include "capteurs.h"
#include "actionneurs.h"
#include "commande.h"
#include "robots.h"
#include "AigsInterface.h"
#include <ros/ros.h>
#include <unistd.h>

using namespace std;

#define NOMBRE_PLACE 100
#define RESET   "\033[0m"
#define BOLDRED     "\033[1m\033[31m"      /* Bold Red */
#define BOLDGREEN   "\033[1m\033[32m"      /* Bold Green */
#define Nb_Place 1000

int main(int argc, char **argv)
{
	ros::init(argc, argv, "commande");
	ros::NodeHandle noeud;


	Commande cmd(noeud,argv[0]);
	Robots robot(noeud);
	AigsInterface aiguillage(noeud);
	Capteurs capteur(noeud);

	sleep(2);

	ros::Rate loop_rate(25); //fréquence de la boucle

	cmd.Initialisation();
  	int code_arrivee;//dépend du produit et du poste sur lequel il apparait
	int M[Nb_Place];
	int Nb_Place_T1,Nb_Place_T2,Nb_Place_T3,Nb_Place_T4;
	Nb_Place_T1=100;

	for(int i=0;i<Nb_Place;i++) M[i]=0;

	M[50]=1;//Robot 1 libre
	M[250]=1;//Robot 2 libre

	bool modif=1;

	while (ros::ok())
	{
		if(cmd.get_arrivee_nouveau_produit())
		{
			modif=1;
			code_arrivee=cmd.get_code_arrivee();
			switch(code_arrivee)
			{
				case 15: //Produit A sur poste 1
					M[15]++;break;
				case 16: //Produit A sur poste 2
					M[16]++;break;
				case 17: //Produit A sur poste 3
					M[17]++;break;
				case 18: //Produit A sur poste 4
					M[18]++;break;

				case 25: //Produit B sur poste 1
					M[26]++;break;
				case 27: //Produit B sur poste 2
					M[27]++;break;
				case 28: //Produit B sur poste 3
					M[28]++;break;
				case 29: //Produit B sur poste 4
					M[29]++;break;

				case 35: //Produit C sur poste 1
					M[35]++;break;
				case 36: //Produit C sur poste 2
					M[36]++;break;
				case 37: //Produit C sur poste 3
					M[37]++;break;
				case 38: //Produit C sur poste 4
					M[38]++;break;

				case 45: //Produit D sur poste 1
					M[45]++;break;
				case 46: //Produit D sur poste 2
					M[46]++;break;
				case 47: //Produit D sur poste 3
					M[47]++;break;
				case 48: //Produit D sur poste 4
					M[48]++;break;

				case 55: //Produit E sur poste 1
					M[55]++;break;
				case 56: //Produit E sur poste 2
					M[56]++;break;
				case 57: //Produit E sur poste 3
					M[57]++;break;
				case 58: //Produit E sur poste 4
					M[58]++;break;

				case 65: //Produit F sur poste 1
					M[65]++;break;
				case 66: //Produit F sur poste 2
					M[66]++;break;
				case 67: //Produit F sur poste 3
					M[67]++;break;
				case 68: //Produit F sur poste 4
					M[68]++;break;
			}
			cmd.renitialiser_arrivee_nouveau_produit();
	  	}

////////////////////////////////////////////////////////////////////////////////
//////////////////////////DEBUT PETRI///////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

		if (M[15]!=0 && capteur.get_PS(20)==1) // faire arreter la navette devant le robot 1
		{
			modif=1;
			M[15]--;
			cmd.Stop_PS(21);
			robot.DoTask(1,4,1);
			robot.AjouterProduit(3,6);
			M[1]++;
		}

		if (M[1]!=0 && robot.TaskPos4Etat(1)==1)
		{
			modif=1;
			M[1]--;
			robot.DeplacerPiece(1,4,1);
			M[2]++;
		}

		if (M[2]!=0 && capteur.get_PS(21)==1)
		{
			modif=1;
			M[2]--;
			robot.DoTask(1,1,2);
			M[3]++;
		}
		if (M[3]!=0 && robot.TaskPos1Etat(1)==1)
		{
			modif=1;
			M[3]--;
			robot.DeplacerPiece(1,1,2);
			cmd.Ouvrir_PS(21);
			cmd.Stop_PS(2);
			M[4]++;
		}

		if (M[4]!=0 && capteur.get_PS(2)==1) //orientation des aiguillages pour la boucle principale
		{
			M[4]--;
			modif=1;
			//aiguillage.Gauche(1);
			//aiguillage.Gauche(2);
			//aiguillage.Gauche(3);
			//aiguillage.Gauche(10);
			robot.DeplacerPiece(2,2,1);
			robot.DoTask(2,1,1);
			M[5]++;
		}

		if (M[5]!=0 && robot.TaskPos1Etat(2)==1)
		{
			M[5]--;
			modif=1;
			robot.Evacuer();
			cmd.Ouvrir_PS(2);
		}

///////////////////////////////////////////////////////////////////////////
////////////////////AFFICHAGE//////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////

		if(modif)
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

		ros::spinOnce(); //permet aux fonction callback de ros dans les objets d'êtres appelées
		loop_rate.sleep(); //permet de synchroniser la boucle while. Il attend le temps qu'il reste pour faire le 25Hz (ou la fréquence indiquée dans le loop_rate)
	}

	return 0;
}
