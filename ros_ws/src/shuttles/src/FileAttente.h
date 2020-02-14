#ifndef FILEATTENTE
#define FILEATTENTE
#include <iostream>
#include <queue>

using namespace std;


class FileAttente
{
    private:
        int id_aiguillage_;
        int successeur_droite_;
        int successeur_gauche_;
        queue<char> queue_;

    public:
	    FileAttente(int id_aiguillage, int successeur_droite, int successeur_gauche, queue<char> spec_queue);
	    ~FileAttente();
        int maj(int DD, int DG);
        char get_first_navette();
        void add_navette_in_queue(char navette);
        void delete_navette_in_queue();
        int get_id_aiguillage();
        queue<char> get_queue();
};
#endif
