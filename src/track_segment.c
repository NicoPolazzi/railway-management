#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "header/track_segment.h"
#include "header/file.h"
#include "header/track.h"

#define SEGMENT_DIR "segments/" /* Forse si può spostare in file.c */

track_segment *new_track_segment(char *name, bool is_station){
    track_segment *ptrack_segment = store_track_segment();
    char file[FILENAME_MAX] = SEGMENT_DIR;
    ptrack_segment -> name = name;
    ptrack_segment -> near = new_track();
    ptrack_segment -> is_station = is_station;
    if(is_station){
        ptrack_segment -> fd = 0;
    }else{
        ptrack_segment -> fd = open_file(strcat(file,name));
        write_int(ptrack_segment -> fd, 0);
    }
    return ptrack_segment;
}

track_segment *store_track_segment(void){
    track_segment *ptrack_segment = (track_segment *)malloc(sizeof(track_segment));
    if(ptrack_segment == NULL){
        printf("[ERRORE in new_track_segment() %s:%d] impossibile allocare memoria.\n", __FILE__, __LINE__);
        perror("malloc() fallita");
        exit(EXIT_FAILURE);
    }
    return ptrack_segment;
}

void link_track_segments(track_segment *first_segment, track_segment *second_segment){
    add_track_segment(first_segment -> near, second_segment);
    add_track_segment(second_segment -> near, first_segment);
}

void free_track_segment(track_segment *ptrack_segment){
    if(!(ptrack_segment -> is_station))
        close_file(ptrack_segment ->fd);
    free_track(ptrack_segment ->near);
    free(ptrack_segment);
    ptrack_segment = NULL;
}