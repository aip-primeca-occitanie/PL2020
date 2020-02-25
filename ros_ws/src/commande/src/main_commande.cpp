#include "capteurs.h"
#include "actionneurs.h"
#include "commande.h"
#include "robots.h"
#include "AigsInterface.h"
#include <ros/ros.h>
#include <unistd.h>

using namespace std;

#define RESET   "\033[0m"
#define BOLDRED     "\033[1m\033[31m"      /* Bold Red */
#define BOLDGREEN   "\033[1m\033[32m"      /* Bold Green */
#define Nb_Place 1000
#define place_debut_display 1
#define Nb_Place_Display 200		

int M[Nb_Place];

// Pour l'affichage //
void display() 
{
	for (int i=place_debut_display;i<=place_debut_display+Nb_Place_Display;i++)
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
}

int main(int argc, char **argv)
{
	////////////////////////////////////////////////////
	///////////  | Debut du Petri plus bas |  //////////
	////////////////////////////////////////////////////


	ros::init(argc, argv, "commande");
	ros::NodeHandle noeud;

	ros::Publisher pub_spawnShuttles = noeud.advertise<std_msgs::Int32>("/commande_locale/nbNavettes",10);

	Commande cmd(noeud,argv[0]);
	Robots robot(noeud);
	AigsInterface aiguillage(noeud);
	Capteurs capteur(noeud);

	ros::Rate loop_rate(25); //fréquence de la boucle

	for(int i=0; i<25*2; i++) // Wait 2s
	{
		ros::spinOnce();
		loop_rate.sleep();
	}

	cmd.Initialisation();
	int code_arrivee;//dépend du produit et du poste sur lequel il apparait

	for(int i=0;i<Nb_Place;i++) M[i]=0;

	M[100]=1; // initialisation

	// Creation des Navettes
	int nbNavettes=2;//Mettre 0 pour demander a l'utilisateur
	while(nbNavettes<1||nbNavettes>6)
	{
		cout << "Combien voulez vous de navettes ? [1..6]" << endl;
		cin >> nbNavettes;
		if(cin.fail())
		{
			cout << endl << " [Erreur mauvais choix ..]" << endl;
			cin.clear();
			cin.ignore(256,'\n');
		}
	}

	std_msgs::Int32 msg_nbNavettes;
	msg_nbNavettes.data=nbNavettes;
	pub_spawnShuttles.publish(msg_nbNavettes);

	while (ros::ok())
	{
		if(cmd.get_arrivee_nouveau_produit())
		{
			display();
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

////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////// | DEBUT PETRI | ////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		// On veut simuler ça -> B : 1 3 : 13 15 : 1
		// comme défini dans le fichier .config
		// Signifie:
		//	- on fait apparaitre un produit B sur n'importe quel poste (pas n'importance)
		//	- Sur ce produit on veut effectuer succesivement les tâches 1 et 3 (donc aux postes 1 et 3) pendant 13s et 15s
		//	- une des navettes va nous servir à faire passer le produit d'un poste à l'autre
		//	- on crée 1 seul produit B avec cette enchainement de tâches
		//
		// -->	Pour que ce Petri fonctionne, on doit pouvoir savoir si le déplacement d'un robot est fini
		//	donc j'ai ajouté dans les if l'état "robot.FinDeplacement(num_Robot)" comme modèle

		if (M[100]==1) // apparaitre produit B sur poste 3
		{
			display();
			M[100]--;
			robot.AjouterProduit(POSTE_3,2); // ajout produit n°2 (donc B) sur poste 3
			robot.AjouterProduit(POSTE_7,6);
			cmd.Stop_PS(10);
			M[101]++;
		}

		if (M[101]==1 && capteur.get_PS(1)) // quand navette à proximité du poste 3, on le fait arrêter au niveau du poste
		{
			display();
			M[101]--;
			cmd.Stop_PS(2);
			robot.DoTask(POSTE_7,3);
			M[102]++;
		}

		if (M[102]==1 && capteur.get_PS(2) && robot.IsTaskOver(POSTE_7)) // le robot 2 prend le produit B sur le poste 3 et le met sur la navette
		{
			display();
			M[102]--;
			robot.DeplacerPiece(ROBOT_2,1,2);
			robot.DeplacerPiece(ROBOT_4,1,3);
			cmd.SortirErgot(1);
			M[103]++;
		}

		if (M[103]==1  && robot.FinDeplacerPiece(ROBOT_2) && robot.FinDeplacerPiece(ROBOT_4)) // la navette repars du poste 3 avec le produit B
		{
			display();
			M[103]--;
			cmd.Ouvrir_PS(2);
			cmd.Ouvrir_PS(10);
			aiguillage.Droite(2);
			cmd.Stop_PS(6);
			cmd.Stop_PS(14);
			M[104]++;
		}

		if (M[104]==1 && capteur.get_PS(6) && capteur.get_PS(14))// && robot.IsTaskOver(POSTE_8)) // On dirige la navette vers le poste 1
		{
			display();
			M[104]--;
			robot.DeplacerPiece(ROBOT_3,2,1);
			aiguillage.Gauche(3);
			aiguillage.Gauche(10); // on le met ici car pas de capteur de position entre aiguillage 3 et 10
			M[105]++;
		}

		if (M[105]==1 && capteur.get_DG(3) && capteur.get_DG(10) && robot.FinDeplacerPiece(ROBOT_3))// && robot.FinDeplacerPiece(ROBOT_4)) // quand les aiguillages ont fini de tourner on fait partir la navette
		{
			display();
			M[105]--;
			cmd.Ouvrir_PS(6);
			cmd.Stop_PS(22);
			robot.DoTask(POSTE_6,6);
			M[106]++;
		}

		if (M[106]==1 && capteur.get_PS(20)) // Attend front descendant de PS20
		{
			display();
			M[106]--;
			M[320]++;
		}

		if(M[320]==1 && !capteur.get_PS(20)) // Front descendant : la navette 1 est passée, on bouge les aiguillages pour la navette 0
		{
			display();
			M[320]--;
			aiguillage.Droite(10);
			aiguillage.Droite(3);
			M[107]++;
		}

		if (M[107]==1 && robot.IsTaskOver(POSTE_6)) // Une fois l'aiguillage 10 lock on laisse passer la navette 0
		{
			display();
			M[107]--;
			robot.DeplacerPiece(ROBOT_3,1,4);
			M[109]++;
		}

		if (M[109]==1 && capteur.get_PS(22) && robot.FinDeplacerPiece(ROBOT_3)) // le robot 1 prend le produit B sur la navette et le met sur le poste 1
		{
			display();
			M[109]--;
			robot.DeplacerPiece(ROBOT_1,3,4);
			robot.DoTask(POSTE_5,3);
			cmd.SortirErgot(8);
			M[110]++;
		}

		if (M[110]==1 && robot.FinDeplacerPiece(1)) // robot 1 fais tache 1 pendant 4s

		{
			display();
			M[110]--;
			robot.DoTask(POSTE_1,4);
			M[111]++;
		}

		if (M[111]==1 && robot.IsTaskOver(POSTE_1) && robot.IsTaskOver(POSTE_5)) // le robot 1 prend le produit B sur le poste et le met sur la navette quand tache fini
		{
			display();
			M[111]--;
			robot.DeplacerPiece(ROBOT_1,4,3);
			robot.DeplacerPiece(ROBOT_3,4,2);
			M[112]++;
		}

		if (M[112]==1 && robot.FinDeplacerPiece(ROBOT_1) && robot.FinDeplacerPiece(ROBOT_3)) // la navette repars du poste 1 avec le produit B qui a fait la tâche 1
		{
			display();
			M[112]--;
			cmd.Ouvrir_PS(22);
			cmd.Ouvrir_PS(14);
			M[115]++;
		}

		if (M[115]==1 && capteur.get_CP(1)) // quand la navette arrive à proximité du poste 4, on le fait arrêter au niveau du poste
		{
			display();
			M[115]--;
			cmd.Stop_PS(3);
			M[150]++;
			M[116]++;
		}

		if(M[150]==1 && capteur.get_PS(2))
		{
			display();
			M[150]--;
			M[151]++;
		}

		if(M[151]==1 && !capteur.get_PS(2))
		{
			display();
			M[151]--;
			cmd.Stop_PS(2);
		}

		if (M[116]==1 && capteur.get_PS(3)) // le robot 2 prend le produit B sur la navette et le met sur le poste 4
		{
			display();
			M[116]--;
			robot.DeplacerPiece(ROBOT_2,3,4);
			M[117]++;
		}

		if (M[117]==1 && robot.FinDeplacerPiece(ROBOT_2)) // On fait la tache du poste 4 pendant 5s
		{
			display();
			M[117]--;
			robot.DoTask(POSTE_4,5);
			M[118]++;
		}

		if(M[118]==1 && capteur.get_PS(2))
		{
			display();
			M[118]--;
			robot.DeplacerPiece(ROBOT_2,2,1);
			M[119]++;
		}

		if(M[119]==1 && robot.FinDeplacerPiece(ROBOT_2))
		{
			display();
			M[119]--;
			robot.Evacuer();
			M[120]++;
		}

		if (M[120]==1 && robot.IsTaskOver(POSTE_4)) // le robot 2 prend le reproduit B sur le poste et le met sur le poste 3
		{
			display();
			M[120]--;
			robot.DeplacerPiece(ROBOT_2,4,1);
			M[122]++;
		}

		if (M[122]==1 && robot.FinDeplacerPiece(ROBOT_2)) // On évacue le produit final et redémarre la navette
		{
			display();
			M[122]--;
			robot.Evacuer(); // Evacue le produit
			cmd.Ouvrir_PS(3); // la navette repart
			cmd.Ouvrir_PS(2);
			M[123]++;
		}

		ros::spinOnce(); //permet aux fonction callback de ros dans les objets d'êtres appelées
		loop_rate.sleep(); //permet de synchroniser la boucle while. Il attend le temps qu'il reste pour faire le 25Hz (ou la fréquence indiquée dans le loop_rate)
	}

	return 0;
}
