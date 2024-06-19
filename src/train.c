#define _XOPEN_SOURCE 500
#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <time.h>
#include <sys/file.h> /* Per flock() */

#include "header/train.h"
#include "header/file.h"
#include "header/socket.h"

/*  Aggiorna il file di log del treno puntato da PTRAIN specificando il
    binario attuale, il prossimo binario e la data attuale.*/
static void update_log(train *ptrain);

train *store_train(){
    train *ptrain = (train *)malloc(sizeof(train)); 
    if(ptrain == NULL){
        printf("[Errore in store_train() %s:%d] impossibile allocare memoria per ptrain.\n", __FILE__, __LINE__);
        perror("malloc() fallita");
        exit(EXIT_FAILURE);
    }
    return ptrain;
}

train *new_train(char *name){
    train *ptrain = store_train();
    ptrain -> name = name;
    ptrain -> position = NULL;
    ptrain -> log = open_log(name);
    ptrain -> itinerary = NULL;
    ptrain -> path = NULL;     
    return ptrain;
}

void receive_itinerary(train *ptrain){
    int registro_serverfd = open_client("Registro");
    request_connection(registro_serverfd);
    char *itinerary = socket_read_string(registro_serverfd);
    ptrain ->path = new_itinerary(itinerary);
    ptrain -> itinerary = ptrain -> path;
    free(itinerary);
    printf("[Treno: %s] itinerario ricevuto ", ptrain -> name);
    print_track(ptrain -> path);
}

void start_train(train *ptrain) {
	ptrain ->position = ptrain->itinerary->segment;
	ptrain->itinerary = ptrain->itinerary->next;
    update_log(ptrain);
    printf("[TRENO: %s] partito dalla stazione %s.\n", ptrain -> name, ptrain -> position -> name);
}

void stop_train(train *ptrain) {
    ptrain -> itinerary -> segment ->number_of_trains++;
	write_int(ptrain->position->fd,0);
    ptrain->position = ptrain->itinerary->segment;
	ptrain->itinerary = NULL;
    update_log(ptrain);
	printf("[Treno: %s] arrivato alla stazione %s.\n ",ptrain->name,ptrain->position->name);
}

void lock_train(train *ptrain){
    write_string(ptrain ->log, "Locked -->\t");
    update_log(ptrain);
    printf("[Treno: %s] fermo sul binario %s.\n", ptrain -> name, ptrain -> position -> name);
}

void move_train(train *ptrain){
    if(ptrain -> position ->is_station){
        ptrain -> position -> number_of_trains--;
    }else{
        write_int(ptrain ->position ->fd, 0);
    }
    if(ptrain -> itinerary -> segment -> is_station == false){
        write_int(ptrain -> itinerary -> segment -> fd, 1);
    }

    ptrain->position = ptrain->itinerary->segment;
	ptrain->itinerary = ptrain->itinerary->next;
    update_log(ptrain);
    printf("[Treno: %s] mosso sul binario %s.\n", ptrain -> name, ptrain -> position -> name);
}

static void update_log(train *ptrain){
    write_string(ptrain -> log, "[ATTUALE: ");
    if(ptrain -> position != NULL){
        write_string(ptrain -> log, ptrain -> position -> name);
    }else{
        write_string(ptrain -> log, "--");
    }
    write_string(ptrain -> log, "]\t[PROSSIMO: ");
    if(ptrain -> itinerary != NULL){
        write_string(ptrain -> log, ptrain -> itinerary-> segment-> name);
    }else{
        write_string(ptrain -> log, "--");
    }
    write_string(ptrain -> log, "]");
    write_time(ptrain -> log);
}

void kill_after_random_time(void){
    clock_t seed = clock();
    if(seed == -1){
        printf("[Errore in kill_after_random_time() %s:%d] impossibile calcolare il tempo.\n", __FILE__, __LINE__);
        perror("clock() fallita");
        exit(EXIT_FAILURE);
    }
    srand(seed);
    int r = rand() %10000;
    if(usleep(r) == -1){
        printf("[Errore in kill_after_random_time() %s:%d] impossibile sosprendere l'esecuzione del programma.\n", __FILE__, __LINE__);
        perror("usleep() fallita");
        exit(EXIT_FAILURE);
    }
    if(kill(getppid(), SIGUSR1) == -1){
        printf("[Errore in kill_after_random_time() %s:%d] impossibile inviare il segnale al padre.\n", __FILE__, __LINE__);
        perror("kill() fallita");
        exit(EXIT_FAILURE);
    }
}

void free_train(train *ptrain){
    close_file(ptrain -> log);
    free_track(ptrain -> path);
    free(ptrain);
    ptrain = NULL;
}
