#include <stdio.h>      //les fonctions d'entrée/sortie standard.
#include <stdlib.h>     //les fonctions standard de gestion de la mémoire.
#include <pthread.h>    //les bibliothèques nécessaires pour utiliser les threads en C.
#define ARRAY_SIZE 10   // Définit la taille du tableau.
#define NUM_THREADS 4   // Définit le nombre de threads à utiliser.
int totalSum = 0;   // Variable globale pour stocker la somme totale des éléments du tableau.
typedef struct {             // creation d'une structure.
    int *start;             // Pointeur vers le début de la section du tableau à traiter.
    int *end;               // Pointeur vers la fin de la section du tableau à traiter.
    pthread_mutex_t *lock; // Pointeur vers un verrou pour synchroniser l'accès à la variable globale totalSum.
} PartialSumArgs;
void *sum_partial(void *args) {  // Fonction exécutée par chaque thread pour calculer la somme partielle des éléments du tableau.
    PartialSumArgs *partialArgs = (PartialSumArgs *)args;
    int partialSum = 0;
    int *p;
    for (p = partialArgs->start; p < partialArgs->end; p++) {
        partialSum += *p;  }
    pthread_mutex_lock(partialArgs->lock);  // Verrouille l'accès à la variable globale totalSum.
    totalSum += partialSum;  // Ajoute la somme partielle à la somme totale.
    pthread_mutex_unlock(partialArgs->lock);  // Déverrouille l'accès à la variable globale totalSum.
    pthread_exit(NULL);  // Termine le thread.}
int main() {// Fonction principale du programme.
    int array[ARRAY_SIZE];  // Déclare un tableau d'entiers.
    int i;// Déclare un variable i .
    for (i = 0; i < ARRAY_SIZE; ++i) { // Initialise le tableau avec des valeurs croissantes.
        array[i] = i + 1;    }
    pthread_mutex_t lock;  // Déclare un verrou pour synchroniser l'accès à la variable globale totalSum.
    pthread_mutex_init(&lock, NULL);  // Initialise le verrou.
    pthread_t threads[NUM_THREADS];  // Déclare un tableau de threads.
    PartialSumArgs threadArgs[NUM_THREADS];  // Déclare un tableau d'arguments pour les threads.
    int sectionSize = ARRAY_SIZE / NUM_THREADS;  // Calcule la taille de chaque section du tableau pour chaque thread.
    for (i = 0; i < NUM_THREADS; ++i) {
        // Initialise les arguments pour chaque thread.
        threadArgs[i].start = (array + i * sectionSize);// Pointe vers le début de la section du tableau à traiter.
        // Pointe vers la fin de la section du tableau à traiter.
        threadArgs[i].end = (array + ((i == NUM_THREADS - 1) ? ARRAY_SIZE : (i + 1) * sectionSize));
        threadArgs[i].lock = &lock;  // Passe le verrou à chaque thread.
        // Crée chaque thread pour traiter une section du tableau.
        if (pthread_create(&threads[i], NULL, sum_partial, (void *)&threadArgs[i]) != 0) {
            fprintf(stderr, "Erreur lors de la création du thread %d\n", i);
            return 1;       } }
    // Attend la fin de l'exécution de chaque thread.
    for (i = 0; i < NUM_THREADS; ++i) {
        pthread_join(threads[i], NULL);    }
    // Affiche la somme totale des éléments du tableau.
    printf("Somme totale : %d\n", totalSum);
    pthread_mutex_destroy(&lock);  // Détruit le verrou.
    return 0;  // Termine le programme avec succès.
}
