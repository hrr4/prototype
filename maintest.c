#include "entitymanager.h"
#include <stdlib.h>
/*#include "list.h"*/

int main(void)
{
  EntityManager* entityManager;

  if (!(entityManager = (EntityManager *) malloc(sizeof(EntityManager))))
  {
    printf("Couldn't allocate for EntityManager!\n");
    return -1;
  }

  if (entitymanager_Initialize(entityManager) == -1)
    return -1;
  entitymanager_handleEvents(entityManager);

  free(entityManager->entityList[0]);
  free(entityManager->entityList[1]);
  /* entityManager->entityList = CreateList();
  entityManager->entityList.Head = (Node *) malloc(sizeof(Node)); */

  return 0;
}