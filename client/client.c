//
// Created by Yibo Wang on 2020/9/8.
//

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
//todo socket
    pthread_create(&sendData, NULL, SendData, NULL);
    pthread_create(&sendACK;, NULL, SendACK, NULL);
    pthread_create(&handleACK, NULL, HandleACK, NULL);
    pthread_create(&retransmit, NULL, Retransmit, NULL);

    gettimeofday(&startAll, NULL);

    pthread_join(sendData, NULL);
    pthread_join(sendACK, NULL);
    pthread_join(handleACK, NULL);
    pthread_join(retransmit, NULL);
}