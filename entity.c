#include "entitymanager.h"

void entity_handleEvents( Entity* pEntity )
{
  printf("Running Entity (Handle Events) on: %p.\n", (void *)pEntity);
}

void entity_Logic( Entity* pEntity )
{
  printf("Running Entity (Logic) on: %p.\n", (void *)pEntity);
}

void entity_Draw( Entity* pEntity )
{
  printf("Running Entity (Draw) on: %p.\n", (void *)pEntity);
}