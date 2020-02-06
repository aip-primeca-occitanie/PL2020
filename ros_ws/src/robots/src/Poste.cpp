/**** Projet long N7 2020 ****/
#include "Poste.h"

//Constructeur
Poste::Poste()
{
  produit_poste_=0; // 0 : pas de produit dans poste
}

//Destructeur
Poste::~Poste()
{

}

void Poste::nommer(string nom)
{
  nom_poste_=nom;
  pres_requis_tache_[0]=1;//on peut changer en fonction du nom du poste
  pres_requis_tache_[1]=2;
  pres_requis_tache_[2]=5;
  sortie_tache_[0]=5;
  sortie_tache_[1]=1;
  sortie_tache_[2]=2;
  temps_operation_[0]=2;
  temps_operation_[1]=2;
  temps_operation_[2]=2;
}

string Poste::get_nom()
{
  return nom_poste_;
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

void Poste::ajouter_produit(int produit)
{
  produit_poste_=produit;
}
