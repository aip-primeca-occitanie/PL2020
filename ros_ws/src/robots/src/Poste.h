/**** Projet long N7 2020 ****/

#ifndef POSTE
#define POSTE

#include <ros/ros.h>
#include <string>
#include <string.h>
#include <std_msgs/Int32.h>

using namespace std;

class Poste
{
private:
  string nom_poste_;
  int produit_poste_;
  int numero_poste_;

  int pres_requis_tache_[3];
  int sortie_tache_[3];
  int temps_operation_[3];

  ros::Subscriber sub_nouveau_produit;

public:
	Poste();
	~Poste();

  void init(ros::NodeHandle noeud, string nom, int numero);
  string get_nom();
  int do_task(int num_tache);
  void ajouter_produit(int produit);
  void ajouter_produitCallback(std_msgs::Int32 msg);
};
#endif
