#ifndef ETCS_H
#define ETCS_H

/* Funzione che lancia il livello 1 di ETCS prendendo come argomento la mappa ARGV contenente
   gli itinerari che i treni dovranno seguire. */
void run_ETCS1(char const *argv);

/* Funzione che lancia il livello 2 di ETCS prendendo come argomento la mappa ARGV contenente
   gli itinerari che i treni dovranno seguire. */
void run_ETCS2(const char *argv);

/*  Funzione che lancia il server socket RBC. Questo ha il compito di 
    gestire le richieste di spostamento dei processi treni.*/
void run_RBC(void);

#endif