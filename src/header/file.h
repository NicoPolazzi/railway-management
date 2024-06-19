#ifndef FILE_H
#define FILE_H

/*  Restituisce il descrittore del file aperto nella posizione PATHNAME.*/
int open_file(char *pathname);

/*  Restituisce il descrittore del file di log aperto di nome NAME.*/
int open_log(char *name);

/*  Restituisce il descrittore di file contente la mappa ARGV.*/
int open_map(const char *argv);

/*  Scrive il numero VALUE convertito in carattere all'inizio del file
    con descrittore di file FD.*/
void write_int(int fd, int value);

/*  Scrive la stringa STR nel file corrispondente al descrittore FD.*/
void write_string(int fd, char *str);

/*  Scrive la data attuale sul file di log con descrittore di file FD. */
void write_time(int fd);

/*  Restituisce l'intero letto sul file con descrittore FD.*/
int read_int(int fd);

/*  Restituisce la stringa letta sul file con descrittore FD; per farlo
    la funziione legge la grandezza della stringa da allocare in memoria. */
char *read_map(int fd);

/*  Chiude il file associato al descrittore FD.*/
void close_file(int fd);

#endif