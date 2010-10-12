#include "../include/NetCon.h"


Plateau::Plateau(Engine* engine)
{
        int errors = S_OK;

        Rect rect = { 0,0,0,0 };
        fond = new Sprite("./resources/bg.bmp", engine->GetScreen(), rect, errors);
        fond->SetRect(rect);

        planetA = new Sprite("./resources/planet_neutral_A.bmp", engine->GetScreen(), errors);
        planetB = new Sprite("./resources/planet_neutral_B.bmp", engine->GetScreen(), errors);
        planetC = new Sprite("./resources/planet_neutral_C.bmp", engine->GetScreen(), errors);

        selA = new Sprite("./resources/planet_select_A.bmp", engine->GetScreen(), errors);
        selB = new Sprite("./resources/planet_select_B.bmp", engine->GetScreen(), errors);
        selC = new Sprite("./resources/planet_select_C.bmp", engine->GetScreen(), errors);

        if(errors == S_OK)
            engine->RegisterSprite(fond);

        nbJoueurs = 0;
}
Plateau::~Plateau()
{
        fond->~Sprite();
}
void Plateau::Generer(int _nbMaxJoueurs, Engine* engine)
{
    nbMaxJoueurs = _nbMaxJoueurs;
    srand(SDL_GetTicks());
    tabSizeX = pow(nbMaxJoueurs,2);
    tabSizeY = pow(nbMaxJoueurs,2);
  // tableau = (Case**)malloc(sizeof(Case) * (tabSizeX * tabSizeY));
    tableau = new Case*[tabSizeY];
    for(int i = 0; i < tabSizeY; i++)
        tableau[i] = new Case[tabSizeX];

    for(int i = 0; i < tabSizeY; i++)
        for(int j = 0; j < tabSizeX; j++)
        {
            int chance = rand()%100;
            if(chance > 75)
            {
                int cat = (int) rand()%3 + 1;
                tableau[i][j].occupee = true;
                planetes.push_back(&tableau[i][j].planete);

                planetes.back()->categorie = cat;
                planetes.back()->population = chance;
                planetes.back()->selectionnee = false;
                planetes.back()->amie = false;

                planetes.back()->rect.x = ( (engine->GetScreen()->w - 50) / tabSizeX) * (j);
                planetes.back()->rect.y = ( (engine->GetScreen()->h - 50) / tabSizeY) * (i);
                planetes.back()->rect.w = 0;
                planetes.back()->rect.h = 0;

                tableau[i][j].IDY = i;
                tableau[i][j].IDX = j;

            }


        }
}
void Plateau::Afficher(Engine* engine)
{


        for(int i = 0; i < planetes.size(); i++)
        {
                if(planetes[i]->categorie == 1)
                {
                    planetA->SetRect(planetes[i]->rect);
                    engine->Draw(planetA);
                    if(planetes[i]->selectionnee)
                    {
                        selA->SetRect(planetes[i]->rect);
                        engine->Draw(selA);
                    }

                }
                else if(planetes[i]->categorie == 2)
                {
                    planetB->SetRect(planetes[i]->rect);
                    engine->Draw(planetB);
                    if(planetes[i]->selectionnee)
                    {
                        selB->SetRect(planetes[i]->rect);
                        engine->Draw(selB);
                    }
                }
                else if(planetes[i]->categorie == 3)
                {
                    planetC->SetRect(planetes[i]->rect);
                    engine->Draw(planetC);
                    if(planetes[i]->selectionnee)
                    {
                        selC->SetRect(planetes[i]->rect);
                        engine->Draw(selC);
                    }
                }
        }

}
void Plateau::MettreAJour(Input input, float dt)
{
        Rect rect;
        for(int i = 0; i < planetes.size(); i++)
        {
            if(input.lbutton)
            {
                     if(planetes[i]->categorie == 1)
                    rect = planetA->GetRect();
                else if(planetes[i]->categorie == 2)
                    rect = planetB->GetRect();
                else if(planetes[i]->categorie == 3)
                    rect = planetC->GetRect();

                    //we just needed the w and h values
                    rect.x = planetes[i]->rect.x;
                    rect.y = planetes[i]->rect.y;

                if(MouseIn(input.x, input.y, rect))
                    planetes[i]->selectionnee = true;
                else
                    planetes[i]->selectionnee = false;
            }
        }

}
