#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "header/track.h"

track *railway;

/*  Restituisce un puntatore ad un binario allocato in memoria.*/
static track *store_track(void){
    track *ptrack = (track *)malloc(sizeof(track));
    if(ptrack == NULL){
        printf("[ERRORE in store_track() %s:%d] impossibile allocare memoria.\n", __FILE__, __LINE__);
        perror("malloc() fallita");
        exit(EXIT_FAILURE);
    }
    return ptrack;
}

track *new_track(void){
    track *ptrack = store_track();
    ptrack -> next = NULL;
    ptrack -> segment = NULL;
    return ptrack;
}

void add_track_segment(track *ptrack, track_segment *ptrack_segment){
    track *tmptrack = ptrack;
    if(ptrack ->segment == NULL){
        ptrack -> segment = ptrack_segment;
        ptrack -> next = NULL;
    }else{
        while(tmptrack -> next != NULL){
            tmptrack = tmptrack -> next;
        }
        tmptrack -> next = store_track();
        tmptrack -> next -> segment = ptrack_segment;
        tmptrack -> next -> next = NULL;
    }
}

void print_track(track *ptrack){
    track *tmptrack = ptrack;
    if(ptrack -> segment != NULL){
        printf("%s ", tmptrack -> segment -> name);
        while(tmptrack -> next != NULL){
            tmptrack = tmptrack -> next;
            printf("%s ", tmptrack -> segment -> name);
        }
    }else{
        printf("NULL");
    }
    puts(""); 
} 

void free_track(track *ptrack){
    track *tmptrack;
    while(ptrack -> next != NULL){
        tmptrack = ptrack;
        ptrack = ptrack -> next;
        free(tmptrack);
    }
    free(ptrack);
    ptrack = NULL;
}

void new_railway(void){
    track_segment *MA1,*MA2,*MA3,*MA4,*MA5,*MA6,*MA7,*MA8,*MA9,*MA10,*MA11,*MA12,*MA13,*MA14,*MA15,*MA16,*S1,*S2,*S3,*S4,*S5,*S6,*S7,*S8;
    MA1 = new_track_segment("MA1", false);
    MA2 = new_track_segment("MA2", false);
    MA3 = new_track_segment("MA3", false);
    MA4 = new_track_segment("MA4", false);
    MA5 = new_track_segment("MA5", false);
    MA6 = new_track_segment("MA6", false);
    MA7 = new_track_segment("MA7", false);
    MA8 = new_track_segment("MA8", false); 
    MA9 = new_track_segment("MA9", false);
    MA10 = new_track_segment("MA10", false);
    MA11 = new_track_segment("MA11", false);
    MA12 = new_track_segment("MA12", false);
    MA13 = new_track_segment("MA13", false);
    MA14 = new_track_segment("MA14", false);
    MA15 = new_track_segment("MA15", false);
    MA16 = new_track_segment("MA16", false);
    S1 = new_track_segment("S1", true);
    S2 = new_track_segment("S2", true);
    S3 = new_track_segment("S3", true);
    S4 = new_track_segment("S4", true);
    S5 = new_track_segment("S5", true);
    S6 = new_track_segment("S6", true);
    S7 = new_track_segment("S7", true);
    S8 = new_track_segment("S8", true);
    link_track_segments(S1,MA1);
	link_track_segments(MA1,MA2);
	link_track_segments(MA2,MA3);
	link_track_segments(MA3,MA4);
	link_track_segments(MA4,S5);
	link_track_segments(S2,MA5);
	link_track_segments(MA5,MA6);
	link_track_segments(MA6,MA7);
	link_track_segments(MA7,MA3);
	link_track_segments(MA3,MA8);
	link_track_segments(MA8,S6);
	link_track_segments(S3,MA9);
	link_track_segments(MA9,MA10);
	link_track_segments(MA10,MA11);
	link_track_segments(MA11,MA12);
	link_track_segments(MA12,MA13);
	link_track_segments(MA13,S7);
	link_track_segments(S4,MA14);
	link_track_segments(MA14,MA15);
	link_track_segments(MA15,MA16);
	link_track_segments(MA16,MA12);
	link_track_segments(MA12,S8);
    railway = new_track();
    add_track_segment(railway, MA1);
    add_track_segment(railway, MA2);
    add_track_segment(railway, MA3);
    add_track_segment(railway, MA4);
    add_track_segment(railway, MA5);
    add_track_segment(railway, MA6);
    add_track_segment(railway, MA7);
    add_track_segment(railway, MA8);
    add_track_segment(railway, MA9);
    add_track_segment(railway, MA10);
    add_track_segment(railway, MA11);
    add_track_segment(railway, MA12);
    add_track_segment(railway, MA13);
    add_track_segment(railway, MA14);
    add_track_segment(railway, MA15);
    add_track_segment(railway, MA16);
    add_track_segment(railway, S1);
    add_track_segment(railway, S2);
    add_track_segment(railway, S3);
    add_track_segment(railway, S4);
    add_track_segment(railway, S5);
    add_track_segment(railway, S6);
    add_track_segment(railway, S7);
    add_track_segment(railway, S8);
}

track_segment *find_in_railway(char *name){
    track *tmptrack = railway;
    while(tmptrack != NULL){
        if(!strcmp(tmptrack -> segment ->name, name)){
            return tmptrack -> segment;
        }
        tmptrack = tmptrack -> next;
    }
    return NULL;   
}

track *new_itinerary(char *itinerary_in_chars){
    track *itinerary = new_track();
    for(char *segment_name = strtok(itinerary_in_chars, ", "); segment_name != NULL; segment_name = strtok(NULL, ", ")){
        if(!strcmp(segment_name, "--")){
            continue;
        }
        add_track_segment(itinerary, find_in_railway(segment_name));
    }
    return itinerary;
}

void free_railway(void){
    track *tmptrack;
    while(railway -> next != NULL){
        tmptrack = railway;
        railway = railway -> next;
        free_track_segment(tmptrack ->segment);
        free(tmptrack);
    }
    free_track_segment(railway -> segment);
    free(railway);
    railway = NULL;
}
