#define _XOPEN_SOURCE 500
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>
#include <time.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <sys/file.h> /* per flock() */

#include "header/train.h"
#include "header/socket.h"
#include "header/file.h"

#define TIME_TO_SLEEP 2



static volatile sig_atomic_t SIGUSR1_counter = 0;

/*  Funzione che prende come argomento la mappa ARGV contenente gli itinerari dei treni. 
    La funzione genera, tramite una fork(), il processo figlio REGISTRO che si occupa di inviare l'itinerario 
    a ciascun treno tramite una comunicazione client-server.   */
static void fork_REGISTRO(char const *argv);

/*  Funzione che genera i processi figlo-treno.*/
static void fork_trains(void);

/*  Funzione che esegue iterativamente il percorso del treno PTRAIN.
    A fine percorso, PTRAIN viene deallocato e viene lanciato un segnale
    SIGUSR1 verso il processo padre-treno. */
static void loop_ETCS1(train *ptrain);

/*  Funzione che valuta il tipo di movimento che il treno PTRAIN dovrà
    eseguire. Restituisce true se PTRAIN è ancora in movimento, false 
    se il treno è arrivato alla stazione finale.  */
static bool move_ETCS1(train *ptrain);

static void random_sleep(void){
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
}
static void my_signal_handler(int signal){
    switch (signal){
    case SIGUSR1:
        SIGUSR1_counter++;
        break;
    
    case SIGUSR2: 
        puts("segnale SIGUSR2 ricevuto");
        break;
    default:
        break;
    }
}

void run_ETCS1(char const *argv){
    signal(SIGUSR2, SIG_IGN);
    signal(SIGTERM, SIG_IGN);
    struct sigaction sa = {0};
    sa.sa_handler = &my_signal_handler;
    sa.sa_flags = SA_RESTART | SA_NODEFER;
    sigaction(SIGUSR1, &sa, NULL);
    
    new_railway();
    fork_REGISTRO(argv);
    fork_trains();

    if(killpg(0,SIGTERM)==-1){
		printf("[ERRORE in etcs1() %s:%d] Impossibile far continuare i fork\n",__FILE__,__LINE__);
		perror("killpg() fallita");
		exit(EXIT_FAILURE);
    }

    int status;
    while(wait(&status) > 0){
        if(WIFSIGNALED(status)){
            if(WTERMSIG(status) != SIGTERM){
                printf("Programma terminato con fallimento %s:%d\n", __FILE__, __LINE__);
                exit(EXIT_FAILURE);
            }else{
                printf("[PADRE-TRENI] il figlio ha ricevuto il segnale SIGTERM con successo.\n");
            }
        }
    }

    free_railway();
}

static void fork_REGISTRO(char const *argv){
    pid_t pid;
    pid = fork();
    if(pid == 0){
        int registro_fd = open_server("Registro");
        int mapfd = open_map(argv);
        char *map = read_map(mapfd);
        send_train_itinerary(registro_fd, map);
        printf("[REGISTRO] inviato gli itinerari.\n");
        free(map);
        close_file(mapfd);        
        close_socket(registro_fd);
        exit(EXIT_SUCCESS);
    }
    if(pid <0){
        printf("[ERRORE in fork_REGISTRO %s:%d] Impossibile creare il processo REGISTRO.\n", __FILE__, __LINE__);
        perror("fork() fallita");
        exit(EXIT_FAILURE);
    }
}

static void fork_trains(void){
    train *ptrains[5];
    ptrains[0] = new_train("T1");
    ptrains[1] = new_train("T2");
    ptrains[2] = new_train("T3");
    ptrains[3] = new_train("T4");
    ptrains[4] = new_train("T5");
    pid_t pid[5];
    useconds_t time_to_wait = 1000;

    for(unsigned short i=0; i<5; ++i, time_to_wait+=1000){
        pid[i] = fork();
        if(pid[i] == 0){
            signal(SIGUSR2, my_signal_handler);
            signal(SIGUSR1, SIG_IGN);
            signal(SIGTERM, SIG_DFL);
            usleep(time_to_wait); //Mi serve un minimo di ritardo per assicurarmi l'ordine di ricezione
            receive_itinerary(ptrains[i]);
            pause();
            loop_ETCS1(ptrains[i]);          
        }
        if(pid[i] < 0){
            printf("[ERRORE in fork_REGISTRO %s:%d] Impossibile creare il processo REGISTRO.\n", __FILE__, __LINE__);
            perror("fork() fallita");
            exit(EXIT_FAILURE);
        }
    }

    sleep(1);
    if(killpg(0,SIGUSR2)==-1){
		printf("[ERRORE in etcs1() %s:%d] Impossibile far continuare i fork\n",__FILE__,__LINE__);
		perror("killpg() fallita");
		exit(EXIT_FAILURE);
    }

    
    do { 
        
    } while(SIGUSR1_counter < 5);
}

static void loop_ETCS1(train *ptrain){
    bool can_proceed = true;
    while(can_proceed){
        can_proceed = move_ETCS1(ptrain);
        sleep(TIME_TO_SLEEP);
    }
    free_train(ptrain);
    kill_after_random_time();
    pause();
}

static bool move_ETCS1(train *ptrain){
    /* Per il caso in cui si riceva un itinerario vuoto */
    if(ptrain -> itinerary-> segment == NULL){ 
        return false;
    }
    if(ptrain -> position == NULL){
        start_train(ptrain);
        return true;
    }
    if(ptrain -> itinerary -> next == NULL){
        stop_train(ptrain);
        return false;
    }
    
    random_sleep();
    if(read_int(ptrain -> itinerary -> segment -> fd) == 0){
        move_train(ptrain);
    }else{
        lock_train(ptrain);
    }
    return true;
}
