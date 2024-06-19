#define _XOPEN_SOURCE  500
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/un.h>

#include "header/socket.h"

#define MAX_LISTEN 10
#define DEFAULT_PROTOCOL 0
#define TMP_DIR "tmp/"

struct sockaddr_un sockaddr;
struct sockaddr *psockaddr = (struct sockaddr *)&sockaddr;
socklen_t sock_length;

/*  Imposta i parametri dell'indirizzo della socket.*/
static void new_address(const char *name){
    sockaddr.sun_family =  AF_UNIX;
    strcpy(sockaddr.sun_path, name);
    sock_length = sizeof(sockaddr);
}

/*  Restituisce il descrittore di un nuovo socket.*/
static int open_socket(void){
    int sock_fd = socket(AF_UNIX, SOCK_STREAM, DEFAULT_PROTOCOL);
    if(sock_fd == -1){
        printf("[ERRORE in open_socket() %s:%d] impossibile creare la socket.\n", __FILE__, __LINE__);
        perror("socket() fallita");
        exit(EXIT_FAILURE);
    }
    return sock_fd;
}

int open_server(const char *name){
    char path[FILENAME_MAX] = TMP_DIR;
    strcat(path, name);
    new_address(path);
    int sock_fd = open_socket();
    unlink(path);
    if(bind(sock_fd, psockaddr, sock_length) == -1){
        printf("[ERRORE in open_server() %s:%d] impossibile associare l'indirizzo alla socket.\n", __FILE__, __LINE__);
        perror("bind() fallita");
        exit(EXIT_FAILURE);
    }
    if(listen(sock_fd, MAX_LISTEN) == -1){
        printf("[ERRORE in open_server() %s:%d] impossibile impostare la lista di attesa del server.\n", __FILE__, __LINE__);
        perror("listen() fallita");
        exit(EXIT_FAILURE);
    }
    return sock_fd;
}

int open_client(const char *name){
    char path[FILENAME_MAX] = TMP_DIR;
    strcat(path, name);
    new_address(path);
    int sock_fd = open_socket();
    return sock_fd;
}

int accept_connection(int sock_fd){
    int client_fd = accept(sock_fd, psockaddr, &sock_length);
    if(client_fd == -1){
        printf("[ERRORE in accept_connection() %s:%d] impossibile accettare la richiesta di connessione.\n", __FILE__, __LINE__);
        perror("accept() fallita");
        exit(EXIT_FAILURE);
    }
    return client_fd;
}

void request_connection(int sock_fd){
    do{
        usleep(1000);
    }while(connect(sock_fd, psockaddr, sock_length) == -1);
    
}

void socket_write_int(int sock_fd, int number){
    if(write(sock_fd, &number, sizeof(int)) == -1){
        printf("[ERRORE in socket_write_int() %s:%d] impossibile scrivere intero sulla socket.\n", __FILE__, __LINE__);
        perror("write() fallita");
        exit(EXIT_FAILURE);
    }
}

void socket_write_string(int sock_fd, char *str){
    if(write(sock_fd, str, strlen(str)) == -1 ){
        printf("[ERRORE in socket_write_string() %s:%d] impossibile scrivere stringa sulla socket.\n", __FILE__, __LINE__);
        perror("write() fallita");
        exit(EXIT_FAILURE);
    }
}

int socket_read_int(int sock_fd){
    int number = 0;
    if(read(sock_fd, &number, sizeof(int)) == -1){
        printf("[ERRORE in socket_read_int() %s:%d] impossibile leggere intero sulla socket.\n", __FILE__, __LINE__);
        perror("read() fallita");
        exit(EXIT_FAILURE);
    }
    return number;
}

char *socket_read_string(int sock_fd){
    size_t size = socket_read_int(sock_fd);
    char *str = (char *)malloc(sizeof(char) *size +1);
    if(read(sock_fd, str, size) == -1){
        printf("[ERRORE in socket_read_string() %s:%d] impossibile leggere stringa sulla socket.\n", __FILE__, __LINE__);
        perror("read() fallita");
        exit(EXIT_FAILURE);
    }
    str[size] = '\0';
    return str;
}

void send_train_itinerary(int server_fd, char *map){
    int client_fd;
    for(map = strtok(map, "\n"); map != NULL; map = strtok(NULL, "\n")){
            client_fd = accept_connection(server_fd);
            socket_write_int(client_fd, strlen(map));
            socket_write_string(client_fd, map);
            close_socket(client_fd);
    }
    free(map);
}

void close_socket(int sock_fd){
    if(close(sock_fd) == -1){
        printf("[ERRORE in close_socket() %s:%d] impossibile chiudere la socket.\n", __FILE__, __LINE__);
        perror("close() fallita");
        exit(EXIT_FAILURE);
    }
}