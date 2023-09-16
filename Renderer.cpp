#include<iostream>

#include "Renderer.h"

#define WIDTH 800
#define HEIGHT 600

using namespace std;

Renderer::Renderer()
{
    window = NULL;
    Running = true;
    message = NULL;
    font = NULL;
    text=NULL;
    textColor.r=0;textColor.g=0;textColor.b=0;//Black
    typing=0;
    data="";
    timer = 0;
}

void Renderer::Padding(int n){
    int square;
    if(screenWidth > screenHeight){
        rect.y = 40+screenHeight/40+5;
        square = screenHeight-rect.y*5/4;
        rect.x = (screenWidth -square)*0.6;
        
    }else{
        rect.x = screenWidth/40+1;
        square = screenWidth-rect.x*5/4;
        rect.y = (screenHeight -square)*.6;
    }
    rect.w = square/n;
    rect.h = square/n;
    //textRect.x = 40;//screenWidth*.1;
    textRect.y = rect.y*.3;
}
            
void Renderer::DrawLattice(int n, int **lattice, int colors[][3]){
    int row = 0,col = 0;
    SDL_Rect block;
    block.w = rect.w;
    block.h = rect.h;
    /* Get the Size of drawing surface */
    for(row=0; row < n; row++){
        for(col=0; col < n; col++){
            int val = lattice[row][col];
            //cout<<"lat: "<<row<<","<<col<<": "<<colors[val][0]<<" "<<colors[val][1]<<" "<<colors[val][2]<<endl;
            SDL_SetRenderDrawColor(renderer, colors[val][0], colors[val][1], colors[val][2], 0);
            block.x = rect.x + col * rect.w;
            block.y = rect.y + row * rect.h;
            SDL_RenderFillRect(renderer, &block);
        }
    }
}

void Renderer::Cleanup()
{
    SDL_FreeSurface(message);
    SDL_DestroyTexture(text);
    //SDL_DestroyTexture(background);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

void Renderer::SaveRender(string str){
    // Create an empty RGB surface that will be used to create the screenshot bmp file
    SDL_Surface* pScreenShot = SDL_CreateRGBSurface(0, screenWidth, screenHeight, 32, 0x00ff0000, 0x0000ff00, 0x000000ff, 0xff000000);

    if(pScreenShot)
    {
        // Read the pixels from the current render target and save them onto the surface
        SDL_RenderReadPixels(renderer, NULL, SDL_PIXELFORMAT_ARGB8888/*SDL_GetWindowPixelFormat(window)*/, pScreenShot->pixels, pScreenShot->pitch);
        // Create the bmp screenshot file
        str = "Lat_"+str + ".png";
        IMG_SavePNG(pScreenShot, str.c_str());
        // Destroy the screenshot surface
        SDL_FreeSurface(pScreenShot);
    }
}

void Renderer::OnRender(int n, int **lattice, int colors[][3])
{
    SDL_SetRenderDrawColor(renderer,255,255,255,0);
    SDL_RenderClear(renderer);
    DrawLattice(n,lattice,colors);
    SDL_RenderCopy(renderer, text, NULL, &textRect);
    SDL_RenderPresent(renderer);
}

bool Renderer::OnInit() {
    if(SDL_Init(SDL_INIT_EVERYTHING) < 0) {
        return false;
    }
    //Creates a SDL Window
    if((window = SDL_CreateWindow("Lattice", 100, 100, WIDTH, HEIGHT, SDL_WINDOW_RESIZABLE | SDL_RENDERER_PRESENTVSYNC)) == NULL) {
        return false;
    }
    //SDL Renderer
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED );
    if (renderer == NULL){
        cout << SDL_GetError() << endl;
        return 1;
    }
    //SDL_SetRenderDrawColor(renderer,255,255,255,0);
    if( TTF_Init() == -1 )
    {
        cout<<"No ttf file!"<<endl;
        return false;
    }
    return true;
}

void Renderer::Resizing(int n){
    SDL_GetRendererOutputSize(renderer,&screenWidth,&screenHeight);
    displayRect.w = screenWidth;
    displayRect.h = screenHeight;
    displayRect.x = displayRect.y = 0;
    Padding(n);
}

void Renderer::OnEvent(SDL_Event* Event, int n) {
    //handle window close
    if(Event->type == SDL_QUIT) {
        Running = false;
    }
    //handle screen resize
    if( Event->type == SDL_WINDOWEVENT )
    {
        switch (Event->window.event){
        case SDL_WINDOWEVENT_RESIZED:
            Resizing(n);
            break;
        }
    }
    //handle key press
    if(Event->type == SDL_KEYDOWN)
    {
        switch(Event->key.keysym.sym)
        {
            case SDLK_d: /*do some thing*/;break;
            case SDLK_a: /*do some thing*/;break;
            case SDLK_ESCAPE: Running=false;break;
        }
    }
    //handle key release
    if(Event->type == SDL_KEYUP)
    {
        switch(Event->key.keysym.sym)
        {
            case SDLK_d: /*do some thing*/;break;
            case SDLK_a: /*do some thing*/;break;

        }
    }
}

bool Renderer::LoadContent(int n)
{
	int w=0,h=0;
	SDL_GetRendererOutputSize(renderer,&screenWidth,&screenHeight);
	displayRect.w = screenWidth;
    displayRect.h = screenHeight;
    displayRect.x = displayRect.y = 0;

	font = TTF_OpenFont( "Tahoma.ttf", 18 );
	if( font == NULL )
	{
		return false;
	}
    Padding(n);
	return 1;
}

bool Renderer::UpdateMsg(string msg){
	int w=0,h=0;
	//string msg = "This is just a short sentence. " + std::to_string(timer)+"s"; 
	message = TTF_RenderText_Solid( font, msg.c_str(), textColor );
	text = SDL_CreateTextureFromSurface(renderer,message);
	SDL_QueryTexture(text, NULL, NULL, &w, &h);
	textRect.w = w;
    textRect.h = h;
    textRect.x = (screenWidth - w)/2;
	return 1;
}

