#include <stdlib.h>
#include <stdio.h>
#include "fonction.h"
#define SIZE 4

#define SIZ 4
#define JOUEURS 20
#define ARMECHICKEN 1
#define ARMEBANANE 2
#define ARMEPOISSON 3
#define POPOVITESSE 4
#define POPOVIE 5
#define POPOFORCE 6

#define CLEV 7
#define CLEF 8
#define SORTIE 9




void remplir_inventaire_vide(Joueur* Joueur)
{
    int j;
    for(j=0;j<4;j++)
        {
            Joueur->inventaire[j]=0;
            if (j==3)
            {
                Joueur->inventaire[j]=3; /// Les trois coeurs !
            }
            switch (j)
            {
            case 0:
                Joueur->Effet[j]=0; /// Le nombre de tour initialisé a 0
                break;
            case 1:
                Joueur->Effet[j]=1; ///Vitesse = 1
                break;
            case 2:
                Joueur->Effet[j]=0;  /// Le nombre de tour initialisé a 0
                break;
            case 3:
                Joueur->Effet[j]=1; /// Force =1
                break;
            }
        }
}


int rand_a_b(int a, int b )/// Fonction pour l'aléatoire
    {
        return rand()%(b-a) +a;
    }






    void remplir_objet(int** map_objet, Joueur* Joueur) /// Joueur => &joueur[j] ici pour que sa modifie en gros
    {
        if (map_objet[Joueur->x][Joueur->y]!=0){
                switch (map_objet[Joueur->x][Joueur->y]){      /// En fonction de la valeur de la map, sa propre place dans l'inventaire
                case 1:
                  Joueur->inventaire[0]= ARMECHICKEN;
                  break;
                case 2:
                  Joueur->inventaire[0]= ARMEBANANE;
                  break;
                case 3:
                  Joueur->inventaire[0]= ARMEPOISSON;
                  break;
                case 4:
                  Joueur->inventaire[1]= POPOVITESSE;
                  break;
                case 5:
                  Joueur->inventaire[1]= POPOVIE;
                  break;
                case 6:
                  Joueur->inventaire[1]= POPOFORCE;
                  break;
                case 7:
                  Joueur->inventaire[2]= CLEV;
                  break;
                case 8:
                  Joueur->inventaire[2]= CLEF;
                  break;
                default:
                  printf("Error objet map");
                  break;
                }
                map_objet[Joueur->x][Joueur->y]=0;
            }}

void utiliser_objet(Joueur* joueur,int choix,Joueur* Cible,int** map_objet,int TOUR)
{
    switch(choix)
    {
        case 1:
            utiliser_arme(joueur,Cible);
            break;
        case 2:
            utiliser_popo(joueur,TOUR);
            break;
        case 3:
            utiliser_cle(joueur,map_objet); /// Si la porte de sortie est noté comme un objet
            break;
        default:
            printf("Pas de choix valide effectué");
            break;
    }
}


void utiliser_arme(Joueur* joueur,Joueur* cible)
{
    if (joueur->inventaire[0]<=3)  ///Qu'il ait bien des armes dans le slot des armes
    {
        if (((joueur->x==cible->x)&((joueur->y==cible->y-1)|(joueur->y==cible->y+1)))|((joueur->y==cible->y)&((joueur->x==cible->x-1)|(joueur->x==cible->x+1))))/// Pour qu'ils soient à une case d'écart !
            {
                switch(joueur->inventaire[0])
                    {

                        case 0:
                            if (rand_a_b(0,100)<=80)  ///Le pourcentage de chance de réussite des poings
                                {
                                    cible->inventaire[3] += -0,5;

                                }
                            break;
                        case ARMECHICKEN:
                            if (rand_a_b(0,100)<=50)  ///Le pourcentage de chance de réussite dd poulet
                                {
                                    cible->inventaire[3] += -2;
                                    joueur->inventaire[0]=0;
                                }
                            break;
                        case ARMEBANANE:
                            if (rand_a_b(0,100)<=70)  ///Le pourcentage de chance de réussite de la banane
                                {
                                    cible->inventaire[3] += -1;  ///1 coeur en moin ici
                                    joueur->inventaire[0]=0;
                                }
                            break;
                        case ARMEPOISSON:
                            if (rand_a_b(0,100)<=60)  ///Le pourcentage de chance de réussite du poisson
                                {
                                    cible->inventaire[3] += -1.5;
                                    joueur->inventaire[0]=0;
                                }
                            break;
                        default:
                            printf("Bah alors ta pas d'arme cool et tu veux te battre ? Rentre chez toi gamin !");
                            break;
                    }

            }
    }
}


void utiliser_popo(Joueur* joueur,int TOUR)
{
    switch(joueur->inventaire[1])
    {
    case 4:
        (joueur->Effet[0])=TOUR;
        joueur->Effet[1]=3;
        break;
    case 5:
        joueur->inventaire[3]+=1;
        break;
    case 6:
        joueur->Effet[2]=TOUR;
        joueur->Effet[3]=30;  /// 30% DE chance de toucher en plus ( A convertir dans les différentes fonctions )
        break;
    default:
        printf("Pas de sirop magique sorry !");
        break;
    }

}

int utiliser_cle(Joueur* joueur,int** map_objet)
{
    if (joueur->inventaire[2]=CLEV)
    {
        if (map_objet[joueur->x][joueur->y]==SORTIE)
        {
            joueur->inventaire[2]=0;
            //map_objet[X0][Y0]=CLEV;   /// Pour que ça reparte au spawn
            return 1;
        }
    }
    else if (joueur->inventaire[2]=CLEF)
    {
        if (map_objet[joueur->x][joueur->y]==SORTIE)
        {
            joueur->inventaire[2]=0;
            //map_objet[X0][Y0]=CLEF;
            printf("Argh ! Mauvaise clé !");
            return 0;
        }
    }
}

