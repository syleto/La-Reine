#include <stdlib.h>
#include <stdio.h>
#include <SDL/SDL.h>
#include <time.h>
#include <SDL/SDL_ttf.h>
//#include <SDL/SDL_image.h>

int main ( int argc, char** argv )
{

    srand(time(NULL));

    SDL_Init(SDL_INIT_VIDEO);
    if (SDL_Init(SDL_INIT_VIDEO) == -1)

    {
        fprintf(stderr, "Erreur d'initialisation de la SDL");
        exit(EXIT_FAILURE);

    }

    TTF_Init();
    if (TTF_Init() == -1)

    {
        fprintf(stderr, "Erreur d'initialisation de la SDL");
        exit(EXIT_FAILURE);

    }


    InitSDL();

        //run(ecran, NB_CASES, TAILLE_CASE, ECART, ECART_BORD, SIZE);

    TTF_Quit();

    SDL_Quit();


    return EXIT_SUCCESS;
}




