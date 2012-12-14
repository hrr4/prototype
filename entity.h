#include <stdio.h>

typedef struct
{
  void (*Handle_Events)( );
  void (*Logic)( Entity* );
  void (*Draw)( Entity* );

  /* Data */
  int health;

} Entity;

void entity_handleEvents( Entity* pEntity );
void entity_Logic( Entity* pEntity );
void entity_Draw( Entity* pEntity );