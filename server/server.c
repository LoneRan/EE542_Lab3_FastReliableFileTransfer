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


//receive data from sender
pthread_t recData;
//send ACK back for every socket received
pthread_t sendACK;
//rearrange the packet received, check if receive all the data
pthread_t rearrange;

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cv = PTHREAD_COND_INITIALIZER;


void *RecData(){

}

void *SendACK(){

}


void *Rearrange(){

}


int main(int argc, char *argv[]) {
//todo socket
    pthread_create(&revData, NULL, RevData, NULL);
    pthread_create(&sendACK;, NULL, SendACK, NULL);
    pthread_create(&rearrange, NULL, Rearrange, NULL);

    gettimeofday(&startAll, NULL);

    pthread_join(revData, NULL);
    pthread_join(sendACK, NULL);
    pthread_join(rearrange, NULL);

}
