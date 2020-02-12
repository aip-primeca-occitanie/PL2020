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
				case 11: //Produit A sur poste 1
					M[11]++;break;
				case 12: //Produit A sur poste 2
					M[12]++;break;
				case 13: //Produit A sur poste 3
					M[13]++;break;
				case 14: //Produit A sur poste 4
					M[14]++;break;

				case 21: //Produit B sur poste 1
					M[21]++;break;
				case 22: //Produit B sur poste 2
					M[22]++;break;
				case 23: //Produit B sur poste 3
					M[23]++;break;
				case 24: //Produit B sur poste 4
					M[24]++;break;

				case 31: //Produit C sur poste 1
					M[31]++;break;
				case 32: //Produit C sur poste 2
					M[32]++;break;
				case 33: //Produit C sur poste 3
					M[33]++;break;
				case 34: //Produit C sur poste 4
					M[34]++;break;

				case 41: //Produit D sur poste 1
					M[41]++;break;
				case 42: //Produit D sur poste 2
					M[42]++;break;
				case 43: //Produit D sur poste 3
					M[43]++;break;
				case 44: //Produit D sur poste 4
					M[44]++;break;

				case 51: //Produit E sur poste 1
					M[51]++;break;
				case 52: //Produit E sur poste 2
					M[52]++;break;
				case 53: //Produit E sur poste 3
					M[53]++;break;
				case 54: //Produit E sur poste 4
					M[54]++;break;

				case 61: //Produit F sur poste 1
					M[61]++;break;
				case 62: //Produit F sur poste 2
					M[62]++;break;
				case 63: //Produit F sur poste 3
					M[63]++;break;
				case 64: //Produit F sur poste 4
					M[64]++;break;
			}
			cmd.renitialiser_arrivee_nouveau_produit();
	  	}

////////////////////////////////////////////////////////////////////////////////
//////////////////////////DEBUT PETRI///////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

		if (M[11]!=0 && capteur.get_PS(20)==1) // faire arreter la navette devant le robot 1
		{
			modif=1;
			M[11]--;
			cmd.Stop_PS(21);
			M[4]++;
		}

		if (M[4]!=0 && capteur.get_PS(21)==1) //Robot 1 fait la manip sur la navette
		{
			modif=1;
			M[4]--;
			robot.DeplacerPiece(1,2,4);
			cmd.Ouvrir_PS(21);
			M[5]++;
		}


		if (M[5]!=0 && capteur.get_PS(24)==1) //orientation des aiguillages pour la boucle principale
		{
			M[5]--;
			modif=1;
			aiguillage.Gauche(1);
			aiguillage.Gauche(2);
			aiguillage.Gauche(3);
			aiguillage.Gauche(10);
			robot.DoTask(1,4,0);
			M[6]++;
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
