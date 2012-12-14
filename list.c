#include "list.h"

List* CreateList(void)
{
  List* newList;
  if ((newList = (List *) malloc(sizeof(List))) == NULL)
  {
    printf("Failed to allocate a new list!\n");
    return -1;
  }

  return newList;
}

int Destroylist( List* listPtr )
{
  Node* currentNode = listPtr->Head;
  while (currentNode->next)
  {
    if (currentNode->prev != NULL)
      free(currentNode->prev);
    currentNode++;
  }

  free(listPtr);
  return 1;
}

// We need to return the head
//Node* addNode( Node* nodePtr, int pos)
Node* addNode(int pos)
{
  // Goto wherever we need to then, make that the next ptr for the current node.
  /*while (pos-- > 0 || )
  {

  }*/
    Node* insertPosition = (Head + pos);
}

void getNext( Node* nodePtr );
void getPrev( Node* nodePtr );