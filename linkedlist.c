#include <stdio.h>
#include "linkedlist.h"
#include <stdlib.h>
#include <stdbool.h>
#define TRUE 1
#define FALSE 0
//Returns the number of elements in the list.
int LinkedListLength(LinkedList *List)
{
    return List->num_members;
}

//Returns TRUE if the list is empty. Returns FALSE otherwise.
int LinkedListEmpty(LinkedList *List)
{
    if (List->num_members == 0)
    {
        return TRUE;
    }
    else
        return FALSE;
}

//If list is empty, just add obj to the list. Otherwise, add obj after Last(). This function returns TRUE if the operation is performed successfully and returns FALSE otherwise.
int LinkedListAppend(LinkedList *List, void *obj)
{
    if (LinkedListEmpty(List) == TRUE)
    {
        LinkedListElem *elem;
        elem = (LinkedListElem *)malloc(sizeof(LinkedListElem));
        elem->obj = obj;
        List->anchor.next = elem;
        elem->prev = &List->anchor;

        elem->next = &List->anchor;
        List->anchor.prev = elem;
    }
    else
    {
        LinkedListElem *lelem = LinkedListLast(List);
        LinkedListElem *elem;
        elem = (LinkedListElem *)malloc(sizeof(LinkedListElem));
        elem->obj = obj;
        lelem->next = elem;
        elem->prev = lelem;

        elem->next = &List->anchor;
        List->anchor.prev = elem;
    }
    List->num_members = List->num_members + 1;
    return 0;
}
//If list is empty, just add obj to the list. Otherwise, add obj before First(). This function returns TRUE if the operation is performed successfully and returns FALSE otherwise.
int LinkedListPrepend(LinkedList *List, void *obj)
{
    if (LinkedListEmpty(List) == TRUE)
    {
        LinkedListElem *elem;
        elem = (LinkedListElem *)malloc(sizeof(LinkedListElem));
        elem->obj = obj;
        List->anchor.next = elem;
        elem->prev = &List->anchor;

        elem->next = &List->anchor;
        List->anchor.prev = elem;
    }
    else
    {
        LinkedListElem *felem = LinkedListFirst(List);
        LinkedListElem *elem;
        elem = (LinkedListElem *)malloc(sizeof(LinkedListElem));
        elem->obj = obj;
        List->anchor.next = elem;
        elem->prev = &List->anchor;

        elem->next = felem;
        felem->prev = elem;
    }
    List->num_members = List->num_members + 1;
    return 0;
}
//Unlink and delete elem from the list. Please do not delete the object pointed to by elem and do not check if elem is on the list.
void LinkedListUnlink(LinkedList *List, LinkedListElem *unelem)
{
    //LinkedListElem *elem = NULL;
    unelem->next->prev = unelem->prev;
    unelem->prev->next = unelem->next;

    List->num_members = List->num_members - 1;
}
//Unlink and delete all elements from the list and make the list empty. Please do not delete the objects pointed to by the list elements.
void LinkedListUnlinkAll(LinkedList *List)
{
    List->anchor.prev = &List->anchor;
    List->anchor.next = &List->anchor;
    List->num_members = 0;
}
//Insert obj between elem and elem->next. If elem is NULL, then this is the same as Append(). This function returns TRUE if the operation is performed successfully and returns FALSE otherwise. Please do not check if elem is on the list.
int LinkedListInsertAfter(LinkedList *List, void *obj, LinkedListElem *inelem)
{

    LinkedListElem *insertelem;
    insertelem = (LinkedListElem *)malloc(sizeof(LinkedListElem));
    insertelem->obj = obj;
    inelem->next->prev = insertelem;
    insertelem->next = inelem->next;

    inelem->next = insertelem;
    insertelem->prev = inelem;
    List->num_members++;
    return 0;
}
//Insert obj between elem and elem->prev. If elem is NULL, then this is the same as Prepend(). This function returns TRUE if the operation is performed successfully and returns FALSE otherwise. Please do not check if elem is on the list.
int LinkedListInsertBefore(LinkedList *List, void *obj, LinkedListElem *inelem)
{

    LinkedListElem *insertelem;
    insertelem = (LinkedListElem *)malloc(sizeof(LinkedListElem));
    insertelem->obj = obj;
    inelem->prev->next = insertelem;
    insertelem->prev = inelem->prev;

    inelem->prev = insertelem;
    insertelem->next = inelem;
    List->num_members++;
    return 0;
}

//Returns the first list element or NULL if the list is empty.
LinkedListElem *LinkedListFirst(LinkedList *List)
{
    if (LinkedListEmpty(List) == TRUE)
    {
        return NULL;
    }
    else
    {
        LinkedListElem *elem = List->anchor.next;
        return elem;
    }
}
//Returns the last list element or NULL if the list is empty.
LinkedListElem *LinkedListLast(LinkedList *List)
{
    if (LinkedListEmpty(List) == TRUE)
    {
        return NULL;
    }
    else
    {
        LinkedListElem *elem = List->anchor.next;
        while (elem->next != &List->anchor)
        {
            elem = elem->next;
        }
        return elem;
    }
}
//Returns elem->next or NULL if elem is the last item on the list. Please do not check if elem is on the list.
LinkedListElem *LinkedListNext(LinkedList *List, LinkedListElem *elem)
{
    if (LinkedListLast(List) == elem)
    {
        return NULL;
    }
    else
    {
        /*
        LinkedListElem *p = List->anchor.next;
        while(p != elem){
            p = p->next;
        }
        */
        return elem->next;
    }
}
//Returns elem->prev or NULL if elem is the first item on the list. Please do not check if elem is on the list.
LinkedListElem *LinkedListPrev(LinkedList *List, LinkedListElem *elem)
{
    if (LinkedListFirst(List) == elem)
    {
        return NULL;
    }
    else
    {

        return elem->prev;
    }
}
//Returns the list element elem such that elem->obj == obj. Returns NULL if no such element can be found.
LinkedListElem *LinkedListFind(LinkedList *List, void *num)
{
    LinkedListElem *elem = NULL;
    for (elem = LinkedListFirst(List); elem != NULL; elem = LinkedListNext(List, elem))
    {
        if (elem->obj == num)
        {
            return elem;
        }
    }
    return NULL;
}
//Initialize the list into an empty list. Returns TRUE if all is well and returns FALSE if there is an error initializing the list.
int LinkedListInit(LinkedList *List)
{
    List = (LinkedList *)malloc(sizeof(LinkedListElem) + sizeof(int));
    List->num_members = 0;
    List->anchor.next = &List->anchor;
    List->anchor.next = &List->anchor;
    return TRUE;
}
