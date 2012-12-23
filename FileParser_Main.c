#include "FileParser.h"

#include <assert.h>

int main(int argc, char* argv[])
{
  long testPosition;
  char objectString[15], dataString[VALUESIZE], fileString[20], tagString[20], **textArray = 0;
  int nodeCount = 0, row = 0, col = 0;

  FileParser* myParser = (FileParser *)malloc(sizeof(FileParser));


  //////////////////////////////////////////////////////////////////////


  printf("Give me a file to open and search through: ");
  scanf("%s", fileString);

  assert(FileParser_OpenFile(fileString, myParser));


  //////////////////////////////////////////////////////////////////////


  printf("Give me an Object to search for: ");
  scanf("%s", objectString);

  testPosition = FileParser_FindNodeByString(objectString, myParser);

  printf("Give me a Tag to search for: ");
  scanf("%s", tagString);

  FileParser_RetrieveValue(testPosition, tagString, dataString, myParser);

  printf("Node:\t\t%s\nProperty:\t%s\nData:\t\t%s\n", objectString, tagString, dataString);


  //////////////////////////////////////////////////////////////////////


  /* -1 signals we either couldn't find the node or we reached the EOF. */
  while ((testPosition = FileParser_NextNode(testPosition, objectString, myParser)) != EOF)
  {
    printf("Lets find the rest!\n");

    FileParser_RetrieveValue(testPosition, tagString, dataString, myParser);

    printf("\nNode:\t\t%s\nProperty:\t%s\nData:\t\t%s\n", objectString, tagString, dataString);
  }


  //////////////////////////////////////////////////////////////////////


  nodeCount = FileParser_NodeCount(objectString, myParser);
  printf("Number of %s Nodes: %d\n", objectString, nodeCount);

  printf("Find %s node by #: ", objectString);
  scanf("%d", &nodeCount);


  //////////////////////////////////////////////////////////////////////


  if ((testPosition = FileParser_FindNodeByCount(objectString, nodeCount, myParser)) != EOF)
  {
    printf("Give me a Tag to search for: ");
    scanf("%s", tagString);

    FileParser_RetrieveValue(testPosition, tagString, dataString, myParser);

    printf("Node:\t%s\nProperty:\t%s\nData:\t%s\n", objectString, tagString, dataString);
  }
  else
    printf("Don't have that many nodes!\n");


  //////////////////////////////////////////////////////////////////////


  if (FileParser_RetrieveText(testPosition, 100, 100, &textArray, myParser) == EOF)
  {
    printf("Couldn't get text!\n");
    return -1;
  }

  for (row = 0; row < 100; ++row)
  {
    printf("Row: %d: ", row);

    for (col = 0; col < 100; ++col)
      printf("%s,", textArray[col+(row*100)]);

    printf("\n");
  }

  /* FileParser_EmptyLayer(100, 100, &textArray); */
  FileParser_DestroyLayer(100, 100, &textArray);

  /* Cleanup */
  FileParser_CloseFile(myParser);
  free(myParser);

  return 0;
}
