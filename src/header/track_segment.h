#ifndef TRACK_SEGMENT_H
#define TRACK_SEGMENT_H

#include <stdbool.h>

typedef struct track_segment{
    char *name;
    int fd;
    struct track *near;
    bool is_station;
    short number_of_trains;
}track_segment;

/*  Restiuisce un puntatore di segmento di binario allocato in memoria popolato
    con nome NAME e specificando con IS_STATION il suo tipo.*/
track_segment *new_track_segment(char *name, bool is_station);

/*  Restituisce un puntatore di segmento di binario allocato dinamicamente
    in memoria.*/
track_segment *store_track_segment(void);

/*  Collega tra di loro FIRST_SEGMENT e SECOND_SEGMENT.*/
void link_track_segments(track_segment *first_segment, track_segment *second_segment);

/*  Dealloca la memoria puntata da PTRACK_SEGMENT.*/
void free_track_segment(track_segment *ptrack_segment);

#endif