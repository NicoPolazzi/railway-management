#define _XOPEN_SOURCE 500
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>

#include "header/train.h"
#include "header/socket.h"
#include "header/file.h"

#define TIME_TO_SLEEP 2

static volatile int SIGUSR1_counter = 0;

/*  Funzione che prende come argomento la mappa ARGV contenente gli itinerari dei treni. 
    La funzione genera, tramite una fork(), il processo figlio REGISTRO che si occupa di inviare l'itinerario 
    a ciascun treno e ad RBC tramite una comunicazione client-server.    */
static void fork_REGISTRO(const char *argv);

/*  Funzione che genera i processi figlo-treno.*/
static void fork_trains(void);

/*  Funzione che invia iterativamente le richieste di spostamento del
    treno PTRAIN. A fine percorso, PTRAIN viene deallocato e viene 
    lanciato un segnale SIGUSR1 verso il processo padre-treno. */
static void loop_ETCS2(train *ptrain);

/*  Funzione che valuta il tipo di movimento che il treno PTRAIN dovrà
    eseguire. Restituisce true se PTRAIN è ancora in movimento, false 
    se il treno è arrivato alla stazione finale.  */
static bool move_ETCS2(train *ptrain, int client_fd);

/* Manda una signal SIGUSR2 al processo RBC. Il pid di RBC è ottenuto tramite una socket. */
static void kill_RBC(void);

void signal_handler_ETCS2(int sig){
    switch (sig)
    {
    case SIGUSR1:
        SIGUSR1_counter++;
        break;
    
    case SIGUSR2: 
        break;
    
    default:
        break;
    }
}

void run_ETCS2(const char *argv){
    signal(SIGUSR2, SIG_IGN);
    signal(SIGTERM, SIG_IGN);
    struct sigaction sa = {0};
    sa.sa_handler = &signal_handler_ETCS2;
    sa.sa_flags = SA_RESTART;
    sigaction(SIGUSR1, &sa, NULL);

    new_railway();
    fork_REGISTRO(argv);
    fork_trains();
    sleep(1);

    if(killpg(0,SIGUSR2)==-1){
		printf("\n[ERROR: ETCS2()] %s:%d -> Impossibile far continuare i fork\n",__FILE__,__LINE__);
		perror("[ERROR killpg()]");
		exit(EXIT_FAILURE);
	}
    
    do {} while(SIGUSR1_counter <5);

    if(killpg(0,SIGTERM)==-1){
		printf("\n[ERROR: ETCS2()] %s:%d -> Impossibile far continuare i fork\n",__FILE__,__LINE__);
		perror("[ERROR killpg()]");
		exit(EXIT_FAILURE);
	}
    int status;
    while(wait(&status) > 0){
        if(WIFSIGNALED(status)){
            if(WTERMSIG(status) != SIGTERM){
                printf("[ERRORE in run_ETCS2() %s:%d]Programma terminato con fallimento.\n", __FILE__, __LINE__);
                exit(EXIT_FAILURE);
            }else{
                printf("[PADRE-TRENI] il figlio ha ricevuto il segnale SIGTERM con successo.\n");
            }
        }
    }

    kill_RBC();
    free_railway();
}

static void fork_REGISTRO(const char *argv){
    pid_t pid; 
    pid = fork();
    if(pid == 0){
        int mapfd = open_map(argv);
        char *map = read_map(mapfd);
        char *tmpmap = strdup(map);

        int registro_serverfd = open_server("Registro");

        send_train_itinerary(registro_serverfd, map);
        send_train_itinerary(registro_serverfd, tmpmap);

        close_socket(registro_serverfd);
        close_file(mapfd);
        exit(EXIT_SUCCESS);
    }
    if(pid<0){
        printf("[ERRORE in forkETCS2() %s:%d] impossibile fare altre fork.\n", __FILE__, __LINE__);
        perror("fork() fallita");
        exit(EXIT_FAILURE);    
    }
}

static void fork_trains(void){
    train *trains[5];
    pid_t pid;
    trains[0] = new_train("T1");
    trains[1] = new_train("T2");
    trains[2] = new_train("T3");
    trains[3] = new_train("T4");
    trains[4] = new_train("T5");   
    useconds_t utime_to_wait = 1000;
    for(unsigned short i=0; i<5; ++i, utime_to_wait+=1000){
        pid = fork();
        if(pid ==0){
            signal(SIGTERM, SIG_DFL);
            signal(SIGUSR1, SIG_IGN);
            signal(SIGUSR2, signal_handler_ETCS2);

            usleep(utime_to_wait); /* Mi serve un minimo di ritardo per assicurarmi l'ordine di esecuzione */
            receive_itinerary(trains[i]);
            pause();
            loop_ETCS2(trains[i]);
        }
        if(pid<0){
            printf("[ERRORE in forkETCS2() %s:%d] non posso creare ulteriori figli.\n", __FILE__, __LINE__);
            perror("fork() failed");
            exit(EXIT_FAILURE);
        }
    }
}

static void loop_ETCS2(train *ptrain){
    int client_fd;
    bool can_proceed = true;
    while(can_proceed){
        client_fd = open_client("RBC");
        request_connection(client_fd);
        socket_write_int(client_fd, strlen(ptrain -> name));
        socket_write_string(client_fd, ptrain -> name);
        if(socket_read_int(client_fd) == 0){
                can_proceed = move_ETCS2(ptrain, client_fd);
        }else{
            printf("[Treno: %s] ricevuta la risposta negativa da RBC.\n", ptrain -> name);
            lock_train(ptrain);
        }
        
        close_socket(client_fd);
        sleep(TIME_TO_SLEEP);
    }
    free_train(ptrain);
    kill_after_random_time();
    pause();
}

static bool move_ETCS2(train *ptrain, int client_fd){
    if(ptrain -> itinerary-> segment == NULL){ 
        return false;
    }
    if(ptrain->position==NULL){
		start_train(ptrain);
		return true;
	}
    if(ptrain ->itinerary->next==NULL) {
		stop_train(ptrain);
        return false;
	} 
    if(read_int(ptrain -> itinerary -> segment -> fd) == 0){
        move_train(ptrain);
        socket_write_int(client_fd, 0);
    }else{
        /* Caso discordanza */
        printf("[Treno: %s] secondo check non passato\n", ptrain -> name);
        lock_train(ptrain);
        socket_write_int(client_fd, 1);
    }
    return true;
}

static void kill_RBC(void){
    int client_fd = open_client("RBC");
    request_connection(client_fd);
    pid_t pid = socket_read_int(client_fd);
    close_socket(client_fd);
    kill(pid, SIGUSR2);
}