// ldd: -lncurses
// ldd: -lncurses
 #include <ncurses.h>
 #include <time.h>
#include <stdlib.h>

int cur_col; // Colonne de la dernière pièce jouée
int cur_line; // Ligne de la dernière pièce jouée
int cur_player; // Joueur courant
char cur_car;

 void INIT_SCREEN (void) {
   initscr();
   raw();
   keypad(stdscr, TRUE);
   noecho();
 }

 void DONE_SCREEN (void) {
   endwin();
   exit(0);
 }

 #define PLAYERS 2
 char CHIP[PLAYERS] = "XO";
 char EMPTY=' '; // Le caractère ' ' signifie qu'il n'y a pas de pièce X ou O
 
#define SIZE 16 // Taille du jeu Puissance 4 en X et Y. Ici les deux tailles sont identiques

char board[SIZE][SIZE];

void init_board()
{
  for (int i=0;i<SIZE;i++)
    for (int j=0;j<SIZE;j++)
      board[i][j]=EMPTY;  // On remplit la matrice de jeu du Puissance 4 ou du vide 
}

// Affichage du plateau de Puissance 4
void draw_board()
{
   // Affichage du tableau du Puissance 4
   for (int i=0;i<SIZE;i++)
   {
     for (int j=0;j<SIZE;j++)
       printw("|%c", board[i][j]);
     printw("|\n");
   }
  // Affichage du base du tableau de puisance 4
  for (int j=0;j<SIZE;j++)
       printw("+-");
  printw("+\n");
  // Affichage des A B C D E F ...
  int dep=(char)'A'; // On commence à partir de la lettre A
  for (int j=0;j<SIZE;j++)
       printw(" %c", (char)(dep+j));
  printw("\n");
}

// Retourne le numéro de la colonne à partir de l'appuie sur une touche clavier A, B, C, D, E, F, ...
int get_col(void)
{
  char ch;
  
  do
  {
    ch = getch();
    if (ch==' ') // A la place du KEY_BACKSPACE
      return -1;
    else if (((int)ch>=(int)'A')&((int)ch<(int)'A'+SIZE))
      return (int)ch - (int)'A'; // Par exemple, si on a appuyé sur B, l'expression retourne 1
    else{}
  }
  while (1);
    
}

// Ajoute un pion dans le puissance 4, sur la colonne col, pour le joueur player
int add_coin(int col, int player)
{
  int line=0;
  if (board[line][col]!=EMPTY) // Si la collonne col est pleine,
    return player; // je retourne player (après ce sera à player de rejouer car le coup n'est pas valide)
  
  while ( (board[line][col]==EMPTY) & (line < SIZE)) // La collone col n'est pas pleine, je vais déterminer la ligne dans laquelle le jeton va être posé, qui est la dernière ligne vide
    line++; // Je vais jusqu'à une ligne son "EMPTY"
  line --; // Et je soustrais 1pour être juste au dessus de la pièce non "EMPTY"
  board[line][col]=CHIP[player]; // Je posela pièce associée au joueur player
  move(line,2*col+1);
  printw("%c",CHIP[player]); // J'écris la pièce à la cordonnée (line, 2*col+1)
  cur_col = col;
  cur_line = line;
  cur_player = player;
  
  return 1 - player; // Je change de player. Dans notre cas, jeu à 2 joueurs.
}

// Je regarde s'il existe 4 pions du joueur player alignés à partir des coordonnées (line, col)
// dans le sens indiqué par (inc_line, inc_col) [horizontal, vertical, en oblique]
// Je répond true si j'ai trouvé 4 pièces de player alignées
bool est_aligne(int player, int line, int col, int inc_line, int inc_col)
{
    if ((col<0)|(col>=SIZE)|(line<0)|(line>=SIZE))
        return false;
    char car=CHIP[player];
    bool pareil = true;
    for (int i=0; i<4; i++)
    {
      line+=inc_line;
      col+=inc_col;
      if ((col<0)|(col>=SIZE)|(line<0)|(line>=SIZE)) // Si je sors du damier, 
        return false;
      if (board[line][col]!=car)
        return false;
 
    }
    return true;    
}

// 
int game_over(void)
{
  bool full=true;
  // Je rearge di le damier n'est pas plein
  for (int col=0;col<SIZE;col++)
    if ( board[0][col] == EMPTY) // Si je trouve une case "EMPTY", ce n'est pas plein
    {
      full = false;
      break;
    }
  if (full)
    return 1; // C'est plein!
  
  // Je regarde si on trouve 4 picèes alignées autour de (cur_line, cur_col)
  for (int i=cur_line-3;i<=cur_line+3; i++)
  {
    for (int j=cur_col-3;j<=cur_col+3;j++)
    {
      if ( est_aligne(cur_player,i,j,1,0) |  est_aligne(cur_player,i,j,-1,0) | est_aligne(cur_player,i,j,0,1) | est_aligne(cur_player,i,j,0,-1) | est_aligne(cur_player,i,j,1,1) | est_aligne(cur_player,i,j,1,-1)| est_aligne(cur_player,i,j,-1,1) | est_aligne(cur_player,i,j,-1,-1))
        return CHIP[cur_player]; // J'ai trouvé 4 pièces alignées du joueur courant cu-player
    }
  }
  return 0; // Le jeu n'est pas terminé, pas 4 picèes alignées.
  
}

void play()
{
  int res;
  int player = 0;
  draw_board(); // J'affiche le jeu vide
 
  move(SIZE+3,0);
  printw("Joueur courant: ");
  do
  {
    move(SIZE+3,15);
    cur_car = CHIP[player];
    printw("%c", cur_car);
    int col=get_col(); // Le jour player joue, à la colonne col
    player = add_coin(col, player); // On rajoute la pièce du joueur player et je passe au joueur suivant si le coup est valide
  }
  while ((res=game_over())==0);
  move(SIZE+4,0);
  printw("*** PARTIE TERMINEE !!!\n");
  if (res == 1)
     printw("*** Personne n'a gagné!");
  else
     printw("Le joueur %c a gagné !", (char)res);
}

int main (void) 
{ 
  INIT_SCREEN(); 
  init_board(); 
  play(); 
  getch(); 
  DONE_SCREEN(); 
}  
