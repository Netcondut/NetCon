#ifndef ENGINE_H_INCLUDED
#define ENGINE_H_INCLUDED

#define KEY int

#include <SDL.h>
#include "SDL_ttf.h"
#include "SDL_image.h"
#include "errors.h"
#include <vector>


struct WindowParams
{
    int width,height;
    int bittype;
};
struct Rect
{
    int h,w,x,y;
};
struct Color
{
    int r,g,b,a;
};
struct Input
{
    float x,y; //mouse position;
    bool rbutton,lbutton;
    KEY key;
};
class Timer
{
    public :
    void Start();
    void Stop();
    uint32_t GetDT();
    private:
    uint32_t start,end;
};
class Sprite
{
public:
    Sprite(const char* textureName,SDL_Surface* screen, int& errors);
    Sprite(const char* textureName,SDL_Surface* screen, Rect _rect, int& errors);
    Sprite(Sprite* sprite);
    ~Sprite();

    void SetRect(Rect _rect);
    void Hide(bool _hide);
    bool isHidden();
    bool isValid();
    Rect GetRect();

private:
 // centre the bitmap on screen
    bool hide;
    Rect rect;
    SDL_Rect dstrect;
public:
    SDL_Surface* bmp;
};

class Text
{
public:
    Text();
    ~Text();
    bool isValid();
    Rect GetRect();
    void Set(char* text, int size, Rect _rect, Color _color);
  SDL_Surface *sText;



private:
    // Step 1: Load a font and give it a point-size:
    TTF_Font* fntArial12;
    Rect rect;

};

class Engine
{
public:
    Engine(WindowParams params, int& errors);
    ~Engine();
    void RegisterSprite(Sprite* sprite);
    void ClearScreen();
    void Draw();
    void Draw(Sprite* sprite);
    void Draw(Text* text);
    void Flush();
    Input GetInput();
    bool run();
    SDL_Surface* GetScreen();
private:
    SDL_Surface* screen;
    std::vector<Sprite*> sprites;
    SDL_Event event;
    bool brun;
};

bool MouseIn(int x, int y, Rect rect);
#endif // ENGINE_H_INCLUDED
