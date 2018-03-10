#include <stdlib.h>
#include <stdio.h>
#include <SDL/SDL.h>
#include <time.h>
#include <SDL/SDL_ttf.h>

//Headers
#include "fonction.h"

//DEFINE DIMENSIONS
#define SIZE 1000
#define WIDTH 1920
#define HEIGHT 1080
#define CASES_PLATEAU 30
#define TAILLE_CASE 30
#define ECART 0
#define ECART_BORD (SIZE - (TAILLE_CASE+ECART)*CASES_PLATEAU)/2

//DEFINE GLOBALE
#define TOURS 50
#define PLAYERS 25
#define NB_OBJET 30
#define INVENTAIRE 3
#define RADAR 1
#define VIDE 0
#define JOUEUR 1
#define OBJET 2
#define MUR 9
#define PORTE 10

//DEFINE JEUX
#define ARME_CHICKEN 1
#define ARME_BANANE 2
#define ARME_POISSON 3
#define POTION_VITESSE 4
#define POTON_VIE 5
#define POTION_FORCE 6
#define FAKE_KEY 7
#define TRUE_KEY 8
#define TAILLE_BITMAP 11

//DEFINE DIRECTION
#define UP 1
#define RIGHT 2
#define LEFT 3
#define DOWN 4

//DEFINE TEXTE
#define SMALL 12
#define MEDIUM 18
#define LARGE 25

TTF_Font* police = NULL;
SDL_Color couleur_texte = {0,0,0};

void InitSDL() //1
{

    freopen("CON", "w", stdout);

    police = TTF_OpenFont("8-bit_wonder.ttf", LARGE);
    VarGame* var_game = NULL;
    var_game = (VarGame*)malloc(sizeof(VarGame));
    if(var_game == NULL){
        fprintf(stderr, "Erreur generation surface : %s\n", SDL_GetError());
        exit(EXIT_FAILURE);
    }
    var_game->background = InitEcran();

    InitGame(var_game);
}

SDL_Surface* InitEcran() //2
{

    //Changer titre du programme
    SDL_WM_SetCaption("Projet S2 - L'Arene", NULL);

    //CHARGEMENT DE L'ECRAN
    SDL_Surface *ecran = NULL;
    ecran = SDL_SetVideoMode(WIDTH,HEIGHT,16, SDL_HWSURFACE | SDL_RESIZABLE | SDL_FULLSCREEN | SDL_DOUBLEBUF);

    if(ecran == NULL) //Si erreur chargement de l'écran
    {

        fprintf(stderr, "Impossible de charger le mode video : %s\n", SDL_GetError());
        exit(EXIT_FAILURE);
    }

    //Colorie le fond d'écran en vert
    SDL_FillRect(ecran, NULL, SDL_MapRGB(ecran->format, 30,141,223));
    SDL_Flip(ecran);

    return ecran;
}

void LoadImageMap(VarGame* var_game){

    var_game->image_map = SDL_LoadBMP("Image/Objet/map.bmp");
    var_game->pos_map.x = ECART_BORD+TAILLE_CASE;
    var_game->pos_map.y = 50+TAILLE_CASE;
    var_game->image_sol = SDL_LoadBMP("Image/Objet/sol.bmp");

}

void InitGame(VarGame* var_game) //4
{

    int i;
    //ALLOCATION DES TABLEAUUX 2D JOUEURS ET OBJETS
    var_game->carte_j = (int**)allocation_carte(CASES_PLATEAU);
    remplirCarteJ(var_game->carte_j, var_game);

    var_game->carte_obj = (int**)allocation_carte(CASES_PLATEAU);

    //ALLOCATION DES TABLEAUX 1D DES JOUEURS ET DES OBJETS
    Joueur* listeJoueur = InitJoueur(var_game->carte_j);
    Objet* listeObjet = InitObjet(var_game->carte_j, var_game->carte_obj);
    //ALLOCATION DU TABLEAU 2D GRAPHIQUE DE LA CARTE

    var_game->carte_graph = InitAreneGraph();
    //ALLOCATION DU TABLEAUX 1D REPRESANTANT L'INVENTAIRE
    creationInventaireGraph(var_game);


    //CHARGMENT DES SPRITES JOUEURS
    var_game->pos_joueur = (SDL_Rect*)malloc(PLAYERS*sizeof(SDL_Rect));
    if(var_game->pos_joueur == NULL)
    {
        fprintf(stderr, "Impossible de charger pos_joueur : %s\n", SDL_GetError());
        exit(EXIT_FAILURE);
    }
    for(i=0; i<PLAYERS; i++)
    {
        var_game->pos_joueur[i].x = 0;
        var_game->pos_joueur[i].y = 0;
        var_game->pos_joueur[i].w = TAILLE_CASE;
        var_game->pos_joueur[i].h = TAILLE_CASE;
    }
    var_game->sprite_joueur = loadImagePerso(var_game->pos_joueur);

    //CHARGEMENT DES SPRITES OBJETS
    var_game->pos_objet = (SDL_Rect*)malloc(TAILLE_BITMAP*sizeof(SDL_Rect));
    if(var_game->pos_objet == NULL)
    {
        fprintf(stderr, "Impossible de charger pos_objet : %s\n", SDL_GetError());
        exit(EXIT_FAILURE);
    }
    for(i=0; i<TAILLE_BITMAP; i++)
    {
        var_game->pos_objet[i].x = 0;
        var_game->pos_objet[i].y = 0;
        var_game->pos_objet[i].w = TAILLE_CASE;
        var_game->pos_objet[i].h = TAILLE_CASE;
    }
    var_game->sprite_objet = loadImageObjet(var_game->pos_objet);

    //CHARGEMENT DE LA MAP IMAGE
    LoadImageMap(var_game);

    //DEMARRAGE DU JEU
    GameV2(var_game, listeJoueur, listeObjet);
}


void GameV2(VarGame* var_game, Joueur* joueur, Objet* objet){

    int i,j;

    joueur[5].inventaire[0] = 5;
    joueur[5].inventaire[1] = 6;

    /**CONFIG TEXTES**/

    //Charge police en size LARGE
    police = TTF_OpenFont("8-bit_wonder.ttf", LARGE);
    SDL_Color couleur_fond = {30,141,223};//COULEUR DU TEXTE

    //TITRE DE L'ARENE
    SDL_Surface* texte = TTF_RenderText_Blended(police, "La Grande Arene", couleur_texte);
    SDL_Rect position = {(var_game->background->w - texte->w) / 2, 10};
    position.w = texte->w;
    position.h = texte->h;
    SDL_BlitSurface(texte, NULL, var_game->background, &position);
    //NUMERO VERSION
    police = TTF_OpenFont("police2.ttf", MEDIUM);

    texte = TTF_RenderText_Blended(police, "Version 1.3", couleur_texte);
    position.x = WIDTH - texte->w;
    position.y = HEIGHT - texte->h;
    SDL_BlitSurface(texte, NULL, var_game->background, &position);

    //Charge la police en SMALL
    InfoPartie info;
    SDL_Surface* texte_info_joueur = NULL;
    police = TTF_OpenFont("8-bit_wonder.ttf", SMALL);

    /**FIN CONFIG TEXTES**/

    //AFFICHE LES CASES
    AffichageCarteGraphV2(var_game, joueur, objet);
    SDL_Flip(var_game->background);

    for(i=0; i<TOURS; i++)
    {

        //AFFICHAGE DU NOMBRE DE TOUR
        free(texte_info_joueur);
        sprintf(info.info_tour, "Tour %d  ", i+1);
        position.x = WIDTH/2;
        position.y = ECART_BORD*2 - 50;
        texte_info_joueur = TTF_RenderText_Shaded(police, info.info_tour, couleur_texte, couleur_fond);
        SDL_BlitSurface(texte_info_joueur, NULL, var_game->background, &position);
        SDL_Flip(var_game->background);

        //FIN

        for(j=0; j<PLAYERS; j++)
        {
            AfficherInventaire(var_game, &joueur[i]);
            //CALCUL DE LA NOUVELLE POS DU JOUEUR J
            NewPosJoueur(var_game->carte_j, var_game->carte_obj, &joueur[j]);

            //REGARDE SI COLLISION AVEC UN OBJET ET AJOUTE L'OBJET DANS INVENTAIRE SI BESOIN
            EventCollisionObjet(var_game->carte_j, var_game->carte_obj, &joueur[j], objet);

            //AFFICHAGE DU JOUEUR QUI JOUE
            sprintf(info.info_joueur, "Le joueur %d joue  ", j+1);
            free(texte_info_joueur);
            texte_info_joueur = TTF_RenderText_Shaded(police, info.info_joueur, couleur_texte, couleur_fond);
            position.x = WIDTH/2;
            position.y = ECART_BORD*2;
            SDL_BlitSurface(texte_info_joueur, NULL, var_game->background, &position);

            //CHARGE LES AFFICHAGES SD
            SDL_BlitSurface(var_game->image_map, NULL, var_game->background, &(var_game->pos_map));
            AffichageCarteGraphV2(var_game, joueur, objet);

            //ATTEND 20ms
            SDL_Delay(30);

            SDL_Flip(var_game->background);//Reload ecran
        }
    }

    //LIBERATION MEMOIRE
    //FreeSurface(var_game, joueur, objet, texte);
    //MET LE PROGRAMME EN ATTENTE
    pause();


}


int** allocation_carte(int taille) //5
{

    int i,j;
    int** map = NULL;
    map = malloc(taille*sizeof(int*));
    if(map == NULL)
    {
        printf("Erreur de creation de la map");
        exit(1);
    }
    else
    {
        for(i=0; i<taille; i++)
        {
            map[i] = malloc(taille*sizeof(int));
            if(map[i] == NULL)
            {
                printf("Erreur de creation de la map, tableau %d non charge", i);
                exit(1);
            }
        }
    }
    //INITIALISE A 0 LES CASES DU TABLEAU
    for(i=0; i<taille; i++)
    {
        for(j=0; j<taille; j++)
        {
            map[i][j] = VIDE;
        }
    }
    return map;
}

void remplirCarteJ(int** carte_joueur, VarGame* var_game){

    int i,j;
    creationPorte(carte_joueur, var_game);
    for(i=0;i<CASES_PLATEAU;i++){
        for(j=0;j<CASES_PLATEAU;j++){
            if(carte_joueur[i][j] != PORTE){
                if( (i == 0 || i == CASES_PLATEAU-1) && (j >= 0 || j <= CASES_PLATEAU-1))
                {
                    carte_joueur[i][j] = MUR;
                }
                else if( (j == 0 || j == CASES_PLATEAU-1) && (i > 0 || i < CASES_PLATEAU-1))
                {
                    carte_joueur[i][j] = MUR;
                }
                else
                {
                    carte_joueur[i][j] = VIDE;
                }

            }
            else{
                carte_joueur[i][j] = PORTE;
            }
            printf("%d", carte_joueur[i][j]);
        }
        printf("\n");
    }

}
void creationPorte(int** carte_joueur, VarGame* var_game){

    int i,j;
    int cote = rand_a_b(0,4);//Choisie un côté de l'arene
    switch(cote){
        case 0://HAUT
            j = 0;
            for(i=TAILLE_CASE/2-1;i<=TAILLE_CASE/2+1;i++){
                carte_joueur[i][j] = PORTE;
            }
        break;
        case 1://DROITE
            i = TAILLE_CASE-1;
            for(j=TAILLE_CASE/2-1;j<=TAILLE_CASE/2+1;j++){
                carte_joueur[i][j] = PORTE;
            }
        break;
        case 2://BAS
            j = TAILLE_CASE-1;
            for(i=TAILLE_CASE/2-1;i<=TAILLE_CASE/2+1;i++){
                carte_joueur[i][j] = PORTE;
            }
        break;
        case 3://GAUCHE
            i = 0;
            for(j=TAILLE_CASE/2-1;j<=TAILLE_CASE/2+1;j++){
                carte_joueur[i][j] = PORTE;
            }
        break;
    }
    var_game->cotePorte = cote;

}
/***FONCTIONS GESTIONS JOUEURS/OBJETS***/

Joueur* InitJoueur(int** map) //6
{

    int i;
    //Creation tableau de type Joueur
    Joueur* joueur = NULL;
    joueur = (Joueur*)malloc((PLAYERS)*sizeof(Joueur));

    if(joueur == NULL)
    {
        printf("Erreur chargement joueur");
        exit(EXIT_FAILURE);
    }

    //Initialisation du tableau
    for(i=0; i<PLAYERS; i++)
    {

        joueur[i].x = 0;
        joueur[i].y = 0;
        joueur[i].numero_player = i;
        joueur[i].inventaire[0] = 0;
        joueur[i].inventaire[1] = 0;
        joueur[i].inventaire[2] = 0;
        joueur[i].vie = 3;
    }
    //DETERMINE POSITION ALEATOIRE DES JOUEURS
    PosJoueurAlea(map, joueur);

    return joueur;//Retourne la liste des joueurs

}

Objet* InitObjet(int** map, int** map_objet) //7
{

    int i;
    //Creation tableau de type Objet
    Objet* objet = NULL;
    objet = (Objet*)malloc((NB_OBJET)*sizeof(Objet));

    if(objet == NULL)
    {
        printf("Erreur chargement joueur");
        exit(EXIT_FAILURE);
    }
    //Initialisation du tableau
    for(i=0; i<(NB_OBJET); i++)
    {

        objet[i].x = 0;
        objet[i].y = 0;
        objet[i].power = 0;
        objet[i].indice = i;
    }

    //DETERMINE POSITION ALEATOIRE DES OBJETS
    PosObjetAlea(map, map_objet, objet);

    for(i=0; i<NB_OBJET; i++)
    {
        map_objet[objet[i].x][objet[i].y] = objet[i].power;
        //printf("objet %d x = %d y = %d pouvoir %d et %d\n", i, objet[i].x, objet[i].y, objet[i].power, map_objet[objet[i].x][objet[i].y]);
    }

    return objet;

}

void PosJoueurAlea(int** map, Joueur* joueur) //8
{

    //Init Variables
    int i;
    int doublon = 0, autour = 0;
    int posX = 0, posY = 0, continuer = 1;

    for(i=0; i<PLAYERS; i++)
    {

        continuer = 1;

        while(continuer != 0)
        {

            doublon = 0;
            autour = 0;
            //Fixe des coordonnées aléatoires pour x et y
            posX = rand_a_b(1,CASES_PLATEAU-1);
            posY = rand_a_b(1,CASES_PLATEAU-1);

            autour = compte_autour(map, posX, posY, JOUEUR, RADAR, CASES_PLATEAU);

            if(map[posX][posY] == JOUEUR)
            {
                doublon = 1;
            }

            //S'il n'y a pas de doublon et qu'il n'y a pas de joueurs autour alors on enregristre ces coordonnées
            if(doublon == 0 && autour == 0)
            {
                joueur[i].x = posX;
                joueur[i].y = posY;
                joueur[i].numero_player = i;
                map[posX][posY] = JOUEUR;
                continuer = 0;
            }
        }
    }
}

void PosObjetAlea(int** map, int** map_objet, Objet* objet) //9
{


    int i;
    int posX = 0, posY = 0;
    int continuer = 1;
    int doublon_with_objet = 0, doublon_with_player = 0, autour = 0;
    int rayon_objet = 3;


    for(i=0; i<(NB_OBJET); i++)
    {

        continuer = 1;
        while(continuer != 0)
        {

            doublon_with_objet = 0;
            doublon_with_player = 0;
            autour = 0;

            //Fixe des coordonnées aléatoires pour x et y
            posX = rand_a_b(1, CASES_PLATEAU-1);
            posY = rand_a_b(1,CASES_PLATEAU-1);

            autour = compte_autour(map_objet, posX, posY, OBJET, rayon_objet, CASES_PLATEAU);

            if(map_objet[posX][posY] == OBJET)
            {
                doublon_with_objet = 1;
            }
            else if(map[posX][posY] == JOUEUR)
            {
                doublon_with_player = 1;
            }
            //printf("objet: %d  posX %d, posY %d autour %d doublon_obj doublon_j %d\n", i, posX, posY, autour, doublon_with_objet, doublon_with_player);
            //S'il n'y a pas de doublon alors on enregristre ces coordonnées
            if(doublon_with_objet == 0 && doublon_with_player == 0 && autour < 1)
            {
                objet[i].x = posX;
                objet[i].y = posY;
                objet[i].power = rand_a_b(1,9);
                map_objet[posX][posY] = OBJET;

                continuer = 0;
            }
        }
    }
}


int compte_autour(int** map, int x, int y, int val, int splash, int nb_case) //10
{

    int i,j, compt = 0;
    for(i=x-splash; i<=x+splash; i++)
    {
        for(j=y-splash; j<=y+splash; j++)
        {
            if((i >= 1 && i < nb_case-1) && (j >= 1 && j < nb_case-1))
            {
                if(map[i][j] == val)
                    compt++;
            }
        }
    }
    return compt;

}

void NewPosJoueur(int** map, int** map_objet, Joueur* joueur) //11
{

    int position_actuelle[2];
    position_actuelle[0] = joueur->x;
    position_actuelle[1] = joueur->y;

    int direction = rand_a_b(1,5);

    switch(direction)
    {

    case UP:
        joueur->y = joueur->y - 1;
        break;
    case DOWN:
        joueur->y = joueur->y + 1;
        break;
    case RIGHT:
        joueur->x = joueur->x + 1;
        break;
    case LEFT:
        joueur->x = joueur->x - 1;
        break;
    }

    if(joueur->x < 2 || joueur->x > CASES_PLATEAU-2 || joueur->y < 2 || joueur->y > CASES_PLATEAU-2)
    {
        joueur->x = position_actuelle[0];
        joueur->y = position_actuelle[1];
    }

    else
    {
        if(map[joueur->x][joueur->y] == JOUEUR)
        {
            EventCollisionJoueur(joueur);
            joueur->x = position_actuelle[0];
            joueur->y = position_actuelle[1];
        }
        else
        {
            map[joueur->x][joueur->y] = JOUEUR;
            map[position_actuelle[0]][position_actuelle[1]] = VIDE;
        }
    }
}

void EventCollisionObjet(int** map, int** map_objet, Joueur* joueur, Objet* objet) //12
{

    if(map_objet[joueur->x][joueur->y] != 0)
    {

        if(map_objet[joueur->x][joueur->y] >= 1 && map_objet[joueur->x][joueur->y] <= 3)
        {
            joueur->inventaire[0] = map_objet[joueur->x][joueur->y];
            printf("joueur %d invenantaire[0] = %d et map_objet[joueur->x][joueur->y] = %d\n", joueur->numero_player, joueur->inventaire[0], map_objet[joueur->x][joueur->y]);
        }
        else if(map_objet[joueur->x][joueur->y] > 3 && map_objet[joueur->x][joueur->y] <= 6)
        {
            joueur->inventaire[1] = map_objet[joueur->x][joueur->y];
            printf("joueur %d invenantaire[1] = %d et map_objet[joueur->x][joueur->y] = %d\n", joueur->numero_player, joueur->inventaire[1], map_objet[joueur->x][joueur->y]);
        }
        else if(map_objet[joueur->x][joueur->y] > 6 && map_objet[joueur->x][joueur->y] <= 8)
        {
            joueur->inventaire[2] = map_objet[joueur->x][joueur->y];
            printf("joueur %d invenantaire[2] = %d et map_objet[joueur->x][joueur->y] = %d\n", joueur->numero_player, joueur->inventaire[2], map_objet[joueur->x][joueur->y]);
        }
        map_objet[joueur->x][joueur->y] = VIDE;

    }
}

void EventCollisionJoueur(Joueur* joueur){

    int adversaire;
    printf("joueur %d et joueur-joueur->numero_player =  %d\n", joueur->numero_player, &joueur-joueur->numero_player);
   // printf("joueur %d\n", joueur[indiceJ].numero_player);
   // adversaire = trouverJoueur(joueur->x, joueur->x, &joueur-joueur->numero_player);
    printf("le joueur %d a rencontre le joueur %d\n", joueur->numero_player, adversaire);



}


/**FIN FONCTIONS GESTIONS JOUEURS / OBJETS***/

/***FONCTIONS GRAPHIQUE***/


//FONCTIONS AFFICHAGE MAP
SDL_Surface*** InitAreneGraph() //1
{

    int i,j;
    SDL_Surface*** carte = NULL;
    carte = (SDL_Surface***)malloc(CASES_PLATEAU*sizeof(SDL_Surface**));
    if(carte == NULL)
    {
        fprintf(stderr, "Erreur allocation surface SDL : %s\n", SDL_GetError());
        exit(EXIT_FAILURE);
    }
    for(i=0; i<CASES_PLATEAU; i++)
    {
        carte[i] = (SDL_Surface**)malloc(CASES_PLATEAU*sizeof(SDL_Surface*));
        if(carte[i] == NULL)
        {
            fprintf(stderr, "Erreur allocation surface SDL : %s\n", SDL_GetError());
            exit(EXIT_FAILURE);
        }
    }
    for(i=0; i<CASES_PLATEAU; i++)
    {
        for(j=0; j<CASES_PLATEAU; j++)
        {
            carte[i][j] = SDL_CreateRGBSurface(SDL_HWSURFACE, TAILLE_CASE, TAILLE_CASE, 16,0,0,0,0);
        }
    }

    return carte;

}

//CHARGES LES IMAGES JOUEURS
SDL_Surface* loadImagePerso(SDL_Rect* position_perso)//2
{

    int i;
    SDL_Surface* tab_image_joueur = SDL_LoadBMP("Image/Perso/sprite_personnage.bmp");
    if(tab_image_joueur == NULL)
    {
        fprintf(stderr, "Impossible de charger le sprite des perso : %s\n", SDL_GetError());
        exit(EXIT_FAILURE);
    }
    for(i=1; i<PLAYERS; i++)
    {
        position_perso[i].x = position_perso[i-1].x + TAILLE_CASE;

    }
    return tab_image_joueur;
}

//CHARGE LES IMAGES OBJETS
SDL_Surface* loadImageObjet(SDL_Rect* position_objet)
{
    int i;
    SDL_Surface* tab_image_objet = SDL_LoadBMP("Image/Objet/sprite_objet.bmp");
    if(tab_image_objet == NULL)
    {
        fprintf(stderr, "Impossible de charger le sprite des perso : %s\n", SDL_GetError());
        exit(EXIT_FAILURE);
    }
    for(i=1;i<TAILLE_BITMAP;i++){
        position_objet[i].x = position_objet[i-1].x + TAILLE_CASE;
    }
    return tab_image_objet;

}

void AffichageCarteGraphV2(VarGame* var_game, Joueur* joueur, Objet* objet){

    int i,j, indiceJ, indiceOb;
    SDL_Rect position = {ECART_BORD,50};
    for(i=0; i<CASES_PLATEAU; i++)
    {

        for(j=0; j<CASES_PLATEAU; j++)
        {
            if(var_game->carte_j[i][j] == MUR){

                  SDL_BlitSurface(var_game->sprite_objet, &(var_game->pos_objet[8]), var_game->background, &position);
                  position.x += ECART + TAILLE_CASE;
            }
            else if(var_game->carte_j[i][j] == PORTE){
                if(var_game->cotePorte == 1 || var_game->cotePorte == 3){
                    SDL_BlitSurface(var_game->sprite_objet, &(var_game->pos_objet[10]), var_game->background, &position);
                }
                else{
                    SDL_BlitSurface(var_game->sprite_objet, &(var_game->pos_objet[9]), var_game->background, &position);
                }
                position.x += ECART + TAILLE_CASE;

            }
            else if(var_game->carte_j[i][j] == JOUEUR)
            {
                indiceJ = trouverJoueur(i,j,joueur);
                SDL_SetColorKey(var_game->sprite_joueur, SDL_SRCCOLORKEY, SDL_MapRGB(var_game->sprite_joueur->format, 255,255,255));
                SDL_BlitSurface(var_game->sprite_joueur, &(var_game->pos_joueur[indiceJ]), var_game->background, &position);
                position.x += ECART + TAILLE_CASE;
            }
            else if(var_game->carte_obj[i][j] >= 1 && var_game->carte_obj[i][j] <= 8)
            {

                indiceOb = trouverObjet(i,j,objet);
                int pouv = objet[indiceOb].power;
                SDL_SetColorKey(var_game->sprite_objet, SDL_SRCCOLORKEY, SDL_MapRGB(var_game->sprite_joueur->format, 255,255,255));
                SDL_BlitSurface(var_game->sprite_objet, &(var_game->pos_objet[pouv-1]), var_game->background, &position);
                position.x += ECART + TAILLE_CASE;
            }
            else if(var_game->carte_j[i][j] == VIDE)
            {
                SDL_BlitSurface(var_game->image_sol, NULL, var_game->background, &position);
                position.x += ECART + TAILLE_CASE;
            }
        }
        position.y += ECART + TAILLE_CASE;
        position.x = ECART_BORD;
    }

}

//FIN AFFICHAGE MAP

//FONCTIONS INVENTAIRES

void creationInventaireGraph(VarGame* var_game)//5
{

    int i;
    var_game->inventaire_graph = (SDL_Surface**)malloc(INVENTAIRE*sizeof(SDL_Surface*));
    if(var_game->inventaire_graph == NULL){
        fprintf(stderr, "Probleme alloc inventaire : %s\n", SDL_GetError());
        exit(EXIT_FAILURE);
    }

    for(i=0; i<INVENTAIRE; i++)
    {
        var_game->inventaire_graph[i] = SDL_CreateRGBSurface(SDL_HWSURFACE, TAILLE_CASE*3, TAILLE_CASE*3, 16, 0,0,0,0);
        SDL_FillRect(var_game->inventaire_graph[i], NULL, SDL_MapRGB(var_game->background->format, 255,255,255));
    }
}

void AfficherInventaire(VarGame* var_game, Joueur* joueur)//6
{

    int i;
    SDL_Rect position = {var_game->background->w /2, 150};
    //RELOAD L'INVENTAIRE
    for(i=0; i<INVENTAIRE; i++)
    {
        SDL_BlitSurface(var_game->inventaire_graph[i], NULL, var_game->background, &position);
        position.x += TAILLE_CASE*3 + TAILLE_CASE;
    }
    SDL_Flip(var_game->background);
    position.x  = var_game->background->w/2;
    position.y = 150;
    for(i=0; i<INVENTAIRE; i++)
    {
        switch(joueur->inventaire[i]){
            case 1:
                SDL_BlitSurface(var_game->sprite_objet, &(var_game->pos_objet[0]), var_game->background, &position);
            break;
            case 2:
                SDL_BlitSurface(var_game->sprite_objet, &(var_game->pos_objet[1]), var_game->background, &position);
            break;
            case 3:
                SDL_BlitSurface(var_game->sprite_objet, &(var_game->pos_objet[2]), var_game->background, &position);
            break;
            case 4:
                SDL_BlitSurface(var_game->sprite_objet, &(var_game->pos_objet[3]), var_game->background, &position);
            break;
            case 5:
                SDL_BlitSurface(var_game->sprite_objet, &(var_game->pos_objet[4]), var_game->background, &position);
            break;
            case 6:
                SDL_BlitSurface(var_game->sprite_objet, &(var_game->pos_objet[5]), var_game->background, &position);
            break;
            case 7:
                SDL_BlitSurface(var_game->sprite_objet, &(var_game->pos_objet[6]), var_game->background, &position);
            break;
            case 8:
                SDL_BlitSurface(var_game->sprite_objet, &(var_game->pos_objet[7]), var_game->background, &position);
            break;
            default:
                SDL_BlitSurface(var_game->inventaire_graph, NULL, var_game->background, &position);
            break;
        }
        position.x += TAILLE_CASE*3 + TAILLE_CASE;
    }
   // SDL_Flip(var_game->background);
}

/***FIN FONCTIONS GRAPHIQUES***/

/***FONCTIONS ANNEXES***/

int rand_a_b(int a, int b) //1
{

    return rand()%(b-a) + a;

}

int trouverJoueur(int posX, int posY, Joueur* joueur)//2
{

    int num = 0, i;
    for(i=0; i<PLAYERS; i++)
    {
        if(posX == joueur[i].x && posY == joueur[i].y)
        {
            num = i;
        }
    }
    return num;
}

int trouverObjet(int posX, int posY, Objet* objet){


    int num = 0, i;
    for(i=0; i<NB_OBJET; i++)
    {
        if(posX == objet[i].x && posY == objet[i].y)
        {
            num = i;
        }
    }
    return num;

}


void pause() //3
{

    int continuer = 1;

    //Crée une variable de type Event
    SDL_Event event;
    //tant que continuer = 1
    while(continuer)
    {
        //On attend que l'utilisateur fasse quelque chose(clique, appuie touche...)
        SDL_PollEvent(&event);
        switch(event.type)
        {
        case SDL_QUIT:
            continuer = 0;
        /*case SDL_KEYDOWN:
            switch (event.key.keysym.sym)
            {
            case SDLK_ESCAPE:
                continuer = 0;
                break;
            }
            break;*/

        }
    }

}


int FreeSurface(VarGame* var_game, Joueur* joueur, Objet* objet, SDL_Surface* texte){

    int i,j;

    for(i=0; i<CASES_PLATEAU; i++)
    {
        for(j=0; j<CASES_PLATEAU; j++)
        {
            SDL_FreeSurface(var_game->carte_graph[i][j]);
            free(var_game->carte_j[i][j]);
            free(var_game->carte_obj[i][j]);
        }
    }
    free(var_game->carte_graph);
    free(var_game->carte_j);
    free(var_game->carte_obj);

    SDL_FreeSurface(texte);

    free(var_game->pos_joueur);
    free(var_game->pos_objet);

    free(objet);
    free(joueur);

    SDL_FreeSurface(var_game->sprite_joueur);
    SDL_FreeSurface(var_game->sprite_objet);
    SDL_FreeSurface(var_game->image_map);
    SDL_FreeSurface(var_game->image_sol);
    SDL_FreeSurface(var_game->background);
    SDL_FreeSurface(var_game->inventaire_graph);

    free(var_game);

    return 0;
}


