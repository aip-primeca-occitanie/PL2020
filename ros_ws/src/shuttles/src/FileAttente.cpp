#include "FileAttente.h"
#include <ros/ros.h>
using namespace std;


FileAttente::FileAttente(int id_aiguillage, int successeur_droite, int successeur_gauche, queue<int> spec_queue)
{
    id_aiguillage_=id_aiguillage;
    successeur_droite_=successeur_droite;
    successeur_gauche_=successeur_gauche;
    queue_=spec_queue;
}

FileAttente::~FileAttente()
{
}


int FileAttente::maj(int DD, int DG)
{
    //on est là suite au front descendant du capteur concerné
    if (!queue_.empty())
    {
        if (id_aiguillage_==0){return successeur_droite_;}
        //appeler service, pour savoir comment est l'aiguillage
        if(DD)//si l'aiguillage est a droite
        {
            return successeur_droite_;
        }
        else if (DG)//si l'aiguillage est a gauche
        {
            return successeur_gauche_;
        }
        else
        {
            return 0;
        }
    }
    else
    {
        return 0;
    }
}

int FileAttente::get_first_navette()
{
    return queue_.front();
}

void FileAttente::add_navette_in_queue(int navette)
{
    queue_.push(navette);
}

void FileAttente::delete_navette_in_queue()
{
    if (queue_.empty())
    {
        ROS_INFO("Attention, ça va segfault dans 10 secondes, désolé");
        usleep(10000000);
    }
    queue_.pop();
}

int FileAttente::get_id_aiguillage()
{
    return id_aiguillage_;
}

queue<int> FileAttente::get_queue()
{
    return queue_;
}
