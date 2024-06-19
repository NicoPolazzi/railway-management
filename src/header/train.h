#ifndef TRAIN_H
#define TRAIN_H

#include "track.h"
#include "track_segment.h"

typedef struct train{
    char *name;
    int log;
    track *itinerary;
    track *path;
    track_segment *position;
}train; 

/*  Restituisce un puntatore a un treno allocato in memoria popolato con 
    il nome NAME e dal descrittore di file di log associato al treno. */
train *new_train(char *name);

/*  Restituisce un puntatore a un treno allocato in memoria.*/
train *store_train(void);

/*  Funzione che si occupa di ricevere e salvare l'itinerario che il processo 
    figlio-treno PTRAIN dovrà seguire.*/
void receive_itinerary(train *ptrain);

/*  Sposta PTRAIN sul primo segmento della lista e aggiorna il file di log. */
void start_train(train *ptrain);

/*  Sposta PTRAIN sul segmento successivo della lista e aggiorna il file di log.*/
void move_train(train *ptrain);

/*  Scrive sul file di log di PTRAIN che il treno è fermo. */
void lock_train(train *ptrain);

/*  Sposta PTRAIN sull'ultimo segmento della lista e aggiorna il file di log. */
void stop_train(train *ptrain);

/*  Invia il segnale SIGUSR1 al processo padre-treno per indicare la fine
    della propria missione.   */
void kill_after_random_time(void);

/*  Dealloca la memoria di PTRAIN e dei suoi componenti; inoltre chiude il file di log ad esso associato. */
void free_train(train *ptrain);

#endif