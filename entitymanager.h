#include "entity.h"
#include <stdlib.h>
/*#include "list.h"*/

typedef struct 
{
  /* data */
  /*List entityList; */
  Entity* entityList[2];

} EntityManager;

int entitymanager_Initialize( EntityManager* managerPtr );

void entitymanager_handleEvents( EntityManager* managerPtr );
void entitymanager_logic( EntityManager* managerPtr );
void entitymanager_draw( EntityManager* managerPtr );