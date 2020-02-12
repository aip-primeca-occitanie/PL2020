/**** Projet long N7 2020 ****/
#include "Robot.h"

using namespace std;

//Constructeur
Robot::Robot(int num_du_robot)
{
	//Valeur de pi
	pi=3.14159265359;
	num_robot=num_du_robot;
	ROS_INFO("le numero du robot est %d ", num_robot);
	for (int i=0;i<7;i++)
	{
		//ATTENTION : Ne pas changer la valeur d'initialisation
		//l'ensemble des setModes doit être à 1 pour que les joints des robots soient commandés
		mymodes[i]=1;
	}

	for(int i=0; i<4; i++)
		couleur_transportee[i]=0;

	repSim_getObjectHandle=false;
        repSim_setJointState=false;
        repSim_getJointState=false;
        repSim_getTime=false;
	repSim_changeColor=false;
	repSim_changeShuttleColor=false;
	repSim_getColor=false;

	msgSim_setJointState.layout.dim.push_back(std_msgs::MultiArrayDimension());
	msgSim_setJointState.layout.dim[0].label="handles";
	msgSim_setJointState.layout.dim[0].size=7;
	msgSim_setJointState.layout.dim[0].stride=1;
	msgSim_setJointState.layout.dim.push_back(std_msgs::MultiArrayDimension());
	msgSim_setJointState.layout.dim[0].label="values";
	msgSim_setJointState.layout.dim[0].size=7;
	msgSim_setJointState.layout.dim[0].stride=1;
	msgSim_setJointState.layout.data_offset=0;

	loop_rate = new ros::Rate(25);
}

//Destructeur
Robot::~Robot()
{
	delete loop_rate;
}

/** Pour atteindre une position prédéfinie **/
//Fonction permettant d'envoyer le robot dans une position prédéfinie
void Robot::EnvoyerRobot(int numposition)
{
	robotPosition.data=numposition;
	//Choix de la position en fonction du numéro reçu
	switch(numposition)
	{
		case 1:
			Rpos[0]=128*pi/180;       // attributs correspondant au position (7 axes du robot)
			Rpos[1]=90*pi/180;
			Rpos[2]=90*pi/180;
			Rpos[3]=80*pi/180;
			Rpos[4]=90*pi/180;
			Rpos[5]=-90*pi/180;
			Rpos[6]=-40*pi/180;
			break;

		case 2:
			Rpos[0]=60*pi/180;
			Rpos[1]=90*pi/180;
			Rpos[2]=90*pi/180;
			Rpos[3]=80*pi/180;
			Rpos[4]=90*pi/180;
			Rpos[5]=-90*pi/180;
			Rpos[6]=-20*pi/180;
			break;

		case 3:
			Rpos[0]=15*pi/180;
			Rpos[1]=90*pi/180;
			Rpos[2]=90*pi/180;
			Rpos[3]=80*pi/180;
			Rpos[4]=90*pi/180;
			Rpos[5]=-90*pi/180;
			Rpos[6]=-65*pi/180;
			break;

		case 4:
			Rpos[0]=-53*pi/180;
			Rpos[1]=90*pi/180;
			Rpos[2]=90*pi/180;
			Rpos[3]=80*pi/180;
			Rpos[4]=90*pi/180;
			Rpos[5]=-90*pi/180;
			Rpos[6]=-40*pi/180;
			break;
	}


	// Utilisation du topic SetJointState pour envoyer le robot dans la position souhaitée
	msgSim_setJointState.data.clear();
	for(int i=0; i++; i<7)
		msgSim_setJointState.data.push_back((float)Rints[i]);
	for(int i=0; i++; i<7)
		msgSim_setJointState.data.push_back(Rpos[i]);

	pubSim_setJointState.publish(msgSim_setJointState);

	//Attente de la réponse
	while(!repSim_setJointState&&ros::ok())
	{
		ros::spinOnce();
		loop_rate->sleep();
	}
	repSim_setJointState=false;


	//Utilisation du topic GetJointState pour savoir si le mouvement du robot est terminé :
	//on considère que la position est atteinte si la position est à 0.001 de sa consigne
	float Position;
	for(int i=0;i<7;i++)
	{
		Position=0;

		//Appel du topic pour connaître la position du robot
		msgSim_getJointState.data = Rints[i];
		pubSim_getJointState.publish(msgSim_getJointState);
		while(!repSim_getJointState&&ros::ok())
		{
			ros::spinOnce();
			loop_rate->sleep();
		}
		repSim_getJointState = false;
		Position = valueSim_getJointState;

		//Attente jusqu'à ce que la position soit atteinte
		while(abs(Position-Rpos[i])>=0.001)
		{
			pubSim_getJointState.publish(msgSim_getJointState);
			while(!repSim_getJointState&&ros::ok())
			{
				ros::spinOnce();
				loop_rate->sleep();
			}
			repSim_getJointState=false;
			Position = valueSim_getJointState;
		}
	}

	ROS_INFO("Position atteinte robot %d", num_robot);

	//Retour vers la commande
	retour.data = 2;
	pub_retourCommande.publish(retour);

	//Retour de la position actuelle du robot
	pub_robotPosition.publish(robotPosition);
}

/** Pour atteindre une position définie manuellement **/
//Fonction permettant d'envoyer le robot dans une position définie manuellement
void Robot::EnvoyerJoints(int joint1, int joint2, int joint3, int joint4, int joint5, int joint6, int joint7)
{
	//Récupération des angles choisis par l'utilisateur
	Rpos[0]=(joint1)*pi/180;
	Rpos[1]=(joint2)*pi/180;
	Rpos[2]=(joint3)*pi/180;
	Rpos[3]=(joint4)*pi/180;
	Rpos[4]=(joint5)*pi/180;
	Rpos[5]=(joint6)*pi/180;
	Rpos[6]=(joint7)*pi/180;


	// Utilisation du topic SetJointState pour envoyer le robot dans la position souhaitée
	msgSim_setJointState.data.clear();
	for(int i=0; i<7; i++)
		msgSim_setJointState.data.push_back((float)Rints[i]);
	for(int i=0; i<7; i++)
		msgSim_setJointState.data.push_back(Rpos[i]);

	pubSim_setJointState.publish(msgSim_setJointState);

	//Attente de la réponse
	while(!repSim_setJointState&&ros::ok())
	{
		ros::spinOnce();
		loop_rate->sleep();
	}
	repSim_setJointState=false;


	//Utilisation du topic GetJointState pour savoir si le mouvement du robot est terminé
	//On considère que la position est atteinte si la position est à 0.001 de sa consigne
	float Position;
	for(int i=0;i<7;i++)
	{
		Position=0;

		//Appel du topic pour connaître la position du robot
		msgSim_getJointState.data = Rints[i];
		pubSim_getJointState.publish(msgSim_getJointState);
		while(!repSim_getJointState&&ros::ok())
		{
			ros::spinOnce();
			loop_rate->sleep();
		}
		repSim_getJointState = false;
		Position = valueSim_getJointState;

		//Attente jusqu'à ce que la position soit atteinte
		while(abs(Position-Rpos[i])>=0.001)
		{
			pubSim_getJointState.publish(msgSim_getJointState);
			while(!repSim_getJointState&&ros::ok())
			{
				ros::spinOnce();
				loop_rate->sleep();
			}
			repSim_getJointState=false;
			Position = valueSim_getJointState;
		}
	}

	ROS_INFO("Position atteinte %d", num_robot);

	//Retour vers la commande
	retour.data = 2;
	pub_retourCommande.publish(retour);
}

/** Pour descendre ou monter le bras **/
//Fonction permettant de mettre le bras en position basse
void Robot::DescendreBras()
{
	//Récupération et modification de la position actuelle
	Rpos[0]=Rpos[0]+2*pi/180;
	Rpos[1]=Rpos[1]+10*pi/180;
	Rpos[2]=Rpos[2];
	Rpos[3]=Rpos[3];
	Rpos[4]=Rpos[4]+10*pi/180;
	Rpos[5]=Rpos[5]+3*pi/180;
	Rpos[6]=Rpos[6]+6*pi/180;


	// Utilisation du topic SetJointState pour envoyer le robot dans la position souhaitée
	msgSim_setJointState.data.clear();
	for(int i=0; i++; i<7)
		msgSim_setJointState.data.push_back((float)Rints[i]);
	for(int i=0; i++; i<7)
		msgSim_setJointState.data.push_back(Rpos[i]);

	pubSim_setJointState.publish(msgSim_setJointState);

	//Attente de la réponse
	while(!repSim_setJointState&&ros::ok())
	{
		ros::spinOnce();
		loop_rate->sleep();
	}
	repSim_setJointState=false;

	//Utilisation du topic GetJointState pour savoir si le mouvement du robot est terminé
	//On considère que la position est atteinte si la position est à 0.001 de sa consigne
	float Position;
	for(int i=0;i<7;i++)
	{
		Position=0;

		//Appel du topic pour connaître la position du robot
		msgSim_getJointState.data = Rints[i];
		pubSim_getJointState.publish(msgSim_getJointState);
		while(!repSim_getJointState&&ros::ok())
		{
			ros::spinOnce();
			loop_rate->sleep();
		}
		repSim_getJointState = false;
		Position = valueSim_getJointState;

		//Attente jusqu'à ce que la position soit atteinte
		while(abs(Position-Rpos[i])>=0.001)
		{
			pubSim_getJointState.publish(msgSim_getJointState);
			while(!repSim_getJointState&&ros::ok())
			{
				ros::spinOnce();
				loop_rate->sleep();
			}
			repSim_getJointState=false;
			Position = valueSim_getJointState;
		}
	}


	ROS_INFO("Bras descendu pour le robot %d", num_robot);

	//Retour vers la commande
	retour.data = 4;
	pub_retourCommande.publish(retour);

	//Retour de l'état actuel du bras
	robotBras.data = 0;
	pub_robotBras.publish(robotBras);
}



//Fonction permettant de mettre le bras en position haute
void Robot::MonterBras()
{
	//Récupération et modification de la position actuelle
	Rpos[0]=Rpos[0]-2*pi/180;
	Rpos[1]=Rpos[1]-10*pi/180;
	Rpos[2]=Rpos[2];
	Rpos[3]=Rpos[3];
	Rpos[4]=Rpos[4]-10*pi/180;
	Rpos[5]=Rpos[5]-3*pi/180;
	Rpos[6]=Rpos[6]-6*pi/180;


	// Utilisation du topic SetJointState pour envoyer le robot dans la position souhaitée
	msgSim_setJointState.data.clear();
	for(int i=0; i++; i<7)
		msgSim_setJointState.data.push_back((float)Rints[i]);
	for(int i=0; i++; i<7)
		msgSim_setJointState.data.push_back(Rpos[i]);

	pubSim_setJointState.publish(msgSim_setJointState);

	//Attente de la réponse
	while(!repSim_setJointState&&ros::ok())
	{
		ros::spinOnce();
		loop_rate->sleep();
	}
	repSim_setJointState=false;

	//Utilisation du topic GetJointState pour savoir si le mouvement du robot est terminé
	//On considère que la position est atteinte si la position est à 0.001 de sa consigne
	float Position;
	for(int i=0;i<7;i++)
	{
		Position=0;

		//Appel du topic pour connaître la position du robot
		msgSim_getJointState.data = Rints[i];
		pubSim_getJointState.publish(msgSim_getJointState);
		while(!repSim_getJointState&&ros::ok())
		{
			ros::spinOnce();
			loop_rate->sleep();
		}
		repSim_getJointState = false;
		Position = valueSim_getJointState;

		//Attente jusqu'à ce que la position soit atteinte
		while(abs(Position-Rpos[i])>=0.001)
		{
			pubSim_getJointState.publish(msgSim_getJointState);
			while(!repSim_getJointState&&ros::ok())
			{
				ros::spinOnce();
				loop_rate->sleep();
			}
			repSim_getJointState=false;
			Position = valueSim_getJointState;
		}
	}


	ROS_INFO("Bras monte pour le robot %d",num_robot);

	//Retour vers la commande
	retour.data = 5;
	pub_retourCommande.publish(retour);

	//Retour de l'état actuel du bras
	robotBras.data = 1;
	pub_robotBras.publish(robotBras);
}

/** Pour fermer ou ouvrir la pince **/
//Fonction permettant de fermer la pince du robot en envoyant une commande sur le topic correspondant
void Robot::FermerPince()
{
	//Message de commande de fermeture de la pince (1 pour fermer, 0 pour ouvrir)
	std_msgs::Int32 cmd;
	cmd.data = 1;

	pub_pince.publish(cmd);

	//Si la pince n'est pas déjà fermée, un retour est envoyé à la commande
	if(retour.data != 6)
	{
		//Attente pour que la pince se ferme
		float t0, time;

		pubSim_getTime.publish(msgSim_getTime);
		while(!repSim_getTime&&ros::ok())
		{
			ros::spinOnce();
			loop_rate->sleep();
		}
		repSim_getTime=false;
		t0 = valueSim_getTime;

		time = t0;
		while(time - t0 < 2)
		{
			pubSim_getTime.publish(msgSim_getTime);
			while(!repSim_getTime&&ros::ok())
			{
				ros::spinOnce();
				loop_rate->sleep();
			}
			repSim_getTime=false;
			time = valueSim_getTime;
		}

		//Retour vers la commande
		retour.data = 6;
		pub_retourCommande.publish(retour);
	}

	//Retour de l'état de la pince
	robotPince.data=1;
	pub_robotPince.publish(robotPince);
}

//Fonction permettant d'ouvrir la pince du robot en envoyant une commande sur le topic correspondant
void Robot::OuvrirPince()
{
	//Message de commande d'ouverture de la pince (1 pour fermer, 0 pour ouvrir)
	std_msgs::Int32 cmd;
	cmd.data = 0;

	pub_pince.publish(cmd);

	//Si la pince n'est pas déjà ouverte, un retour est envoyé à la commande
	if(retour.data != 7)
	{
		//Attente pour que la pince s'ouvre
		float t0, time;

		pubSim_getTime.publish(msgSim_getTime);
		while(!repSim_getTime&&ros::ok())
		{
			ros::spinOnce();
			loop_rate->sleep();
		}
		repSim_getTime=false;
		t0 = valueSim_getTime;

		time = t0;
		while(time - t0 < 1)
		{
			pubSim_getTime.publish(msgSim_getTime);
			while(!repSim_getTime&&ros::ok())
			{
				ros::spinOnce();
				loop_rate->sleep();
			}
			repSim_getTime=false;
			time = valueSim_getTime;
		}

		//Retour vers la commande
		retour.data = 7;
		pub_retourCommande.publish(retour);
	}

	//Retour de l'état de la pince
	robotPince.data=0;
	pub_robotPince.publish(robotPince);
}

/*** Fonctions permettant de controler le robot avec des ordres du noeud commande ***/
/** Envoyer le robot automatiquement **/
//Fonction Callback permettant d'envoyer le robot dans une position prédéfinie à la réception du message de Commande
void Robot::SendPositionCallback(const robots::Msg_numrobot::ConstPtr& msg)
{
	if(num_robot==msg->num_robot)
	{
		//Récupération des données du message : numéro de la position prédéfinie
		int pos;
		pos = msg->data;

		//Envoi du robot dans la position choisie
		EnvoyerRobot(pos);
	}
}

/** Envoyer le robot manuellement **/
//Fonction Callback permettant d'envoyer le robot dans une position choisie par l'utilisateur à la réception du message de Commande
void Robot::SendJointsCallback(const robots::RobotJoints::ConstPtr& msg)
{
	if(num_robot==msg->num_robot)
	{
		EnvoyerJoints(msg->joint1, msg->joint2, msg->joint3, msg->joint4, msg->joint5, msg->joint6, msg->joint7);
	}
}

/** Fermer la pince **/
//Fonction Callback permettant de fermer la pince du robot à la réception du message de Commande
void Robot::FermerPinceCallback(const robots::Msg_numrobot::ConstPtr& msg)
{
	if(num_robot==msg->num_robot)
	{
		FermerPince();
	}
}
/** Ouvrir la pince **/
//Fonction Callback permettant d'ouvrir la pince du robot à la réception du message de Commande
void Robot::OuvrirPinceCallback(const robots::Msg_numrobot::ConstPtr& msg)
{
	if(num_robot==msg->num_robot)
	{
		OuvrirPince();
	}
}

/** Descendre le bras **/
//Fonction Callback permettant de mettre le bras en position basse à la réception du message de Commande
void Robot::DescendreBrasCallback(const robots::Msg_numrobot::ConstPtr& msg)
{
	if(num_robot==msg->num_robot)
	{
		DescendreBras();
	}
}

/** Monter le bras **/
//Fonction Callback permettant de mettre le bras en position haute à la réception du message de Commande
void Robot::MonterBrasCallback(const robots::Msg_numrobot::ConstPtr& msg)
{
	if(num_robot==msg->num_robot)
	{
		MonterBras();
	}
}

/** Contrôler le robot entièrement **/
//Fonction Callback permettant de contrôler l'ensemble des mouvements du robot à la réception du message de Commande
void Robot::ControlerRobotCallback(const robots::MoveRobot::ConstPtr& msg)
{
	if(num_robot==msg->num_robot)
	{
		//Envoi du robot dans la position souhaitée
		EnvoyerRobot(msg->position);

		//Envoi du bras dans l'état souhaité
		switch(msg->bras)
		{
			case -1:
				DescendreBras();
				break;

			case 1:
				MonterBras();
				break;
		}

		//Envoi de la pince dans l'état souhaité
		switch(msg->pince)
		{
			case -1:
				OuvrirPince();
				break;

			case 1:
				FermerPince();
				break;
			}
	}
}

int Robot::computeTableId(int position)
{
	int id=-1;

	switch(num_robot)
	{
		case 1:
			if(position==1)
				id=1;
			else if(position==4)
				id=0;
			break;
		case 2:
			if(position==1)
				id=3;
			else if(position==4)
				id=4;
			break;
		case 3:
			if(position==1)
				id=6;
			else if(position==4)
				id=7;
			break;
		case 4:
			if(position==1)
				id=10;
			else if(position==4)
				id=9;
			break;
	}
	
	return id;
}

void Robot::ColorerCallback(const robots::ColorMsg::ConstPtr& msg)//attention c'est forcement quand on transporte !!
{
	if (msg->num_robot==num_robot)
	{
		cout << "me concerne robot=" << msg->num_robot << " pos=" << msg->position << endl;
		int idNavette=-1;
		if(msg->position==2 || msg->position==3) // Si navette
		{
			//idNavette = shuttleManager.getIdNavette(robot=,pos=);
			idNavette=1;
		}
		
	// regarde la couleur de ce qu'on veut prendre (call shuttleManager si navette)
		bool erreur=false;
		int couleur[4];
		char c=(char)(idNavette+64);
		string signal;
		if(msg->position==2 || msg->position==3) // Si navette
		{
			signal="Shuttle"+string(&c);
		}
		else if(msg->position==1 || msg->position==4) // Si poste
		{
			if(msg->position==1)
				signal=poste_pos_1.get_nom();
			else
				signal=poste_pos_4.get_nom();
		}
		else
		{
			erreur=true;
			ROS_ERROR("ColorerCallback: msg->position incorrecte !!!");
		}

		cout << "signal=" << signal << endl;
		string fin;

		if(!erreur)
		{
			for(int i=0; i<4; i++)
			{
				fin.clear();
				fin.append(signal);
				fin.append("#");
				fin.append(to_string(i));
				fin.append("_color");
				msgSim_getColor.data=fin;

				pubSim_getColor.publish(msgSim_getColor);
				while(!repSim_getColor&&ros::ok())
				{
					ros::spinOnce();
					loop_rate->sleep();
				}
				repSim_getColor=false;
				couleur[i]=valueSim_getColor;
			}
		}
	
		cout << "get color" << endl;
		for(int i=0; i<4; i++)
			cout << "couleur[" << i << "]=" << couleur[i] << endl;
	
		int produit_detecte=1;
		if(msg->position==1)
			poste_pos_1.ajouter_produit(produit_detecte);
		else if(msg->position==4)
			poste_pos_4.ajouter_produit(produit_detecte);

		cout << "ajout des produit" << endl;

	// colore le poste ou navette en pos 1 avec couleur en mémoire
		if(msg->position==2 || msg->position==3) // Si navette
		{
			msgSim_changeShuttleColor.data.clear();
			msgSim_changeShuttleColor.data.push_back(idNavette);
			for(int i=0; i<4; i++)
				msgSim_changeShuttleColor.data.push_back(couleur_transportee[i]);
			pubSim_changeShuttleColor.publish(msgSim_changeShuttleColor);
			while(!repSim_changeShuttleColor&&ros::ok())
			{
				ros::spinOnce();
				loop_rate->sleep();
			}
			repSim_changeShuttleColor=false;
		}
		else if(msg->position==1 || msg->position==4)
		{
			cout << "debut change color" << endl;
			msgSim_changeColor.data.clear();
			msgSim_changeColor.data.push_back(computeTableId(msg->position));
			for(int i=0; i<4; i++)
				msgSim_changeColor.data.push_back(couleur_transportee[i]);
			pubSim_changeColor.publish(msgSim_changeColor);
			cout << "apres publish" << endl;
			while(!repSim_changeColor&&ros::ok())
			{
				ros::spinOnce();
				loop_rate->sleep();
			}
			repSim_changeColor=false;
		}

	// on met a jour la couleur en mémoire (qu'on transporte)
		for(int i=0; i<4; i++)
		{
			couleur_transportee[i]=couleur[i];
			cout << "couleur_trasportee[" << i << "]=" << couleur_transportee[i] << endl;
		}
		cout << "fin" << endl;
	}
}

void Robot::colorerPoste(int produit, string poste)
{
	ROS_INFO("La je veux colorer le poste suite a quelquechose, ca arrive bientot");
	ROS_INFO("produit:%d poste:%s",produit, poste.c_str());
}

void Robot::doTaskCallback(const robots::DoTaskMsg::ConstPtr& msg)
{
	if (msg->num_robot==num_robot)
	{
		if (msg->position==1)
		{
			produit_sur_poste=poste_pos_1.do_task(msg->num_tache);
			colorerPoste(produit_sur_poste,poste_pos_1.get_nom());
		}
		if (msg->position==4)
		{
			produit_sur_poste=poste_pos_4.do_task(msg->num_tache);
			colorerPoste(produit_sur_poste,poste_pos_4.get_nom());
		}
	}
}

void Robot::ajouter_produitCallback(commande_locale::Msg_AddProduct msg)
{
	if (poste_pos_1.get_numero()==msg.num_poste)
	{
		poste_pos_1.ajouter_produit(msg.num_produit);
		colorerPoste(poste_pos_1.get_produit(),poste_pos_1.get_nom());
	}
	if (poste_pos_4.get_numero()==msg.num_poste)
	{
		poste_pos_4.ajouter_produit(msg.num_produit);
		colorerPoste(poste_pos_4.get_produit(),poste_pos_4.get_nom());
	}
}


/*** Initialisation ***/
//Initialisation des services, des publishers et des subscribers + Récupération des handles des robots
void Robot::init(ros::NodeHandle noeud)
{
	string num_str;
	string nom;
	int numero_poste;
	switch(num_robot){

	case 1:
		num_str="1";
		nom="Table#1";
		numero_poste=1;
		poste_pos_1.init(nom,numero_poste);
		nom="Table#0";
		numero_poste=2;
		poste_pos_4.init(nom,numero_poste);
	break;

	case 2:
		num_str="2";
		nom="Table#3";
		numero_poste=3;
		poste_pos_1.init(nom,numero_poste);
		nom="Table#4";
		numero_poste=4;
		poste_pos_4.init(nom,numero_poste);
	break;

 	case 3:
		num_str="3";
		nom="Table#6";
		numero_poste=5;
		poste_pos_1.init(nom,numero_poste);
		nom="Table#7";
		numero_poste=6;
		poste_pos_4.init(nom,numero_poste);
	break;

  case 4:
		num_str="4";
		nom="Table#10";
		numero_poste=7;
		poste_pos_1.init(nom,numero_poste);
		nom="Table#9";
		numero_poste=8;
		poste_pos_4.init(nom,numero_poste);
	break;

  default:
		ROS_INFO("CHOIX ROBOT INCORRECT");
	break;
	}

	// Topic pour V-Rep
	pubSim_getObjectHandle = noeud.advertise<std_msgs::String>("/sim_ros_interface/services/robot"+to_string(num_robot)+"/GetObjectHandle",100);
	subSim_getObjectHandle = noeud.subscribe("/sim_ros_interface/services/response/robot"+to_string(num_robot)+"/GetObjectHandle",100,&Robot::simGetObjectHandleCallback,this);

	pubSim_setJointState = noeud.advertise<std_msgs::Float32MultiArray>("/sim_ros_interface/services/robot"+to_string(num_robot)+"/SetJointState",100);
	subSim_setJointState = noeud.subscribe("/sim_ros_interface/services/response/robot"+to_string(num_robot)+"/SetJointState",100,&Robot::simSetJointStateCallback,this);

	pubSim_getJointState = noeud.advertise<std_msgs::Int32>("/sim_ros_interface/services/robot"+to_string(num_robot)+"/GetJointState",100);
	subSim_getJointState = noeud.subscribe("/sim_ros_interface/services/response/robot"+to_string(num_robot)+"/GetJointState",100,&Robot::simGetJointStateCallback,this);

	pubSim_getTime = noeud.advertise<std_msgs::Byte>("/sim_ros_interface/services/robot"+to_string(num_robot)+"/GetTime",100);
	subSim_getTime = noeud.subscribe("/sim_ros_interface/services/response/robot"+to_string(num_robot)+"/GetTime",100,&Robot::simGetTimeCallback,this);

	pubSim_changeColor = noeud.advertise<std_msgs::Int32MultiArray>("/sim_ros_interface/services/robot"+to_string(num_robot)+"/ChangeColor",100);
	subSim_changeColor = noeud.subscribe("/sim_ros_interface/services/response/robot"+to_string(num_robot)+"/ChangeColor",100,&Robot::simChangeColorCallback,this);

	pubSim_changeShuttleColor = noeud.advertise<std_msgs::Int32MultiArray>("/sim_ros_interface/services/robot"+to_string(num_robot)+"/ChangeShuttleColor",100);
	subSim_changeShuttleColor = noeud.subscribe("/sim_ros_interface/services/response/robot"+to_string(num_robot)+"/ChangeShuttleColor",100,&Robot::simChangeShuttleColorCallback,this);

	pubSim_getColor = noeud.advertise<std_msgs::String>("/sim_ros_interface/services/robot"+to_string(num_robot)+"/GetColor",100);
	subSim_getColor = noeud.subscribe("/sim_ros_interface/services/response/robot"+to_string(num_robot)+"/GetColor",100,&Robot::simGetColorCallback,this);


	//Subscribers
	planifSendPosition = noeud.subscribe("/commande/Simulation/SendPositionRobot",10,&Robot::SendPositionCallback,this); // Ici ont récupère ce qui a été publié dans le topic par d'autre programme (ici c'est le programme robots
	planifSendJoints = noeud.subscribe("/commande/Simulation/SendJointsRobot",10,&Robot::SendJointsCallback,this);
 	planifFermerPince = noeud.subscribe("/commande/Simulation/FermerPinceRobot",10,&Robot::FermerPinceCallback,this);
	planifOuvrirPince = noeud.subscribe("/commande/Simulation/OuvrirPinceRobot",10,&Robot::OuvrirPinceCallback,this);
	planifDescendreBras = noeud.subscribe("/commande/Simulation/DescendreBras",10,&Robot::DescendreBrasCallback,this);
	planifMonterBras = noeud.subscribe("/commande/Simulation/MonterBras",10,&Robot::MonterBrasCallback,this);
	planifControlerRobot = noeud.subscribe("/commande/Simulation/ControlerBras",10,&Robot::ControlerRobotCallback,this);
	sub_colorer = noeud.subscribe("/commande/Simulation/Colorer",10,&Robot::ColorerCallback,this);
	sub_doTask = noeud.subscribe("/commande/Simulation/doTask",10,&Robot::doTaskCallback,this);
	sub_nouveau_produit= noeud.subscribe("/commande_locale/AddProduct", 1, &Robot::ajouter_produitCallback,this);


	//Publishers
	pub_pince = noeud.advertise<std_msgs::Int32>("/robot/cmdPinceRobot"+num_str, 10);
	pub_robotPosition = noeud.advertise<std_msgs::Int32>("/robot/PositionRobot"+num_str,10);
	pub_robotBras = noeud.advertise<std_msgs::Int32>("/robot/BrasRobot"+num_str,10);
	pub_robotPince = noeud.advertise<std_msgs::Int32>("/robot/PinceRobot"+num_str,10);
	pub_retourCommande = noeud.advertise<std_msgs::Int32>("/commande/Simulation/retourCommande"+num_str, 10);


	sleep(1);

	//Utilisation du service simRosGetObjectHandle pour obtenir les handles du robot
	for (int i=1;i<8;i++)
	{
		stringstream sr;
		sr << i;
		switch(num_robot){
			case 1:
				msgSim_getObjectHandle.data = "LBR_iiwa_14_R820_joint" + sr.str();
			break;

			case 2:
				msgSim_getObjectHandle.data = "LBR_iiwa_14_R820_joint" + sr.str()+"#0";
			break;

			case 3:
				msgSim_getObjectHandle.data = "LBR_iiwa_14_R820_joint" + sr.str()+"#1";
			break;

			case 4:
				msgSim_getObjectHandle.data = "LBR_iiwa_14_R820_joint" + sr.str()+"#2";
			break;
		}


		pubSim_getObjectHandle.publish(msgSim_getObjectHandle);
		while(!repSim_getObjectHandle&&ros::ok())
		{
			ros::spinOnce();
			loop_rate->sleep() ;
		}
		repSim_getObjectHandle = false;

		Rints[i-1]=valueSim_getObjectHandle;

		if(Rints[i-1]==-1)
		{
			ROS_INFO("Robot %d : Handle non obtenu pour joint %d",num_robot,i);
		}
		else
		{
			if(i==7)
			{
				ROS_INFO("Robot %d OK",num_robot);
				//Retour vers la commande
				retour.data = 0;
				pub_retourCommande.publish(retour);
			}
		}
	}

}

/** Callbacks pour V-Rep **/
void Robot::simGetObjectHandleCallback(const std_msgs::Int32::ConstPtr& msg)
{
	valueSim_getObjectHandle=msg->data;

	repSim_getObjectHandle=true;
}

void Robot::simSetJointStateCallback(const std_msgs::Byte::ConstPtr& msg)
{
        repSim_setJointState=true;
}

void Robot::simGetJointStateCallback(const sensor_msgs::JointState::ConstPtr& msg)
{
	valueSim_getJointState=msg->position[0];

        repSim_getJointState=true;
}

void Robot::simGetTimeCallback(const std_msgs::Float32::ConstPtr& msg)
{
	valueSim_getTime=msg->data;

        repSim_getTime=true;
}

void Robot::simChangeColorCallback(const std_msgs::Byte::ConstPtr& msg)
{
	repSim_changeColor=true;
}

void Robot::simChangeShuttleColorCallback(const std_msgs::Byte::ConstPtr& msg)
{
	repSim_changeShuttleColor=true;
}

void Robot::simGetColorCallback(const std_msgs::Int32::ConstPtr& msg)
{
	valueSim_getColor=msg->data;
	repSim_getColor=true;
}
