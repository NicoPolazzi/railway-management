#include <stdlib.h>
#include <string.h>

#include "header/ETCS.h"

int main(int argc, char const *argv[]){
    if(argc == 3 && !strcmp(argv[1], "ETCS1")) run_ETCS1(argv[2]);
    if(argc == 3 && !strcmp(argv[1], "ETCS2")) run_ETCS2(argv[2]);
    if(argc == 4 && !strcmp(argv[1], "ETCS2") && !strcmp(argv[2], "RBC")) run_RBC();
    
    return EXIT_SUCCESS;
}
