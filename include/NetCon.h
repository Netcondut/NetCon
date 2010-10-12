#ifndef NETCON_H_INCLUDED
#define NETCON_H_INCLUDED

#include "Engine.h"
#include <time.h>
#include <math.h>

struct Planete
{
    Rect rect;
   bool amie, selectionnee;
   int categorie, population;
   int poIDX,posIDY;
};
struct Case
{
   int IDX,IDY;
   int x,y;
   bool occupee;
   Planete planete;
};
class Plateau
{
    public:
    Plateau(Engine* engine);
    ~Plateau();
    void Generer(int _nbMaxJoueurs, Engine* engine);
    void Afficher(Engine* engine);
    void MettreAJour(Input input, float dt);

    private:
    Sprite* fond;

    std::vector<Planete*> planetes;
    int nbMaxJoueurs, nbJoueurs;
    int tabSizeX,tabSizeY;
    Case** tableau;
    Sprite* planetA;
    Sprite* planetB;
    Sprite* planetC;

    Sprite* selA;
    Sprite* selB;
    Sprite* selC;

};

#endif // NETCON_H_INCLUDED
