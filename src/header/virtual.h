#ifndef VIRTUALRAILWAY_H
#define VIRTUALRAILWAY_H

#include "train.h"
#include "track.h"
#include "track_segment.h"

/*  Restituisce un puntatore a un treno virtuale allocato in
    memoria e popolato con NAME.*/
train *new_virtual_train(char *name);

/*  Genera e collega tra loro i segmenti di binario virtuali e
    li collega tra di loro.*/
void new_virtual_railway(void);

/*  Restituisce il segmento di binario contenuto nella ferrovia virtuale
    avente nome NAME.*/
track_segment *find_in_virtual_railway(char *name);

/*  Restituisce l'itinerario virtuale costruito a partire dalla stringa che lo rappresenta.  */
track *new_virtual_itinerary(char *itinerary_in_chars);

/*  Libera la memoria relativa al treno virtuale puntato da PTRAIN.*/
void free_virtual_train(train *ptrain);

/*  Dealloca la memoria dei segmenti di binario che compongono la ferrovia virtuale.*/
void free_virtual_railway(void);

#endif