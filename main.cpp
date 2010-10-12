
    #include <stdio.h>
    #include <stdlib.h>
    #include "include/NetCon.h"



int main ( int argc, char** argv )
{
    int errors = S_OK;
    WindowParams params;
    params.width = 1024;
    params.height = 764;
    params.bittype = 32;
    Engine *engine = new Engine(params, errors);

    Text* text = new Text();

    Plateau* plateau = new Plateau(engine);

    plateau->Generer(3, engine);

    Timer timer;
    Rect rect;
    Input input;

    Color white = {255,255,255,255};
    text->Set("TEST", 24, rect, white);

    // program main loop

    while(engine->run())
    {
        timer.Start();
        input = engine->GetInput();
        plateau->MettreAJour(input, timer.GetDT());
        // DRAWING STARTS HERE

        engine->ClearScreen();

        engine->Draw();  //d'abord dessiner les sprites enregistrés
        plateau->Afficher(engine); //puis seulement les autres
        //engine->Draw(text);
        engine->Flush();
        timer.Stop();

        // DRAWING ENDS HERE
    }// end main loop

    text->~Text();
    plateau->~Plateau();
    engine->~Engine();


    return 0;
}
