#include<stdlib.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include <time.h>
#include <sys/stat.h>

#include"header/file.h"

#define MAPPA1_PATH "segments/MAPPA1"
#define MAPPA2_PATH "segments/MAPPA2"
#define LOG_DIR "log/"

#define MAX_SIZE 200

mode_t mode = S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH;

int open_file(char *pathname){
    int fd;
    fd = open(pathname, O_CREAT| O_RDWR | O_TRUNC , mode);
    if(fd == -1){
        printf("[%s:%d][Error] non posso aprire il file %s.\n", __FILE__, __LINE__, pathname);
        perror("open_file() failed");
        exit(EXIT_FAILURE);
    }
    if(chmod(pathname, mode)){
        printf("[%s:%d][Error] non posso modifcare le politiche di accesso.\n", __FILE__, __LINE__);
        perror("chmod() failed");
        exit(EXIT_FAILURE);
    }
    return fd;
}

int open_log(char *name){
    int fd;
    char pathname[FILENAME_MAX] = LOG_DIR;
    fd = open(strcat(strcat(pathname, name), ".log"), O_RDWR | O_CREAT | O_APPEND | O_TRUNC, mode);
    if(fd == -1){
        printf("[%s:%d][Error] non posso aprire il file di log %s.\n", __FILE__, __LINE__, pathname);
        perror("open_log() failed");
        exit(EXIT_FAILURE);
    }
    return fd;
}

int open_map(const char *argv){
    int fd = -1;
    if(!strcmp(argv, "MAPPA1")){
        fd = open(MAPPA1_PATH, O_RDONLY);
    }else if(!strcmp(argv, "MAPPA2")){
        fd = open(MAPPA2_PATH, O_RDONLY);
    }
    if(fd == -1){
        printf("[ERRORE in open_map() %s:%d] impossibile aprire il file contente gli itinerari.\n", __FILE__, __LINE__);
        perror("open() fallita");
        exit(EXIT_FAILURE);
    }
    return fd;
}

void write_int(int fd, int value){
    if(lseek(fd, 0, SEEK_SET) == -1){
        printf("[Errore in write_int() %s:%d] impossibile spostare l'indicatore nel file.\n", __FILE__, __LINE__);
        perror("lseek() fallita");
        close_file(fd);
        exit(EXIT_FAILURE);
    }
    char cvalue = (char)(value +48);
    if(write(fd, &cvalue, sizeof(cvalue)) != sizeof(cvalue)){
        printf("[Errore in write_int() %s:%d] non posso scrivere intero sul file.\n", __FILE__, __LINE__);
        perror("write() failed");
        close_file(fd);
        exit(EXIT_FAILURE);
    }
}

void write_time(int fd){
    time_t timer = time(NULL);
    if(timer == (time_t)-1){
        printf("[Errore in write_time() %s:%d] impossibile calcolare il tempo.\n", __FILE__, __LINE__);
        perror("time() fallita");
        exit(EXIT_FAILURE);
    }
    struct tm *ptime = localtime(&timer);
    if(ptime == NULL){
        printf("[Errore in write_time() %s:%d] impossibile creare il puntatore alla struttura tm.\n", __FILE__, __LINE__);
        perror("localtime() fallita");
        exit(EXIT_FAILURE);
    }
    char time_str[25];
    if(strftime(time_str, sizeof(time_str), "%d %b %Y %H:%M:%S\n", ptime) == 0){ 
        printf("[Errore in write_time() %s:%d] impossibile aggiungere la data attuale.\n", __FILE__, __LINE__);
        perror("strftime() fallita");
        exit(EXIT_FAILURE);
    }
    write_string(fd, time_str);
}

int read_int(int fd){
    if(lseek(fd, 0, SEEK_SET) == (off_t)-1){
        fputs("Non  posso spostare l'indicatore del file.\n", stderr);
        perror("lseek() fallita");
        close_file(fd);
        exit(EXIT_FAILURE);
    }
    char cvalue[1];
    if(read(fd, cvalue, sizeof(cvalue)) == -1){
        printf("[Errore in read_int() %s:%d] Non posso leggere il numero sul file.\n", __FILE__, __LINE__);
        perror("read() fallita");
        close_file(fd);
        exit(EXIT_FAILURE);
    }
    return (int)(cvalue[0]-48);
}

void write_string(int fd, char *str){
    if(write(fd, str, strlen(str)) == -1){
        printf("[Errore in write_string() %s:%d] Non posso scrivere la stringa nel file.\n", __FILE__, __LINE__);
        perror("write() fallita");
        close_file(fd);
        exit(EXIT_FAILURE);
    }
}
char *read_map(int fd){
    char *str = (char *)calloc(MAX_SIZE, sizeof(char));
    if(str == NULL){
        printf("[ERRORE in read_map() %s:%d] impossibile allocare memoria.\n", __FILE__, __LINE__);
        perror("calloc() fallita");
        close_file(fd);
        exit(EXIT_FAILURE);
    }
    if(read(fd, str, MAX_SIZE) == -1){
        printf("[ERRORE in read_map() %s:%d] impossibile leggere la mappa sul file.\n", __FILE__, __LINE__);
        perror("read() fallita");
        close_file(fd);
        exit(EXIT_FAILURE);   
    }
    str[MAX_SIZE-1] = '\0';
    return str;

}

void close_file(int fd){
    if(close(fd) == -1){
        printf("[ERRORE in close_file() %s:%d] impossibile chiudere il file.\n", __FILE__, __LINE__);
        perror("close() fallita");
        exit(EXIT_FAILURE);
    }
}

