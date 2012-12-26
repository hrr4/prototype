#include "LevelData.h"

LEVELDATA* LevelData_Initialize(const char* levelString)
{
  LEVELDATA* levelDataStruct = (LEVELDATA *) malloc(sizeof(LEVELDATA));
  if (levelDataStruct)
  {
  levelDataStruct->currentParser = (FILEPARSER *) malloc(sizeof(FILEPARSER));
  strncpy(levelDataStruct->levelName, levelString, sizeof(levelDataStruct->levelName));

  assert(FileParser_OpenFile(levelDataStruct->levelName, levelDataStruct->currentParser));

  if (LevelData_RetrieveLayers(levelDataStruct) < 1)
    printf("Problem getting layers! Quick! GET OUT! ITS GONNA BLOW!\n");
  LevelData_RetrieveCollisionData(levelDataStruct);
  }

  return levelDataStruct;
}

void LevelData_Cleanup(LEVELDATA* dataStruct)
{
  int i;
  
  /* Cleanup each layer. */
  for (i = 0; i < dataStruct->layerCount; ++i)
    free(dataStruct->Layers[i]);    

  free(dataStruct->Layers);

  FileParser_CloseFile(dataStruct->currentParser);

  free(dataStruct);
}

/*******************************
          Statics
*******************************/
static int LevelData_RetrieveLayers(LEVELDATA* dataStruct)
{
  int layerCount = 0;
  long currentPosition = 0;
  char dataBuf[75];
  char* layerString = "layer";
  LAYER newLayer = { 0 };
  FILEPARSER* currentFileParser = dataStruct->currentParser;
  dataStruct->layerCount = FileParser_NodeCount(layerString, currentFileParser);
  dataStruct->Layers = (LAYER **) malloc(sizeof(LAYER*) * dataStruct->layerCount);

  if (!dataStruct->Layers)
    return -1;

  while((currentPosition = FileParser_NextNode(currentPosition, layerString, dataStruct->currentParser)) != EOF)
  {
    FileParser_RetrieveValue(currentPosition, "width", dataBuf, currentFileParser);
    newLayer.width = (short)atoi(dataBuf);

    FileParser_RetrieveValue(currentPosition, "height", dataBuf, currentFileParser);
    newLayer.height = (short)atoi(dataBuf);

    if (FileParser_RetrieveText_AsInt(currentPosition, newLayer.width, newLayer.height, &newLayer.layerData, currentFileParser) != EOF)
    {
      dataStruct->Layers[layerCount] = (LAYER *) malloc(sizeof(LAYER));
      if (!dataStruct->Layers[layerCount])
        return -1;
      memcpy(dataStruct->Layers[layerCount++], &newLayer, sizeof(newLayer));
    }
    else
    {
      printf("Something went wrong in RetrieveLayers!\n");
      return -1;
    }
  }

  return 1;
}

static int LevelData_RetrieveCollisionData(LEVELDATA* dataStruct)
{

}
