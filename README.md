# La-Reine
Arène de jeu Projet S2 Efrei Paris
Equipe: Lékiptip 
Contexte: Notre arène plonge les joueurs au sein d'une map où le but est d'être le premier à sortir de l'arène ou bien le dernier survivant. ou le meilleur..
Objetctif pour gagner:

  Sortir de l'arène: 
    Pour sortir de l'arène, il faut ouvrir des portes qui se situeront sur les côtés de la map. Pour pouvoir ouvrir la porte il       vous faudra une clé (pour l'instant rien d'anormale). Cependant sur la map, il n'y aura pas une mais plusieurs clés par exemple 4 clés et seulement 1 seule permettra d'ouvrir la porte de l'arène. Ce sera donc aux joueurs de trouver la bonne clé et d'essayer de sortir le plus vite possible.
  
  Dernier survivant:
    Pour gagner le match, il peut aussi être possible d'être le dernier survivant de l'arène. Ainsi pour se faire des armes, potions seront mis à disposition sur la carte pour le plus grand plaisir des combattants.Chaque joueurs disposera aussi de 3 vies au départ.
    
Déroulement de la partie:
  Le match duera 120 tours durant lequel chaque joueur ne pourra effectuer une action par tour. Si au bout de 120 tours, personne n'est sortie de l'arène et qu'il reste des combattants alors le gagnant est celui qui aura fait le plus de kill. Si dans le cas extrème il n'y a pas de mort, l'arène est relancée. 

  
Les objets:
  Chaque joueur a un inventaire de 3 slots pour avoir une arme, potion et une clé. Il ne peut pas avoir plusieurs armes ni plusieurs potions, ni plusieurs clés. Ainsi dès que un joueur prend un objet il perd son ancien objet.
  
  - 3 armes différentes:
   - Chicken Wing = 2 coeurs avec 50% chance de réussir son attaque.
   - Banane = 1 coeur avec  70% de chance de réussir.
   - Poisson = 1.5 coeur avec 60% chance de réussir.
   
  - 3 potions différentes:
    - Potion vitesse = 3 blocks au lieu d’1 pendant 3 tour  
    - Potion Vie = +1 cœur de vie 
    - Potion force = +30% de réussir son coup pendant 5 tours
    
  - les 2 type de clés vrai et fausse
  
  ATTENTION: Si un joueur est tué ou qu'il perd un objet celui-ci retourne à son spawn d'origine.
  
La map:
    La carte de l'arène n'est pas généré aléatoirement mais fait par nos soins pour que chaque joueurs soient placés de telle manière à ce qu'il n'est pas de déséquilibre au début de partie. Les joueurs auront tous les mêmes chances de gagner c'est seulement la stratégie embloyée qui déterminera le gagnant.
   
Les stratégies:
  Comme l'arène à deux façon de gagner, les stratégies sont multiples:
    Par exemple, un joueur pourra avoir tendance à directement prendre les clés et courir vers les portes tout en attrapant les potions de vitesse.
    Une autre stratégie serait d'attraper les meilleurs armes et survivre en attaquant les joueurs les plus faibles. 
    Une autre stratégie serait de prendre de prévoir les coups d'un adversaire si il est à côté d'un tel ou tel objet. 
    
   Notre arène propose des dilemmes pour les jouers car plusieurs joueurs seront amenés à être à côté des mêmes objets et devront prendre les bonnes décisions  
    
Conseils pour développer l'IA:
  Pour développer votre IA vous n'aurez qu'à faire une fonction qui retournera un tableau 1D d'instruction où chaque case représentera une action. Par exemple si je demande à l'IA d'avancer d'une case à droite et que l'action avancer d'une case à droite est la numéro 4 du tableaux, alors dans la fonction j'écris:
  
  instruction[4] = 1; 
  return instruction;
  
  IMPORTANT: au début de chaque fonction le tableau des instructions sera remis à zero et une seule instruction peut être donnée par tour, ainsi toutes les autres valeurs du tableau seront à 0.

prototype de la fonction: ex monIA(int** map, int* instruction);



  
