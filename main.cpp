#include "SDL/SDL.h"
#include "SDL/SDL_image.h"
#include <string>
#include <fstream>
using namespace std;


const int SIZE_X=5;
const int SIZE_Y=5;

char tablero[SIZE_X][SIZE_Y]={{'_','_','_','#','#'},
                              {'O','#','_','#','#'},
                              {'_','_','_','#','#'},
                              {'#','#','#','#','#'},
                              {'#','#','#','#','#'}
                             };

//Screen attributes
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;
const int SCREEN_BPP = 32;

//The surfaces
SDL_Surface *background = NULL;
SDL_Surface *upMessage = NULL;
SDL_Surface *downMessage = NULL;
SDL_Surface *leftMessage = NULL;
SDL_Surface *rightMessage = NULL;
SDL_Surface *message = NULL;
SDL_Surface *screen = NULL;
SDL_Surface *camino = NULL;
SDL_Surface *obstaculo = NULL;
SDL_Surface *pared = NULL;
SDL_Surface *verde = NULL;

//The event structure
SDL_Event event;

void LeerLaberinto(){
    char lbs;
    ifstream Laberinto("EstructuraLab.txt");

    for(int x = 0; x<5; x++){
        for(int y = 0; y<5; y++){
            Laberinto>>lbs;
            tablero[x][y] = lbs;
        }
    }

    Laberinto.close();
}

SDL_Surface *load_image( std::string filename )
{
    return IMG_Load( filename.c_str() );
}

void apply_surface( int x, int y, SDL_Surface* source, SDL_Surface* destination, SDL_Rect* clip = NULL )
{
    //Holds offsets
    SDL_Rect offset;

    //Get offsets
    offset.x = x;
    offset.y = y;

    //Blit
    SDL_BlitSurface( source, clip, destination, &offset );
}

bool init()
{
    //Initialize all SDL subsystems
    if( SDL_Init( SDL_INIT_EVERYTHING ) == -1 )
    {
        return false;
    }

    //Set up the screen
    screen = SDL_SetVideoMode( SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_BPP, SDL_SWSURFACE );

    //If there was an error in setting up the screen
    if( screen == NULL )
    {
        return false;
    }

    //Initialize SDL_ttf
    /*if( TTF_Init() == -1 )
    {
        return false;
    }*/

    //Set the window caption
    SDL_WM_SetCaption( "Tarea 05 Leer Laberinto-GACD", NULL );

    //If everything initialized fine
    return true;
}

bool load_files()
{
    //Load the background image
    background = load_image( "background.png" );
    camino = load_image("tiles/camino.png");
    obstaculo = load_image("tiles/obstaculo.png");
    pared = load_image("tiles/pared.png");
    verde = load_image("tiles/verde.png");

    if(camino==NULL){
        exit(0);
    }

    //If there was a problem in loading the background
    if( background == NULL )
    {
        return false;
    }
    //If everything loaded fine
    return true;
}

void clean_up()
{
    //Free the surfaces
    SDL_FreeSurface( background );
    SDL_FreeSurface( upMessage );
    SDL_FreeSurface( downMessage );
    SDL_FreeSurface( leftMessage );
    SDL_FreeSurface( rightMessage );


    //Quit SDL
    SDL_Quit();
}

void imprimirTablero()
{
    LeerLaberinto();
    for(int x=0;x<SIZE_X;x++)
    {
        for(int y=0;y<SIZE_X;y++)
        {
            if(tablero[y][x]=='#')
                apply_surface(x*64,y*64,pared,screen,NULL);
            if(tablero[y][x]=='-')
                apply_surface(x*64,y*64,camino,screen,NULL);
            if(tablero[y][x]=='O')
                apply_surface(x*64,y*64,obstaculo,screen,NULL);
        }
    }
}

bool puedoLLegar(int x_inicio,int y_inicio,
                 int x_final, int y_final,
                 char tablero[SIZE_Y][SIZE_X],
                 int pasos)
{
    imprimirTablero();

    apply_surface(x_inicio*64,y_inicio*64,verde,screen,NULL);
    SDL_Flip(screen);
    SDL_Delay(500);


    if(pasos<0)
        return false;

    if(tablero[y_inicio][x_inicio]=='O')
        pasos--;

    if(tablero[y_final][x_final]=='#'
       || tablero[y_inicio][x_inicio]=='#')
        return false;

    if(x_inicio<0 || x_inicio>=SIZE_X
       || y_inicio<0 || y_inicio>=SIZE_Y)
       return false;


    if(x_final<0 || x_final>=SIZE_X
       || y_final<0 || y_final>=SIZE_Y)
       return false;

    if(x_inicio==x_final && y_inicio==y_final)
        return true;

    if(puedoLLegar(x_inicio+1,y_inicio,x_final,y_final,tablero,pasos-1))
        return true;
    if(puedoLLegar(x_inicio-1,y_inicio,x_final,y_final,tablero,pasos-1))
        return true;
    if(puedoLLegar(x_inicio,y_inicio+1,x_final,y_final,tablero,pasos-1))
        return true;
    if(puedoLLegar(x_inicio,y_inicio-1,x_final,y_final,tablero,pasos-1))
        return true;

    return false;
}


int main( int argc, char* args[] )
{


    //Quit flag
    bool quit = false;

    //Initialize
    if( init() == false )
    {
        return 1;
    }

    //Load the files
    if( load_files() == false )
    {
        return 1;
    }

    LeerLaberinto();



    //Apply the background
    apply_surface( 0, 0, background, screen );

    puedoLLegar(0,0,2,0,tablero,8);
    SDL_Flip(screen);
    SDL_Delay(1000);
    //puedoLLegar(0,0,1,2,tablero,3);

//
    //Clean up
    clean_up();

    return 0;
}
