#include "entitymanager.h"

int entitymanager_Initialize( EntityManager* managerPtr )
{
  if (!(managerPtr->entityList[0] = (Entity *) malloc(sizeof(Entity))))
  {
    printf("Couldn't allocate Entity 0!!\n");
    return -1;
  }

  if (!(managerPtr->entityList[1] = (Entity *) malloc(sizeof(Entity))))
  {
    printf("Couldn't allocate Entity 1!!\n");
    return -1;
  }

  printf("Allocated memory for Manager + array\n");
  return 0;
}

void entitymanager_handleEvents( EntityManager* managerPtr )
{
  int i;
  printf("EntityManager (Handle Events): Starting function.\n");
  for (i = 0; i < 2; ++i)
  {
    printf("EntityManager (Handle Events): About to call entity (%i) handle_events.\n", i);
    (*entity_handleEvents)(managerPtr->entityList[i]);
  }
}
void entitymanager_logic( EntityManager* managerPtr );
void entitymanager_draw( EntityManager* managerPtr );