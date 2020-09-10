#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <unistd.h>
#include <getopt.h>
#include <ctype.h>
#include <stdbool.h>
#include <sys/time.h>
#include <signal.h>

//time of start of transmit
struct timeval startAll;

//send data to receiver
pthread_t sendData;
//send ACK back for every socket received
pthread_t sendACK;
//handle ACK: freeze timer
pthread_t handleACK;
//when a socket time out, retransmit
pthread_t retransmit;


pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cv = PTHREAD_COND_INITIALIZER;


void *SendData(){

}

void *SendACK(){

}

void *HandleACK(){

}

void *Retransmit(){

}


int main(int argc, char *argv[]) {

    char *sizeValue = "1000";
    char *dest = "127.0.0.1";
    char *eptr;
    int  opt;
    long fileSize;
    char destination;
    static struct option long_options[] = {
            {"size",      required_argument,       0,  's' },
            {"destination", required_argument,       0,  'd' },
            {0,          0,                 0,  0   }
    };
    int long_index = 0;
    while((opt = getopt_long_only(argc, argv, "d:s:", long_options, &long_index)) != -1){
        switch(opt){
            case 'd':
                sizeValue = optarg;
                fileSize = strtol(sizeValue,&eptr, 20);
                break;
            case 's':
                dest = optarg;
                destination = *dest;
                break;

            case ':':                           /* error - missing operand */
                fprintf(stderr, "malformed command\nOption -%c requires an operand\n", optopt);
                exit(1);
                break;
            case '?':                           /* error - unknown option */
                fprintf(stderr,"malformed command\n");
                fprintf(stderr,"Usage: %s [-lambda number] [-mu number] [-r number] [-B number] [-P number] [-B number] [-n number]\n", argv[0]);
                exit(1);
                break;
            default:
                fprintf(stderr,"Usage: %s [-destination ip_addr] [-size number]\n", argv[0]);
                exit(1);
        }
    }
//todo socket
    pthread_create(&sendData, NULL, SendData, NULL);
    pthread_create(&sendACK, NULL, SendACK, NULL);
    pthread_create(&handleACK, NULL, HandleACK, NULL);
    pthread_create(&retransmit, NULL, Retransmit, NULL);

    gettimeofday(&startAll, NULL);

    pthread_join(sendData, NULL);
    pthread_join(sendACK, NULL);
    pthread_join(handleACK, NULL);
    pthread_join(retransmit, NULL);
}