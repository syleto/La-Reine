#include <stdlib.h>
#include <stdio.h>
#include <SDL/SDL.h>
#include <time.h>
#include <SDL/SDL_ttf.h>

typedef struct Joueur Joueur;
typedef struct Objet Objet;
typedef struct InfoPartie InfoPartie;
typedef struct VarGame VarGame;
struct Joueur
{

    int numero_player;
    int x;
    int y;
    int inventaire[3];
    int vie;
    int Effet[4];

};

struct Objet
{

    int x;
    int y;
    int power;
    int indice;

};



struct InfoPartie
{

    char info_joueur[20];
    char info_tour[20];
   // SDL_Surface* image_perso;

};

struct VarGame{

    SDL_Surface* background;
    SDL_Surface*** carte_graph;
    SDL_Surface** inventaire_graph;
    int** carte_j;
    int** carte_obj;
    SDL_Rect* pos_joueur;
    SDL_Rect* pos_objet;
    SDL_Surface* sprite_joueur;
    SDL_Surface* sprite_objet;
    SDL_Surface* image_map;
    SDL_Surface* image_sol;
    SDL_Rect pos_map;
    int cotePorte;
};




/**FONCTIONS DEMARRAGE**/
void InitSDL();

SDL_Surface* InitEcran();

void InitGame(VarGame* var_game);

/**FONCTIONS ALLOCATIONS**/
int** allocation_carte(int taille);

void remplirCarteJ(int** carte_joueur, VarGame* var_game);

void creationPorte(int** carte_joueur,VarGame* var_game);

SDL_Surface*** InitAreneGraph();

SDL_Surface* loadImagePerso(SDL_Rect* position_perso);//Charge le sprite des personnages

SDL_Surface* loadImageObjet(SDL_Rect* position_objet);//Charge le sprite des objets

void creationInventaireGraph(VarGame* var_game);

Joueur* InitJoueur(int** map);

Objet* InitObjet(int** map, int** map_objet);

void LoadImageMap(VarGame* var_game);//Charge l'image de la map

/**FONCTIONS GESTION POS ALEA**/
void PosJoueurAlea(int** map, Joueur* joueur);

void PosObjetAlea(int** map, int** map_objet, Objet* objet);

int compte_autour(int** map, int x, int y, int val, int splash, int nb_case);


/**FONCTIONS GRAPHIQUE**/

void RemplissageCarteGraphV2(VarGame* var_game);

void AffichageCarteGraphV2(VarGame* var_game, Joueur* joueur, Objet* objet);

void AfficherInventaire(VarGame* var_game, Joueur* joueur);

/**FONCTIONS JEU**/

void GameV2(VarGame* var_game, Joueur* joueur, Objet* objet);

void NewPosJoueur(int** map, int** map_objet, Joueur* joueur);

void EventCollisionObjet(int** map, int** map_objet, Joueur* joueur, Objet* objet);

void EventCollisionJoueur(Joueur* joueur);

/**FONCTIONS ANNEXES**/

int FreeSurface(VarGame* var_game, Joueur* joueur, Objet* objet, SDL_Surface* texte);

int rand_a_b(int a, int b);

int trouverJoueur(int posX, int posY, Joueur* joueur);

int trouverObjet(int posX, int posY, Objet* objet);

void pause();
