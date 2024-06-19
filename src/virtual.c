#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "header/virtual.h"

track *virtual_railway;

/*  Restituisce un puntatore a un segmento di binario virtuale 
    popolato con NAME.*/
static track_segment *new_virtual_segment(char *name);

/*  Libera la memoria relativa al segmento di binario SEGMENT.*/
static void free_virtual_track_segment(track_segment *segment);

train *new_virtual_train(char *name){
    train *ptrain = store_train();
    ptrain -> name = name;
    ptrain -> position = NULL;
    ptrain -> log = 0;
    ptrain -> itinerary = NULL;      
    return ptrain;
}

void new_virtual_railway(void){
    track_segment *S1,*S2,*S3,*S4,*S5,*S6,*S7,*S8,*MA1,*MA2,*MA3,*MA4,*MA5,*MA6,*MA7,*MA8,*MA9,*MA10,*MA11,*MA12,*MA13,*MA14,*MA15,*MA16;
    S1 = new_virtual_segment("S1");
    S2 = new_virtual_segment("S2");
    S3 = new_virtual_segment("S3");
    S4 = new_virtual_segment("S4");
    S5 = new_virtual_segment("S5");
    S6 = new_virtual_segment("S6");
    S7 = new_virtual_segment("S7");
    S8 = new_virtual_segment("S8");
    MA1 = new_virtual_segment("MA1");
    MA2 = new_virtual_segment("MA2");
    MA3 = new_virtual_segment("MA3");
    MA4 = new_virtual_segment("MA4");
    MA5 = new_virtual_segment("MA5");
    MA6 = new_virtual_segment("MA6");
    MA7 = new_virtual_segment("MA7");
    MA8 = new_virtual_segment("MA8");
    MA9 = new_virtual_segment("MA9");
    MA10 = new_virtual_segment("MA10");
    MA11 = new_virtual_segment("MA11");
    MA12 = new_virtual_segment("MA12");
    MA13 = new_virtual_segment("MA13");
    MA14 = new_virtual_segment("MA14");
    MA15 = new_virtual_segment("MA15");
    MA16 = new_virtual_segment("MA16");
    virtual_railway = new_track();
    add_track_segment(virtual_railway, MA1);
    add_track_segment(virtual_railway, MA2);
    add_track_segment(virtual_railway, MA3);
    add_track_segment(virtual_railway, MA4);
    add_track_segment(virtual_railway, MA5);
    add_track_segment(virtual_railway, MA6);
    add_track_segment(virtual_railway, MA7);
    add_track_segment(virtual_railway, MA8);
    add_track_segment(virtual_railway, MA9);
    add_track_segment(virtual_railway, MA10);
    add_track_segment(virtual_railway, MA11);
    add_track_segment(virtual_railway, MA12);
    add_track_segment(virtual_railway, MA13);
    add_track_segment(virtual_railway, MA14);
    add_track_segment(virtual_railway, MA15);
    add_track_segment(virtual_railway, MA16);
    add_track_segment(virtual_railway, S1);
    add_track_segment(virtual_railway, S2);
    add_track_segment(virtual_railway, S3);
    add_track_segment(virtual_railway, S4);
    add_track_segment(virtual_railway, S5);
    add_track_segment(virtual_railway, S6);
    add_track_segment(virtual_railway, S7);
    add_track_segment(virtual_railway, S8);
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
}

static track_segment *new_virtual_segment(char *name){
    track_segment *ptrack_segment = store_track_segment();
    ptrack_segment -> name = name;
    ptrack_segment -> near = new_track();
    ptrack_segment -> fd = 0;
    return ptrack_segment; 
}

void free_virtual_train(train *train){
    free_track(train -> path);
    free(train);
    train = NULL;
}

track_segment* find_in_virtual_railway(char *name) {
	track *tmptrack = virtual_railway;
    while(tmptrack != NULL){
        if(!strcmp(tmptrack -> segment ->name, name)){
            return tmptrack -> segment;
        }
        tmptrack = tmptrack -> next;
    }
    return NULL;
}

track *new_virtual_itinerary(char *str){
    track *myitinerary = new_track();
    for(char *segmentname = strtok(str, ", "); segmentname != NULL; segmentname = strtok(NULL, ", ")){
        if(!strcmp(segmentname, "--")){
            continue;
        }
        add_track_segment(myitinerary, find_in_virtual_railway(segmentname));
    }
    return myitinerary;
}

void free_virtual_railway(void){
    track *tmptrack;
    while(virtual_railway ->next != NULL){
        tmptrack = virtual_railway;
        virtual_railway = virtual_railway -> next;
        free_virtual_track_segment(tmptrack -> segment);
        free(tmptrack);
    }
    free_virtual_track_segment(virtual_railway -> segment);
    free(virtual_railway);
    virtual_railway = NULL;
}

static void free_virtual_track_segment(track_segment *segment){
    free_track(segment -> near);
    free(segment);
    segment = NULL;
}