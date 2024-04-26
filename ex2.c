#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
// 1. D�finition de la fonction print_message qui affiche un message pass� en argument
void *print_message(void *ptr) {
    char *message = (char *)ptr; // Conversion du pointeur en cha�ne de caract�res
    printf("%s\n", message);     // Affichage du message
    pthread_exit(NULL);          // Fin du thread}
int main() {
    pthread_t thread;                           // Identifiant du thread
    char *message = "Bonjour, je suis un thread !"; // Message � afficher
    // 3. Cr�ation d'un thread en lui passant la fonction print_message et le message comme argument
    if (pthread_create(&thread, NULL, print_message, (void *)message)) {
        fprintf(stderr, "Erreur lors de la cr�ation du thread\n");
        return 1;    }
    // 4. Attente de la fin de l'ex�cution du thread
    if (pthread_join(thread, NULL)) {
        fprintf(stderr, "Erreur lors de l'attente de la fin du thread\n");
        return 1;   }
    return 0; // 6. Fin du programme
}

