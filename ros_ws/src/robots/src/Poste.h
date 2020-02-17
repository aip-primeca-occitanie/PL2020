/**** Projet long N7 2020 ****/

#ifndef POSTE
#define POSTE

#include <ros/ros.h>
#include <string>
#include <string.h>
#include <std_msgs/Int32.h>
#include "commande_locale/Msg_AddProduct.h"

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

  int couleur_;

  int duree_task_;
  float temps_debut_task_;
  bool task_en_cours_;

public:
	Poste();
	~Poste();

  void init(string nom, int numero);
  string get_nom();
  int get_numero();
  int do_task(int num_tache);
  void ajouter_produit(int produit);
  int get_produit();
  int get_color();
  void debutTask(float VREPtime, int duree);
  bool updateTask(float time);
  bool isTaskEnCours();
  void stopTask();
};
#endif
