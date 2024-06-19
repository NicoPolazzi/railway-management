#ifndef SOCKET_H
#define SOCKET_H

/* Crea un server socket, nominandolo NAME e restituendo il descrittore di file del server. */
int open_server(const char *name);

/* Accetta una connesione sulla socket SOCK_FD. Restituisce il descrittore di file relativo alla socket accettata.  */
int accept_connection(int sock_fd);

/* Crea una socket lato client, nominandola NAME. Restituisce il descrittore di file relativo. */
int open_client(const char *name);

/* Richiede una connessione con la socket avente descrittore di file SOCK_FD. */
void request_connection(int sock_fd);

/* Scrive il valore NUMBER sulla socket avente descrittore di file  SOCK_FD. */
void socket_write_int(int sock_fd, int number);

/* Scrive la stringa STR sulla socket avente descrittore di file SOCK_FD. */
void socket_write_string(int sock_fd, char *str);

/* Funzione che restituisce il valore intero contenuto nella socket avente descrittore di file SOCK_FD. */
int socket_read_int(int sock_fd);

/* Restituisce la stringa letta sulla socket con descrittore di file SOCK_FD. */
char *socket_read_string(int sock_fd);

/* Scrive sulla socket accetta sul server con descrittore di file SERVER_FD gli itinerari definiti in MAP. */
void send_train_itinerary(int server_fd, char *map);

/*Chiude la socket avente descrittore di file SOCK_FD. */
void close_socket(int sock_fd);

#endif