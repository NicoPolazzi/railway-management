#define _XOPEN_SOURCE 500
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>

#include "header/socket.h"
#include "header/virtual.h"
#include "header/file.h"

#define OPEN 0
#define TAKEN 1
#define RBC_NAME "RBC"

/*  Riceve l'itinerario dal processo REGISTRO e lo salva nella struttura dati
    puntata da PTRAIN. */
static void receive_virtual_itinerary(train *ptrain);

/*  Funzione che stabilisce se PTRAIN abbia compiuto la sua missione e che 
    aggiorna il file di log di RBC FD. 
    La comunicazione con i processi treni avviene tramite la socket con
    descrittore di file SOCKFD.*/
static bool request_RBC(int sockfd, train *ptrain, int fd);

/*  Aggiorna il file di log con descrittore di file LOGFILE_FD con la
    richiesta di PTRAIN e la sua accettazione STATE.*/
static void write_RBC_log(int logfile_fd, train *ptrain, int state);

/*  Valuta il movimento del treno PTRAIN e lo comunica attraverso la
    socket avente descrittore di file SOCKFD.*/
static void move_RBC(int sockfd, train *ptrain);

/*  Manda il pid del processo chiamante sulla socket con descrittore di file SERVER_FD.*/
static void send_pid(int server_fd);

void handle_SIGUSR2(int sig){
    puts("SIGUSR2 ricevuto, posso terminare la mia esecuzione.");
}

void run_RBC(void){
    struct sigaction sa;
    sa.sa_handler = &handle_SIGUSR2;
    sa.sa_flags = SA_RESTART;
    sigaction(SIGUSR2, &sa, NULL);
    train *trains[5];
    trains[0] = new_virtual_train("T1");
    trains[1] = new_virtual_train("T2");
    trains[2] = new_virtual_train("T3");
    trains[3] = new_virtual_train("T4");
    trains[4] = new_virtual_train("T5");
    new_virtual_railway();
    
    for(int i = 0; i<5; ++i){
        receive_virtual_itinerary(trains[i]);
    }
    
    int rbc_serverfd = open_server("RBC");
    int logfile_fd = open_log(RBC_NAME);
    int client_fd, position;
    bool can_proceed = true;
    char *client_name;
    
    while(can_proceed){
        client_fd = accept_connection(rbc_serverfd);
        client_name = socket_read_string(client_fd);
        for(position = 0; strcmp(trains[position]->name, client_name); ++position);
        can_proceed = request_RBC(client_fd, trains[position], logfile_fd);
        close_socket(client_fd);
    }
    
    send_pid(rbc_serverfd);
    pause();
    close_file(logfile_fd);
    close_socket(rbc_serverfd);
    
    for(int i =0; i<5; ++i){
        free_virtual_train(trains[i]);
    }
    
    free_virtual_railway();
}

static void receive_virtual_itinerary(train *ptrain){
    int client_fd = open_client("Registro");
    request_connection(client_fd);
    char *itinerary = socket_read_string(client_fd);
    ptrain ->path = new_virtual_itinerary(itinerary);
    ptrain -> itinerary = ptrain -> path;
    printf("[Treno %s]Itinerario ricevuto.", ptrain -> name);
    print_track(ptrain -> path);
}

static bool request_RBC(int sockfd, train *ptrain, int fd){
    static unsigned int number_of_stations = 0;
    /* Caso in cui si legga l'itinerario nullo */
    if(ptrain -> itinerary-> segment == NULL){ 
        socket_write_int(sockfd, OPEN);
        socket_read_int(sockfd);
        number_of_stations +=2;
        return true;
    }
    char *next_position_name = ptrain -> itinerary -> segment -> name;
    printf("[RBC] richiesta da parte del treno: %s per il segmento: %s.\n", ptrain -> name, next_position_name);

    if(!strncmp(next_position_name, "S\0", 1)){
        write_RBC_log(fd, ptrain, 0);
        move_RBC(sockfd, ptrain);
        number_of_stations++;
    }else{
        if(ptrain -> itinerary -> segment -> fd == OPEN){
            write_RBC_log(fd, ptrain, OPEN);
            move_RBC(sockfd, ptrain);
            
        }else{
            write_RBC_log(fd, ptrain, TAKEN);
            socket_write_int(sockfd, TAKEN);
        }
    }

    printf("[RBC] Richiesta di movimento di %s valutata.\n", ptrain -> name);

    if(number_of_stations == 10){
        return false;
    }
    return true;
}

static void write_RBC_log(int logfile_fd, train *train, int state){
    write_string(logfile_fd, "[TRENO richiedente autorizzazione: ");
    write_string(logfile_fd, train -> name);
    write_string(logfile_fd, "], [SEGMENTO attuale: ");
    
    if(train -> position != NULL){
        write_string(logfile_fd, train -> position -> name );
    }else{
        write_string(logfile_fd, "--");
    }

    write_string(logfile_fd, "], [SEGMENTO richiesto: ");
    
    if(train -> itinerary != NULL){
        write_string(logfile_fd, train -> itinerary ->  segment -> name);
    }else{
        write_string(logfile_fd, "--");
    }
    
    if(state == 0){
        write_string(logfile_fd, "], [AUTORIZZATO: si] ");
    }else{
        write_string(logfile_fd, "], [AUTORIZZATO: no] ");
    }
    write_time(logfile_fd);
}

static void move_RBC(int sockfd, train *ptrain){
    socket_write_int(sockfd, OPEN);
    int double_check = socket_read_int(sockfd);
    if(double_check == OPEN){
        if(ptrain -> position != NULL){
        ptrain -> position -> fd = 0;
        }
        ptrain -> position = ptrain -> itinerary -> segment;
        ptrain -> position -> fd = 1;
        ptrain -> itinerary = ptrain -> itinerary -> next;
        printf("[RBC] Richiesta di movimento di %s accettata.\n", ptrain -> name);
    }   
}

static void send_pid(int server_fd){
    int client_fd = accept_connection(server_fd);
    socket_write_int(client_fd, getpid());
    close_socket(client_fd);
}