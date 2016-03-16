/*
Systemes Informatique LSINF1252
Projet 1 :  Implementation de mymalloc, mycalloc et myfree

AIGRET Julien   8343-1300
VANVYVE Nicolas 6590-1300

MARS 2016
*/
#include <stdio.h>
#include <ctype.h>
#include <unistd.h>
#include "mymalloc.h"

static const size_t MB = 1024*1024; // Taille de notre heap a sa premiere initialisation

static block_header *FLOOR = NULL; // Pointeur qui sera le repere de debut de notre zone de heap
static block_header *limit = NULL; // Pointeur qui sera la fin de notre zone de heap
static block_header *last = NULL; // Pointeur vers le dernier block_header cree

/*
@pre  : Recoit une taille de memoire a allouer dans le heap
@post : Retourne un void * vers une zone de memoire allouee de la taille size arrondie
        au premier multiple de 4 superieur ou egal a size, ou NULL si la size etait
        0 ou qu il n a pas pu allouer de memoire dans le heap.
*/
void *mymalloc (size_t size_asked) {

  if (size_asked == 0) return NULL; // Si on nous demande un zone de taille 0, renvoie NULL

  size_asked = size4(size_asked); // Transforme la taille demandee en un multiple de 4 superieur ou egal

  if (FLOOR == NULL) { // Si c'est le premier appel a mymalloc
    FLOOR = (block_header *) sbrk(0);
    void * err = sbrk(MB+BH_SIZE);
    if (err == (void *) -1) return NULL;
    FLOOR->alloc = 0;
    FLOOR->size = MB;
    limit = (block_header *) FLOOR + MB + BH_SIZE;
    last = FLOOR; // On initialise toutes les variables statiques du programme
  }


  block_header *ptr = FLOOR; // On demarre la recherche au debut du heap
  size_t size_disp = 0; // contient la zone de memoire disponible a partir du ptr
  int best_fit = 0; // Si on trouve une zone qui convient parfaitement
  int fit = 0; // Si on trouve au moins une zone qui convient
  int best_size = 0; // La plus grande zone de memoire libre deja trouvee
  int limit_reached = 0; // Si on atteint le last
  block_header *best_ptr = NULL; // Le meilleur resultat

  while (!best_fit && !limit_reached) {
    size_disp = 0; // On reinitialise le code

    while ((ptr + size_disp) < last && (ptr+size_disp)->alloc == 0 && size_disp != size_asked){
      size_disp += (ptr + size_disp)->size + BH_SIZE; // Tant que la zone de memoire est
      // libre et que la size_disp est differente de la taille demandee, on rajoute la
      // memoire libre a size_disp
    }

    if (ptr == last  || ptr + size_disp == last ) {
      limit_reached = 1; // Signifie qu'on a atteint les limites de la zone assignee
    }

    if (size_disp == size_asked){ // On a trouve une zone de memoire de taille exacte
      best_fit = 1;
      best_size = size_disp;
      best_ptr = ptr;
    }

    if (size_disp > size_asked) { // On a trouve une zone de memoire plus
                                 // grande que ce que demande
      fit = 1;
      if (size_disp > best_size) { // Si on ne trouve pas de zone de taille exacte,
                                   // on separe la plus grande zone de memoire trouvee
        best_ptr = ptr;
        best_size = size_disp;
      }
      ptr += ptr->size; // On checke a partir de chaque block_header
    }

    if (size_disp < size_asked && !limit_reached) { // La taille disponible jusqu'a la
                                                   // prochaine zone allouee est trop petite
      ptr += size_disp + BH_SIZE + (ptr+size_disp)->size; // Donc on se deplace apres la
                                                // zone occupee, en "territoire libre"
    }



  }

  if (!fit) { // Si aucune zone n'a ete trouvee
    if (ptr+size_asked > limit) return NULL; // Si la taille de heap n est pas suffisante,
                                             // on ne l augmente pas et on reourne NULL
    int mem = last->size; // On se souviens de la taille qui restait
                          // entre le last et le limit
    ptr->alloc = 1; // On reserve la zone de memoire
    ptr->size = size_asked; // On lui donne la taille demandee
    last = ptr + BH_SIZE + size_asked; // On deplace le last
    last->alloc = 0; // On marque qu'il est libre
    last->size = mem - (size_asked - size_disp); // On regle sa taille
    return ptr+BH_SIZE; // On retourne le pointeur
  }

  if (best_size != size_asked) { // Si on doit redimensionner une zone de memoire
	  (best_ptr+BH_SIZE+size_asked)->alloc = 0; // Le nouveau BH esst libre
    (best_ptr+BH_SIZE+size_asked)->size = best_size - size_asked - BH_SIZE; // Et on regle
                                                                           // sa taille
  }

  best_ptr->alloc = 1; // On reserve le bloc
  best_ptr->size = size_asked; // On lui met la bonne taille
  return best_ptr+BH_SIZE; // On retourne le bon pointeur

}

/*
@pre  : Recoit une taille de memoire a allouer et initialiser dans le heap
@post : Retourne un void * vers une zone de memoire allouee et initialisee de la taille
         size arrondie au premier multiple de 4 superieur ou egal a size, ou NULL si la
         size etait 0 ou qu il n a pas pu allouer de memoire dans le heap.
*/
void *mycalloc (size_t size){
	void *ptr = mymalloc(size); // On demande a mymalloc de nous fournir le meilleur pointeur
  int *cl_ptr = (int *) ptr; // on le caste en (int *) pour l'initialisation
	size = size4(size); // On arrondi sa taille au plus petit multiple de 4 superieur ou egal a la taille demandee
	if (ptr == NULL) return NULL; // Si mymalloc n'a rien retourne on renvoie NULL
	for (int i =0; i < size; i++) *(cl_ptr+i) = 0; // On initialise la zone de memoire a 0
	return ptr; // On retourne le pointeur initialise
}
/*
@pre  : ptr est un pointeur obtenu via mymalloc ou mycalloc
@post : free() libere l'espace memoire pointe par ptr, qui a ete obtenu
        lors d'un appel anterieur a mymalloc() ou mycalloc().
        Si le pointeur ptr n'a pas ete obtenu par l'un de ces appels,
        ou s'il a deja ete libere avec myfree(ptr), le comportement est indetermine.
        Si ptr est NULL, ou n appartient pas au heap,  aucune tentative de liberation n'a
        lieu.
*/
void myfree(void *ptr) {
  if (ptr==NULL) return; // Si on ne nous fourni pas un pointeur on ne fait rien
  //printf("\nFREE POINTEUR : %p\n", ptr);
  block_header *bh_ptr = ptr-BH_SIZE; // On trouve le bon BH
  //printf("FREE BLOCK_HEADER : %p\n", bh_ptr);
  if (bh_ptr >= limit) return; // Si il n appartient pas a notre heap
  if (bh_ptr < FLOOR) return; // on n y touche pas
  bh_ptr->alloc = 0; // Si il fait bien partie de notre heap, on le libere
}

/*
// Tests intermédiaires

int main(int argc, char const *argv[]) {
  int *ptr = (int *) mymalloc (sizeof(int));
  *ptr = 8;
  printf("Adresse 1 : %p, valeur (8): %i\n", ptr, *ptr );
  char *ptr2 = (char *) mymalloc (1000);
  ptr2[0] = (char) 'B';
  ptr2[1] = (char) 'I';
  ptr2[2] = (char) 'T';
  ptr2[3] = (char) 'E';
  ptr2[4]= (char) '\0';
  printf("Adresse 2 : %p, valeur (BITE): %s\n", ptr2, ptr2 );
  double *ptr3 = (double *) mymalloc (sizeof(double));
  *ptr3 = 13.5;
  printf("Adresse 3 : %p, valeur (13.5): %lf\n", ptr3, *ptr3 );
  long *ptr4 = (long *) mymalloc (sizeof(long));
  *ptr4 = 22;
  printf("Adresse 1  : %p, valeur (8): %i\n", ptr, *ptr );
  printf("Adresse 2  : %p, valeur (BITE): %s\n", ptr2, ptr2 );
  printf("Adresse 3  : %p, valeur (13.5): %lf\n", ptr3, *ptr3 );
  printf("Adresse 4  : %p, valeur (22): %ld\n", ptr4, *ptr4 );
  return 0;
}
*/
