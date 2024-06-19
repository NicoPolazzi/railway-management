#ifndef TRACK_H
#define TRACK_H

#include "track_segment.h"

typedef struct track{
    struct track_segment *segment;
    struct track *next;
} track;

/*  Restituisce un puntatore a un binario allocato in memoria.*/
track *new_track(void);

/*  Aggiunge al binario PTRACK il segmento PTRACK_SEGMENT, allocandolo
    adeguatamente in memoria.*/
void add_track_segment(track *ptrack, track_segment *ptrack_segment);

/* Stampa a schermo i dati contenuti nei segmenti del binario PTRACK.*/
void print_track(track *ptrack);

/*  Dealloca la memoria relativa agli elementi del binario PTRACK.*/
void free_track(track *ptrack);

/*  Genera e collega tra loro i segmenti di binario e li salva tutti
    nella variabile globale railway. */
void new_railway(void);

/*  Restituisce il segmento di binario contenuto nella ferrovia di nome NAME.*/
track_segment *find_in_railway(char *name);

/*  Restituisce l'itinerario costruito a partire dalla stringa che lo rappresenta.  */
track *new_itinerary(char *itinerary_in_chars);

/*  Dealloca la memoria dei segmenti di binario che compongono la ferrovia.*/
void free_railway(void);

#endif