#include <stdlib.h>

typedef struct 
{
  /* data */
  Node* prev, next;
} Node;

typedef struct 
{
  /* data */
  Node* Head, Tail;
} List;

/*******************************************************************************
  AddNode:
    - Adds a node to the linked list.
    - Optional position parameter to iterate to that pos and insert after.
        * If not specified, stick it in the front.
*******************************************************************************/
List* CreateList(void);
int Destroylist( List* listPtr );

void addNode( Node* nodePtr, int pos = 0);
void removeNode( Node* nodePtr, int pos = 0);

void getNext( Node* nodePtr );
void getPrev( Node* nodePtr );
