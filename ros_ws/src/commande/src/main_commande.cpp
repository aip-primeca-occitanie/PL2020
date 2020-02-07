#include "capteurs.h"
#include "actionneurs.h"
#include "commande.h"
#include "robots.h"
#include "AigsInterface.h"
#include <ros/ros.h>
#include <unistd.h>
//#include <shuttles/srvGetEmptyShuttles.h>

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

	//Services shuttle
	//ros::ServiceClient client_GetEmptyShuttle;
	//shuttles::srvGetEmptyShuttles srv_GetEmptyShuttle;

	//client_GetEmptyShuttle = noeud.serviceClient<shuttles::srvGetEmptyShuttles>("/commande_navette/srvGetEmptyShuttles");

// Initialisation variables //

	cmd.Initialisation();
  int code_arrivee;
	int M[Nb_Place];
	int Nb_Place_T1,Nb_Place_T2,Nb_Place_T3,Nb_Place_T4;

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

////////////////////Poste 1//////////////////////////////

	if (M[11]!=0 && capteur.PSx[20]==1) // faire arreter la navette devant le robot 1
		{
			modif=1;
			M[11]--;
			cmd.Stop_PS(21);
			M[4]++;
		}

		if (M[4]!=0 && capteur.PSx[21]==1) //Robot 1 fait la manip sur la navette
			{
				modif=1;
				M[4]--;
				robot.DeplacerPiece(1,2,4);
				cmd.Ouvrir_PS(21);
				M[5]++;
			}


			if (M[5]!=0 && capteur.PSx[24]==1) //orientation des aiguillages pour la boucle principale
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

/*		//t1
		if(M[1]!=0 && cmd.NavetteStoppeeVide(1)!=0)
		{
			modif=1;
			M[2]=cmd.NavetteStoppeeVide(1);
			M[1]--;
		}
*/
		//t2
		/*if(M[3]!=0 && cmd.NavetteStoppee(1)!=0)
		{
			modif=1;
			M[4]=cmd.NavetteStoppee(1);
			M[3]--;
			cmd.NavettePartie(1);

		}*/

/*		//t3
		if(M[2]!=0 && M[50]==1 && cmd.ProduitSurNavette(M[2])==0)
		{
			modif=1;
			M[5]=M[2];
			M[2]=0;
			M[50]=0;
			Robots.ControlerRobot(1, 1, -1, 1); /// Pour comprendre comment cette fonction marche, la fonction ce trouve dans robots.cpp
		}

		//t4
		if(M[5]!=0 &&   Robots.PinceEnPosition(1)==1)
		{
			modif=1;
			M[6]=M[5];
			M[5]=0;
			Robots.MonterBras(1);
		}

		//t5
		if(M[6]!=0 && M[14]!=0 && Robots.BrasEnPosition(1)==1)
		{
			modif=1;
			M[7]=M[6];
			M[6]=0;
			M[14]=0;
			Robots.ControlerRobot(1, 2, -1, -1);
		}

		//t6
		if(M[7]!=0  &&   Robots.RobotEnPosition(1)==1&& Robots.BrasEnPosition(1)==-1 && Robots.PinceEnPosition(1)==-1)
		{
			modif=1;
			M[8]=M[7];
			M[7]=0;
			Robots.MonterBras(1);
		} */

		//t7
	/*	if(M[4]!=0 && M[50]==1 && cmd.ProduitSurNavette(M[4])!=0)
		{
			modif=1;
			M[9]=M[4];
			M[4]=0;
			M[50]=0;
			Robots.ControlerRobot(1, 2, -1, 1);
		}


		//t8
		if(M[9]!=0  &&  Robots.RobotEnPosition(1)==1 && Robots.BrasEnPosition(1)==-1 && Robots.PinceEnPosition(1)==1)
		{
			modif=1;
			M[10]=M[9];
			M[9]=0;
			Robots.MonterBras(1);
		}

		//t9
		if(M[10]!=0 && Robots.BrasEnPosition(1)==1)
		{
			modif=1;
			M[11]=M[10];
			M[10]=0;
			cmd.DestroyShuttle(M[11]);
			//cmd.NavettePartie(1);
			//cmd.PiecePrise(1);
			//M[0]++;
			//Robots.ControlerRobot(1, 1, -1, -1);
		}
*/
/*		//t10
		if(M[11]!=0  &&   Robots.RobotEnPosition(1)==1 && Robots.BrasEnPosition(1)==-1 && Robots.PinceEnPosition(1)==-1)
		{
			modif=1;
			M[13]=M[11];
			M[11]=0;
			M[12]=1;
			Robots.MonterBras(1);

		}

		//t11
		if(M[13]!=0 && M[0]!=0 && Robots.TraitementFini(1)==1)
		{
			modif=1;
			M[0]--;
			M[13]=0;
			M[14]=1;
			M[1]++;
			int handle;
			handle=cmd.NavetteDisponible();
			cmd.DefinirDestination(handle, 1);
		}

		//t12
		if(M[8]!=0 && Robots.BrasEnPosition(1)==1)
		{
			modif=1;

			M[15]=M[8];
			M[8]=0;
			M[50]=1;
			cmd.NavettePartie(1);
			cmd.PieceDeposee(1);
			usleep(500000);
		}

		//t13
		if(M[12]!=0 && Robots.BrasEnPosition(1)==1)
		{
			modif=1;

			M[12]=0;
			M[50]=1;
		}

		//t14
		if(M[15]!=0 && cmd.ProduitSurNavette(M[15])==2)//envoie vers P2
		{
			M[103]++;
			modif=1;
			cout<<"Envoi vers P2 : produit : "<<cmd.ProduitSurNavette(M[15])<<endl;
			cmd.DefinirDestination(M[15], 2);
			M[15]=0;
		}

		//t15
		if(M[15]!=0 && cmd.ProduitSurNavette(M[15])==3)//envoie vers P3
		{
			M[203]++;
			modif=1;
			cout<<"Envoi vers P3 : produit : "<<cmd.ProduitSurNavette(M[15])<<endl;
			cmd.DefinirDestination(M[15], 3);
			M[15]=0;
		}

		//t16
		if(M[15]!=0 && cmd.ProduitSurNavette(M[15])==4)//envoie vers P4
		{
			M[303]++;
			modif=1;
			cout<<"Envoi vers P4 : produit : "<<cmd.ProduitSurNavette(M[15])<<endl;
			cmd.DefinirDestination(M[15], 4);
			M[15]=0;
		}

		//t17
		if(M[15]!=0 && cmd.ProduitSurNavette(M[15])==5)//sortie du produit
		{
			//M[103]++;
			modif=1;
			cout<<"Sortie de la navette : produit : "<<cmd.ProduitSurNavette(M[15])<<endl;
			cmd.DefinirDestination(M[15], 5);
			M[15]=0;
		}

		*/
		Nb_Place_T1=15;
/*
/////////////////////////Poste 2///////////////////////////////////////

		//t101 M[101] : Navette vide ; M[103] : Navette occuppee

		if(M[101]!=0 && cmd.NavetteStoppeeVide(2)!=0)
		{
			modif=1;
			M[102]=cmd.NavetteStoppeeVide(2);
			M[101]--;
		}

		//t102
		if(M[103]!=0 && cmd.NavetteStoppee(2)!=0)
		{
			modif=1;
			M[104]=cmd.NavetteStoppee(2);
			M[103]--;
			cmd.NavettePartie(2);
		}

		//t103
		if(M[102]!=0)
		{
			cout<<"Prodiot sur navette : "<<cmd.ProduitSurNavette(M[102])<<endl;
		}
		if(M[102]!=0 && M[50]==1 && cmd.ProduitSurNavette(M[104])==0)
		{
			modif=1;
			M[105]=M[102];
			M[102]=0;
			M[50]=0;
			Robots.ControlerRobot(1, 4, -1, 1);
		}


		//t104
		if(M[105]!=0 &&   Robots.PinceEnPosition(1)==1)
		{
			modif=1;
			M[106]=M[105];
			M[105]=0;
			Robots.MonterBras(1);
		}

		//t105
		if(M[106]!=0 && M[114]!=0 && Robots.BrasEnPosition(1)==1)
		{
			modif=1;
			M[107]=M[106];
			M[106]=0;
			M[114]=0;
			Robots.ControlerRobot(1, 3, -1, -1);
		}


		//t106
		if(M[107]!=0  &&   Robots.RobotEnPosition(1)==1 && Robots.BrasEnPosition(1)==-1 && Robots.PinceEnPosition(1)==-1)
		{
			modif=1;
			M[108]=M[107];
			M[107]=0;
			Robots.MonterBras(1);
		}

		//t107
		if(M[104]!=0 && M[50]==1 && cmd.ProduitSurNavette(M[104])!=0)
		{
			modif=1;
			M[109]=M[104];
			M[104]=0;
			M[50]=0;
			Robots.ControlerRobot(1, 3, -1, 1);
		}


		//t108
		if(M[109]!=0  &&  Robots.RobotEnPosition(1)==1 && Robots.BrasEnPosition(1)==-1 && Robots.PinceEnPosition(1)==1)
		{
			modif=1;
			M[110]=M[109];
			M[109]=0;
			Robots.MonterBras(1);
		}

		//t109
		if(M[110]!=0 && Robots.BrasEnPosition(1)==1)
		{
			modif=1;
			M[111]=M[110];
			M[110]=0;
			cmd.NavettePartie(2);
			cmd.PiecePrise(2);
			M[0]++;
			Robots.ControlerRobot(1, 4, -1, -1);
		}


		//t110
		if(M[111]!=0  &&   Robots.RobotEnPosition(1)==1 && Robots.BrasEnPosition(1)==-1 && Robots.PinceEnPosition(1)==-1)
		{
			modif=1;
			M[113]=M[111];
			M[111]=0;
			M[112]=1;
			Robots.MonterBras(1);

		}

		//t111
		if(M[113]!=0 && M[0]!=0 && Robots.TraitementFini(2)==1)
		{
			modif=1;
			M[0]--;
			M[113]=0;
			M[114]=1;
			M[101]++;
			int handle;
			handle=cmd.NavetteDisponible();
			cmd.DefinirDestination(handle, 2);
		}

		//t112
		if(M[108]!=0 && Robots.BrasEnPosition(1)==1)
		{
			modif=1;

			M[115]=M[108];
			M[108]=0;
			M[50]=1;
			cmd.NavettePartie(2);
			cmd.PieceDeposee(2);
			usleep(500000);
		}

		//t113
		if(M[112]!=0 && Robots.BrasEnPosition(1)==1)
		{
			modif=1;

			M[112]=0;
			M[50]=1;
		}


		//t114
		if(M[115]!=0 && cmd.ProduitSurNavette(M[115])==1)//envoie vers P1
		{
			M[3]++;
			modif=1;
			cout<<"Envoi vers P1 : produit : "<<cmd.ProduitSurNavette(M[115])<<endl;
			cmd.DefinirDestination(M[115], 1);
			M[115]=0;
		}

		//t115
		if(M[115]!=0 && cmd.ProduitSurNavette(M[115])==3)//envoie vers P3
		{
			M[203]++;
			modif=1;
			cout<<"Envoi vers P3 : produit : "<<cmd.ProduitSurNavette(M[115])<<endl;
			cmd.DefinirDestination(M[115], 3);
			M[115]=0;
		}

		//t116
		if(M[115]!=0 && cmd.ProduitSurNavette(M[115])==4)//envoie vers P4
		{
			M[303]++;
			modif=1;
			cout<<"Envoi vers P4 : produit : "<<cmd.ProduitSurNavette(M[115])<<endl;
			cmd.DefinirDestination(M[115], 4);
			M[115]=0;
		}

		//t117
		if(M[115]!=0 && cmd.ProduitSurNavette(M[115])==5)//sortie du produit
		{
			//M[103]++;
			modif=1;
			cout<<"Sortie de la navette : produit : "<<cmd.ProduitSurNavette(M[115])<<endl;
			cmd.DefinirDestination(M[115], 5);
			M[115]=0;
		}

		Nb_Place_T2=15;



////////////////////Poste 3//////////////////////////////

		//t201
		if(M[201]!=0 && cmd.NavetteStoppeeVide(3)!=0)
		{
			modif=1;
			M[202]=cmd.NavetteStoppeeVide(3);
			M[201]--;
		}

		//202
		if(M[203]!=0 && cmd.NavetteStoppee(3)!=0)
		{
			modif=1;
			M[204]=cmd.NavetteStoppee(3);
			M[203]--;
			cmd.NavettePartie(3);
		}




		//t203
		if(M[202]!=0 && M[250]==1 && cmd.ProduitSurNavette(M[202])==0)
		{
			modif=1;
			M[205]=M[202];
			M[202]=0;
			M[250]=0;
			Robots.ControlerRobot(2, 1, -1, 1);
		}


		//t204
		if(M[205]!=0 &&   Robots.PinceEnPosition(2)==1)
		{
			modif=1;
			M[206]=M[205];
			M[205]=0;
			Robots.MonterBras(2);
		}

		//t205
		if(M[206]!=0 && M[214]!=0 && Robots.BrasEnPosition(2)==1)
		{
			modif=1;
			M[207]=M[206];
			M[206]=0;
			M[214]=0;
			Robots.ControlerRobot(2, 2, -1, -1);
		}


		//t206
		if(M[207]!=0  &&   Robots.RobotEnPosition(2)==1&& Robots.BrasEnPosition(2)==-1 && Robots.PinceEnPosition(2)==-1)
		{
			modif=1;
			M[208]=M[207];
			M[207]=0;
			Robots.MonterBras(2);
		}

		//t207
		if(M[204]!=0 && M[250]==1 && cmd.ProduitSurNavette(M[204])!=0)
		{
			modif=1;
			M[209]=M[204];
			M[204]=0;
			M[250]=0;
			Robots.ControlerRobot(2, 2, -1, 1);
		}


		//t208
		if(M[209]!=0  &&  Robots.RobotEnPosition(2)==1 && Robots.BrasEnPosition(2)==-1 && Robots.PinceEnPosition(2)==1)
		{
			modif=1;
			M[210]=M[209];
			M[209]=0;
			Robots.MonterBras(2);
		}

		//t209
		if(M[210]!=0 && Robots.BrasEnPosition(2)==1)
		{
			modif=1;
			M[211]=M[210];
			M[210]=0;
			cmd.NavettePartie(3);
			cmd.PiecePrise(3);
			M[0]++;
			Robots.ControlerRobot(2, 1, -1, -1);
		}


		//t210
		if(M[211]!=0  &&   Robots.RobotEnPosition(2)==1 && Robots.BrasEnPosition(2)==-1 && Robots.PinceEnPosition(2)==-1)
		{
			modif=1;
			M[213]=M[211];
			M[211]=0;
			M[212]=1;
			Robots.MonterBras(2);

		}

		//t211
		if(M[213]!=0 && M[0]!=0 && Robots.TraitementFini(3)==1)
		{
			modif=1;
			M[0]--;
			M[213]=0;
			M[214]=1;
			M[201]++;
			int handle;
			handle=cmd.NavetteDisponible();
			cmd.DefinirDestination(handle, 3);
		}

		//t212
		if(M[208]!=0 && Robots.BrasEnPosition(2)==1)
		{
			modif=1;

			M[215]=M[208];
			M[208]=0;
			M[250]=1;
			cmd.NavettePartie(3);
			cmd.PieceDeposee(3);
			usleep(500000);
		}

		//t213
		if(M[212]!=0 && Robots.BrasEnPosition(2)==1)
		{
			modif=1;

			M[212]=0;
			M[250]=1;
		}

		//t214
		if(M[215]!=0 && cmd.ProduitSurNavette(M[215])==2)//envoie vers P2
		{
			M[103]++;
			modif=1;
			cout<<"Envoi vers P2 : produit : "<<cmd.ProduitSurNavette(M[215])<<endl;
			cmd.DefinirDestination(M[215], 2);
			M[215]=0;
		}

		//t215
		if(M[215]!=0 && cmd.ProduitSurNavette(M[215])==1)//envoie vers P1
		{
			M[3]++;
			modif=1;
			cout<<"Envoi vers P3 : produit : "<<cmd.ProduitSurNavette(M[215])<<endl;
			cmd.DefinirDestination(M[215], 1);
			M[215]=0;
		}

		//t216
		if(M[215]!=0 && cmd.ProduitSurNavette(M[215])==4)//envoie vers P4
		{
			M[303]++;
			modif=1;
			cout<<"Envoi vers P4 : produit : "<<cmd.ProduitSurNavette(M[215])<<endl;
			cmd.DefinirDestination(M[215], 4);
			M[215]=0;
		}

		//t217
		if(M[215]!=0 && cmd.ProduitSurNavette(M[215])==5)//sortie du produit
		{
			modif=1;
			cout<<"Sortie de la navette : produit : "<<cmd.ProduitSurNavette(M[215])<<endl;
			cmd.DefinirDestination(M[215], 5);
			M[215]=0;
		}


		Nb_Place_T3=15;

/////////////////////////Poste 4///////////////////////////////////////

		//t301 M[101] : Navette vide ; M[103] : Navette occuppee

		if(M[301]!=0 && cmd.NavetteStoppeeVide(4)!=0)
		{
			modif=1;
			M[302]=cmd.NavetteStoppeeVide(4);
			M[301]--;
		}

		//302
		if(M[303]!=0 && cmd.NavetteStoppee(4)!=0)
		{
			modif=1;
			M[304]=cmd.NavetteStoppee(4);
			M[303]--;
			cmd.NavettePartie(4);
		}




		//t303
		if(M[302]!=0 && M[250]==1 && cmd.ProduitSurNavette(M[304])==0)
		{
			modif=1;
			M[305]=M[302];
			M[302]=0;
			M[250]=0;
			Robots.ControlerRobot(2, 4, -1, 1);
		}


		//t304
		if(M[305]!=0 &&   Robots.PinceEnPosition(2)==1)
		{
			modif=1;
			M[306]=M[305];
			M[305]=0;
			Robots.MonterBras(2);
		}

		//t305
		if(M[306]!=0 && M[314]!=0 && Robots.BrasEnPosition(2)==1)
		{
			modif=1;
			M[307]=M[306];
			M[306]=0;
			M[314]=0;
			Robots.ControlerRobot(2, 3, -1, -1);
		}


		//t306
		if(M[307]!=0  &&   Robots.RobotEnPosition(2)==1 && Robots.BrasEnPosition(2)==-1 && Robots.PinceEnPosition(2)==-1)
		{
			modif=1;
			M[308]=M[307];
			M[307]=0;
			Robots.MonterBras(2);
		}

		//t307
		if(M[304]!=0 && M[250]==1 && cmd.ProduitSurNavette(M[304])!=0)
		{
			modif=1;
			M[309]=M[304];
			M[304]=0;
			M[250]=0;
			Robots.ControlerRobot(2, 3, -1, 1);
		}


		//t308
		if(M[309]!=0  &&  Robots.RobotEnPosition(2)==1 && Robots.BrasEnPosition(2)==-1 && Robots.PinceEnPosition(2)==1)
		{
			modif=1;
			M[310]=M[309];
			M[309]=0;
			Robots.MonterBras(2);
		}

		//t309
		if(M[310]!=0 && Robots.BrasEnPosition(2)==1)
		{
			modif=1;
			M[311]=M[310];
			M[310]=0;
			cmd.NavettePartie(4);
			cmd.PiecePrise(4);
			M[0]++;
			Robots.ControlerRobot(2, 4, -1, -1);
		}


		//t310
		if(M[311]!=0  &&   Robots.RobotEnPosition(2)==1 && Robots.BrasEnPosition(2)==-1 && Robots.PinceEnPosition(2)==-1)
		{
			modif=1;
			M[313]=M[311];
			M[311]=0;
			M[312]=1;
			Robots.MonterBras(2);

		}

		//t311
		if(M[313]!=0 && M[0]!=0 && Robots.TraitementFini(4)==1)
		{
			modif=1;
			M[0]--;
			M[313]=0;
			M[314]=1;
			M[301]++;
			int handle;
			handle=cmd.NavetteDisponible();
			cmd.DefinirDestination(handle, 4);
		}

		//t312
		if(M[308]!=0 && Robots.BrasEnPosition(2)==1)
		{
			modif=1;

			M[315]=M[308];
			M[308]=0;
			M[250]=1;
			cmd.NavettePartie(4);
			cmd.PieceDeposee(4);
			usleep(500000);
		}

		//t313
		if(M[312]!=0 && Robots.BrasEnPosition(2)==1)
		{
			modif=1;

			M[312]=0;
			M[250]=1;
		}


		//t314
		if(M[315]!=0 && cmd.ProduitSurNavette(M[315])==1)//envoie vers P1
		{
			M[3]++;
			modif=1;
			cout<<"Envoi vers P1 : produit : "<<cmd.ProduitSurNavette(M[315])<<endl;
			cmd.DefinirDestination(M[315], 1);
			M[315]=0;
		}

		//t315
		if(M[315]!=0 && cmd.ProduitSurNavette(M[315])==3)//envoie vers P3
		{
			M[203]++;
			modif=1;
			cout<<"Envoi vers P3 : produit : "<<cmd.ProduitSurNavette(M[315])<<endl;
			cmd.DefinirDestination(M[315], 3);
			M[315]=0;
		}

		//t316
		if(M[315]!=0 && cmd.ProduitSurNavette(M[315])==2)//envoie vers P2
		{
			M[103]++;
			modif=1;
			cout<<"Envoi vers P2 : produit : "<<cmd.ProduitSurNavette(M[315])<<endl;
			cmd.DefinirDestination(M[315], 2);
			M[315]=0;
		}

		//t317
		if(M[315]!=0 && cmd.ProduitSurNavette(M[315])==5)//sortie du produit
		{
			modif=1;
			cout<<"Sortie de la navette : produit : "<<cmd.ProduitSurNavette(M[315])<<endl;
			cmd.DefinirDestination(M[315], 5);
			M[315]=0;
		}

		Nb_Place_T4=15; */


////////////////////Affichage//////////////////////////////////////////////

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
