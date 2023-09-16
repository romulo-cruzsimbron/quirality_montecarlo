
#include<SDL2/SDL.h>
#include<SDL2/SDL_image.h>
#include<SDL2/SDL_ttf.h>
#include<string>

#define WIDTH 800
#define HEIGHT 600

class Renderer{
//friend class Lattice;
private:
friend class Lattice;
    SDL_Rect rect;
    bool Running;
    SDL_Window* window;
    SDL_Renderer* renderer;
    SDL_Event Event;
    int screenWidth,screenHeight;
    SDL_Rect displayRect;

    SDL_Texture *background;
    SDL_Rect rectBackground;

    TTF_Font *font;
    SDL_Surface *message;
    SDL_Texture *text;
    SDL_Rect textRect;
    SDL_Color textColor;

    std::string data;
    bool typing;
    int timer;
public:
    Renderer();
    int Display();
    int Animate(unsigned int netapas,double T,double C0,double C1,double C2);
    bool OnInit();
    bool LoadContent(int n);
    void Padding(int n);
    void Resizing(int n);
    void OnEvent(SDL_Event*, int n);
    void DrawLattice(int, int **, int [][3]);
    //void OnLoop();
    bool UpdateMsg(std::string);
    void SaveRender(std::string);
    void OnRender(int n, int **lattice, int colors[][3]);
    void Cleanup();
};

