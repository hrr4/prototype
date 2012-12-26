#ifndef LEVELDATA_H
#define LEVELDATA_H

#include "Tileset.h"
#include "CollisionBox.h"
#include "Layer.h"
#include "FileParser.h"

#include <assert.h>

typedef struct
{
  LAYER** Layers;
  int layerCount;

  CollisionPoly collisionData[50];

  FILEPARSER* currentParser;
  char levelName[50];
} LEVELDATA;

/*******************************
          Externals
*******************************/
/* All initialization is encapsulated here. */
LEVELDATA* LevelData_Initialize(const char* levelString);

void LevelData_Cleanup(LEVELDATA* dataStruct);

/*******************************
          Statics
*******************************/
static int LevelData_RetrieveLayers(LEVELDATA* dataStruct);
static int LevelData_RetrieveCollisionData(LEVELDATA* dataStruct);

#endif
