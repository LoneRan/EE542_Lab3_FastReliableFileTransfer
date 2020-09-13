
#include <sys/types.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <netdb.h>
#include <stdio.h>
#include <math.h>
#include <stdbool.h>
#ifndef _LinkedList_H_
#define _LinkedList_H_
#define MAX 2048
#define BUF 1400

struct packetI
{
    uint8_t offset;
    u_int file_size;
    u_int num;
};

struct packetLoss
{
    uint8_t offset;
    int ID;
    unsigned char data[BUF];
};

struct packetA
{
    uint8_t offset;
    char index[20000];
};

struct packet_t
{
    long int ID;
    long int length;
    char data[MAX];
};

typedef struct tagLinkedListElem
{
    void *obj;
    struct tagLinkedListElem *next;
    struct tagLinkedListElem *prev;
} LinkedListElem;

typedef struct tagLinkedList
{
    int num_members;
    LinkedListElem anchor;

    /* You do not have to set these function pointers */
    int (*Length)(struct tagLinkedList *);
    int (*Empty)(struct tagLinkedList *);

    int (*Append)(struct tagLinkedList *, void *);
    int (*Prepend)(struct tagLinkedList *, void *);
    void (*Unlink)(struct tagLinkedList *, LinkedListElem *);
    void (*UnlinkAll)(struct tagLinkedList *);
    int (*InsertBefore)(struct tagLinkedList *, void *, LinkedListElem *);
    int (*InsertAfter)(struct tagLinkedList *, void *, LinkedListElem *);

    LinkedListElem *(*First)(struct tagLinkedList *);
    LinkedListElem *(*Last)(struct tagLinkedList *);
    LinkedListElem *(*Next)(struct tagLinkedList *, LinkedListElem *cur);
    LinkedListElem *(*Prev)(struct tagLinkedList *, LinkedListElem *cur);

    LinkedListElem *(*Find)(struct tagLinkedList *, void *obj);
} LinkedList;

extern int LinkedListLength(LinkedList *);
extern int LinkedListEmpty(LinkedList *);

extern int LinkedListAppend(LinkedList *, void *);
extern int LinkedListPrepend(LinkedList *, void *);
extern void LinkedListUnlink(LinkedList *, LinkedListElem *);
extern void LinkedListUnlinkAll(LinkedList *);
extern int LinkedListInsertAfter(LinkedList *, void *, LinkedListElem *);
extern int LinkedListInsertBefore(LinkedList *, void *, LinkedListElem *);

extern LinkedListElem *LinkedListFirst(LinkedList *);
extern LinkedListElem *LinkedListLast(LinkedList *);
extern LinkedListElem *LinkedListNext(LinkedList *, LinkedListElem *);
extern LinkedListElem *LinkedListPrev(LinkedList *, LinkedListElem *);

extern LinkedListElem *LinkedListFind(LinkedList *, void *);

extern int LinkedListInit(LinkedList *);

#endif /*_LinkedList_H_*/
