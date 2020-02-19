/**** Projet long N7 2020 ****/
#include "Poste.h"

//Constructeur
Poste::Poste()
{
	produit_poste_=0; // 0 : pas de produit dans poste
	duree_task_=0;
	temps_debut_task_=0;
	task_en_cours_=false;
}

//Destructeur
Poste::~Poste()
{
}


void Poste::init(string nom, int numero)
{
	nom_poste_=nom;
	numero_poste_=numero;
	couleur_=numero_poste_*10+3;
	pres_requis_tache_[0]=1;//on peut changer en fonction du nom du poste
	pres_requis_tache_[1]=2;
	pres_requis_tache_[2]=5;
	sortie_tache_[0]=5;
	sortie_tache_[1]=1;
	sortie_tache_[2]=2;
	temps_operation_[0]=2;// a remonter deux classes plus haut
	temps_operation_[1]=2;
	temps_operation_[2]=2;
}

//le robot ne doit pas être occupé pendant la tâche

string Poste::get_nom()
{
	return nom_poste_;
}

int Poste::get_color()
{
	return couleur_;
}

int Poste::get_numero()
{
	return numero_poste_;
}

int Poste::get_duree()
{
	return duree_task_;
}

int Poste::do_task(int num_tache)
{
	if (num_tache<11 && num_tache>-1)
	{
		if(produit_poste_==pres_requis_tache_[num_tache])
		{
			sleep(temps_operation_[num_tache]);
			produit_poste_=sortie_tache_[num_tache];
		}
		else
		{
			ROS_INFO("Je refuse, tu t'es plante, c'est pas le bon produit pour faire cette tache");
		}
	}
	else
	{
		ROS_INFO("Je refuse, tu t'es plante, c'est pas un bon numéro de tache");
	}
	return produit_poste_;
}

int Poste::get_produit()
{
	return produit_poste_;
}

void Poste::ajouter_produit(int produit)
{
	produit_poste_=produit;
}

void Poste::debutTask(float VREPtime, int duree)
{
	task_en_cours_=true;
	temps_debut_task_=VREPtime;
	duree_task_=duree;
}

bool Poste::updateTask(float time)
{
	bool fin_tache=false;

	cout << "Poste N=" << numero_poste_ << " task_en_cours=" << task_en_cours_ << fixed << setprecision(1) << " time=" << time << " fin=" << temps_debut_task_+duree_task_ << " temps_debut=" << temps_debut_task_ << " duree=" << duree_task_ << endl;

	if(task_en_cours_ && time>temps_debut_task_+duree_task_)
	{
		task_en_cours_=false;
		fin_tache=true;
	}

	return fin_tache;		
}

bool Poste::isTaskEnCours()
{
	return task_en_cours_;
}

void Poste::stopTask()
{
	task_en_cours_=false;
}
