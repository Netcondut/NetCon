#include "../include/Engine.h"

void Timer::Start()
{
    start = SDL_GetTicks();
}
void Timer::Stop()
{
    end = SDL_GetTicks();
}
uint32_t Timer::GetDT()
{
    return (end - start);
}

Engine::Engine(WindowParams params, int& errors)
{
    // initialize SDL video
    if ( SDL_Init( SDL_INIT_VIDEO ) < 0 )
    {
        //printf( "Unable to init SDL: %s\n", SDL_GetError() );
        errors = E_FAIL_INIT;
    }
        TTF_Init();
    // make sure SDL cleans up before exit
    atexit(SDL_Quit);

    // create a new window
    screen = SDL_SetVideoMode(params.width, params.height, params.bittype,SDL_HWSURFACE|SDL_DOUBLEBUF);
    if ( !screen )
    {
       // printf("Unable to set 640x480 video: %s\n", SDL_GetError());
        errors = E_FAIL_INIT;
    }
    brun = true;
}
Engine::~Engine()
{
    for(int i = 0; i < sprites.size(); i++)
        sprites[i]->~Sprite();
    if(screen) {SDL_FreeSurface(screen); screen = 0;}

    SDL_Quit();
    TTF_Quit();
}
void Engine::ClearScreen()
{
         // clear screen
    SDL_FillRect(screen, 0, SDL_MapRGB(screen->format, 0, 0, 0));
}
void Engine::Draw()
{


    for(int i = 0; i < sprites.size(); i++)
    {
        if(!sprites[i]->isHidden() && sprites[i]->isValid())
        {
            Rect rect = sprites.at(i)->GetRect();
            SDL_Rect dstrect;
            dstrect.x = rect.x;
            dstrect.y = rect.y;
            dstrect.w = rect.w;
            dstrect.h = rect.h;

            // draw bitmap
            SDL_BlitSurface(sprites.at(i)->bmp, 0, screen, &dstrect);
        }
        if(!sprites[i]->isValid())
            sprites.erase(sprites.begin() + i); //if sprite was destroyed, remove it from the list
    }

}
void Engine::Draw(Sprite* sprite)
{
        if(sprite->isValid())
        {
            Rect rect = sprite->GetRect();
            SDL_Rect dstrect;
            dstrect.x = rect.x;
            dstrect.y = rect.y;
            dstrect.w = rect.w;
            dstrect.h = rect.h;

            // draw bitmap
            SDL_BlitSurface(sprite->bmp, 0, screen, &dstrect);
        }
}
void Engine::Draw(Text* text)
{

        if(text->isValid())
        {
          //  CHECKPOINT;
                Rect rect = text->GetRect();
                SDL_Rect rcDest = { rect.w , rect.h, rect.x,rect.y };
                SDL_BlitSurface( text->sText,0, screen,&rcDest );
        }
}
void Engine::Flush()
{
        SDL_Flip(screen);
}
void Engine::RegisterSprite(Sprite* sprite)
{
        sprites.push_back(sprite);
}
bool Engine::run()
{
    return brun;
}
SDL_Surface* Engine::GetScreen()
{
        return screen;
}
Input Engine::GetInput()
{
    Input input = {0,0,false,false,-1};
    SDL_PollEvent(&event);
    switch (event.type)
            {
                // exit if the window is closed
            case SDL_QUIT:
                brun = false;
                break;

                // check for keypresses
            case SDL_KEYDOWN:
                    // exit if ESCAPE is pressed
                    if (event.key.keysym.sym == SDLK_ESCAPE)
                        brun = false;

                    input.key = event.key.keysym.sym;
                    break;

            case SDL_MOUSEMOTION:

                input.x = event.button.x;
                input.y = event.button.y;
                break;

            case SDL_MOUSEBUTTONDOWN:

                input.x = event.button.x;
                input.y = event.button.y;

                if( event.button.button == SDL_BUTTON_LEFT )
                {
                    input.lbutton = true;
                }
                if( event.button.button == SDL_BUTTON_RIGHT )
                {
                    input.rbutton = true;
                }
                break;

        }// end switch
        return input;
}
/******************************************************************************/
Sprite::Sprite(const char* textureName, SDL_Surface* screen, int& errors)
{
    errors = S_OK;
    hide = false;
    rect.x = 0;
    rect.y = 0;
    rect.w = 0;
    rect.h = 0;
    dstrect.x = rect.x;
    dstrect.y = rect.y;
    dstrect.w = rect.w;
    dstrect.h = rect.h;
    //SetRect(rect);
    // load an image
    bmp = SDL_LoadBMP(textureName);
    if (!bmp)
    {
        errors = E_FAIL_LOADIMAGE;
    }
    else
    {
        Uint32 colour = SDL_MapRGBA(screen->format, 0, 0, 0,1);
        SDL_SetColorKey(bmp,SDL_SRCCOLORKEY|SDL_RLEACCEL,colour);
       // bmp = SDL_DisplayFormatAlpha(bmp);
    }
}
Sprite::Sprite(const char* textureName,SDL_Surface* screen, Rect _rect, int& errors)
{
    errors = S_OK;
    // load an image
    bmp = SDL_LoadBMP(textureName);
    SetRect(_rect);

    if (!bmp)
    {
        errors = E_FAIL_LOADIMAGE;
    }
    else
    {
        Uint32 colour = SDL_MapRGBA(screen->format, 0, 0, 0,1);
        SDL_SetColorKey(bmp,SDL_SRCCOLORKEY|SDL_RLEACCEL,colour);
        //bmp = SDL_DisplayFormatAlpha(bmp);
    }
}
Sprite::Sprite(Sprite* sprite)
{
    bmp = sprite->bmp;
    SetRect(sprite->GetRect());
}
Sprite::~Sprite()
{
     // free loaded bitmap
    if(bmp) {SDL_FreeSurface(bmp);}
    bmp = 0;
}
void Sprite::Hide(bool _hide)
{
    hide = _hide;
}
bool Sprite::isHidden()
{
    return hide;
}
bool Sprite::isValid()
{
    return (bmp != 0);
}
void Sprite::SetRect(Rect _rect)
{
    rect = _rect;
    if(rect.w == 0)
    rect.w = bmp->w; //override value
    if(rect.h == 0)
    rect.h = bmp->h; //override value

    dstrect.x = rect.x;
    dstrect.y = rect.y;
    dstrect.w = rect.w;
    dstrect.h = rect.h;
}
Rect Sprite::GetRect()
{
    if(rect.w == 0)
    rect.w = bmp->w; //override value
    if(rect.h == 0)
    rect.h = bmp->h; //override value
    return rect;
}
/*****************************************************/
Text::Text()
{
    sText = 0;


    SDL_Rect rcDest = { 20,50, 0,0 };
    fntArial12 = TTF_OpenFont( "arial.ttf", 12 );
    SDL_Color clrWhite = { 255,255,255, 255 };
    sText = TTF_RenderText_Solid( fntArial12, "", clrWhite );
    TTF_CloseFont(fntArial12);
    //SDL_BlitSurface( sText,NULL, engine->GetScreen(),&rcDest );

}
Text::~Text()
{
    // free loaded bitmap
    if(sText) {SDL_FreeSurface(sText);}
    sText = 0;
}
void Text::Set(char* text, int size, Rect _rect, Color color)
{
    rect = _rect;
    fntArial12 = TTF_OpenFont( "arial.ttf", size );
    SDL_Color clr = { color.r, color.g,color.b, color.a };
    sText = TTF_RenderText_Solid( fntArial12, text, clr );
    TTF_CloseFont(fntArial12);
}
Rect Text::GetRect()
{
    return rect;
}
bool Text::isValid()
{
    return (sText != 0);
}
/**************************************************************/
bool MouseIn(int x, int y, Rect rect)
{
    #if DEBUG
    printf("\n%d --- %d --- %d --- %d --- %d --- %d",x, y, rect.x, rect.y, rect.w, rect.h);
    #endif
    return ( ( x > rect.x ) && ( x < rect.x + rect.w ) && ( y > rect.y ) && ( y < rect.y + rect.h ) );
}
