#include "FileAttente.h"
using namespace std;


FileAttente::FileAttente(int id_aiguillage, int successeur_droite, int successeur_gauche, queue<char> spec_queue)
{
    id_aiguillage_=id_aiguillage;
    successeur_droite_=successeur_droite;
    successeur_gauche_=successeur_gauche;
    queue_=spec_queue;
}

FileAttente::~FileAttente()
{
}


int FileAttente::maj()
{
    //on est là suite au front descendant du capteur concerné
    if (!queue_.empty())
    {
        //appeler service, pour savoir comment est l'aiguillage
        if(1)//si l'aiguillage est a droite
        {
            return successeur_droite_;
        }
        else if (0)//si l'aiguillage est a gauche
        {
            return successeur_gauche_;
        }
        else
        {
            //ROS_INFO("J'ai paumé la navette, elle va moins bien marcher maintenant");
            return 0;
        }
    }
}

char FileAttente::get_first_navette()
{
    return queue_.front();
}

void FileAttente::add_navette_in_queue(char navette)
{
    queue_.push(navette);
}
