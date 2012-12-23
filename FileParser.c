#include "FileParser.h"

int FileParser_OpenFile(char* fileString, FileParser* parserStruct)
{
  if ((parserStruct->currentFile = fopen(fileString, "rb")) != 0) 
    return 1; 
  return 0;
}

int FileParser_CloseFile(FileParser* parserStruct)
{
  if (fclose(parserStruct->currentFile) == 0)
    return 0;
  return EOF;
}

long FileParser_FindNodeByString(char* nodeString, FileParser* parserStruct) { return FileParser_NextNode(fseek(parserStruct->currentFile, 0, SEEK_SET)-strlen(nodeString), nodeString, parserStruct); }

long FileParser_FindNodeByCount(char* nodeString, int nodeCount, FileParser* parserStruct)
{
  long currentPosition = 0;

  while (nodeCount-- >= 1 && currentPosition != EOF) currentPosition = FileParser_NextNode(currentPosition, nodeString, parserStruct);

  return (currentPosition != EOF) ? FileParser_NextNode(currentPosition, nodeString, parserStruct) : currentPosition;
}

void FileParser_RetrieveValue(long nodePosition, const char* tagString, char* dataOutput, FileParser* parserStruct)
{
  int tagLength = strlen(tagString);
  char lineBuf[LINEBUFSIZE], *delims = "=\"";

  if (nodePosition >= 0LL)
  {
    fseek(parserStruct->currentFile, nodePosition, SEEK_SET);

    if (fgets(lineBuf, LINEBUFSIZE, parserStruct->currentFile))
    {
      char* currentTag;

      if ((currentTag = strstr(lineBuf, tagString)) != 0)
      {
        /* Increment the substring pointer past the actual tag string. */
        while (tagLength-- > 0) currentTag++;

        strncpy(dataOutput, strtok(currentTag, delims), VALUESIZE);
      }
    }
  }
  else
    strncpy(dataOutput, "", VALUESIZE);
}

int FileParser_RetrieveText(long nodePosition, const int width, const int height, char*** textArray, FileParser* parserStruct)
{
  char lineBuf[LINEBUFSIZE], *delims = ",\n";

  if (nodePosition >= 0LL)
  {
    fseek(parserStruct->currentFile, nodePosition, SEEK_SET);

    if (fgets(lineBuf, LINEBUFSIZE, parserStruct->currentFile))
    {
      int i = 0;
      char nodeName[15], *linePos = lineBuf;
      
      *textArray = (char**) malloc(width * height * sizeof(char*));

      /* Get rid of spaces. */
      while (*linePos == ' ') linePos++;

      /* Get past the rest of the tags and get to the textual data! */
      while (sscanf(linePos, "<%s", nodeName) >= 1) 
      {
        fgets(lineBuf, LINEBUFSIZE, parserStruct->currentFile);
        linePos = lineBuf;
        while (*linePos == ' ') linePos++;
      }

      linePos = lineBuf;

      /* Should be at the beginning of the data we need. */
      while (sscanf(linePos, "</%s", nodeName) < 1)
      {
        char* currentToken = strtok(lineBuf, delims);

        while (currentToken != NULL && i < width*height)
        {
          (*textArray)[i] = (char*) malloc(3*sizeof(char));

          strncpy((*textArray)[i++], currentToken, 3);

          currentToken = strtok(NULL, delims);
        }

        fgets(lineBuf, LINEBUFSIZE, parserStruct->currentFile);
        linePos = lineBuf;
        while (*linePos == ' ') linePos++;
      }

      return 0;
    }
  }

  return EOF;
}

long FileParser_NextNode(long currentPosition, const char* nodeString, FileParser* parserStruct)
{
  char lineBuf[LINEBUFSIZE], nodeName[15] = { 0 };

  /* Goto the position we're starting from. */
  fseek(parserStruct->currentFile, currentPosition+strlen(nodeString), SEEK_SET);

  while (!feof(parserStruct->currentFile))
  {
    /* Get the rest of the line. */
    if (fgets(lineBuf, LINEBUFSIZE, parserStruct->currentFile))
    {
      char* linePos = lineBuf;

      /* Check for spaces because of XML nesting.
         If we have some, incrememt the pointer. */
      while (*linePos == ' ') linePos++;

      /* Read the new node name. */
      sscanf(linePos, "<%s", nodeName);
    }

    /* See if we're at the right node. */
    if (strcmp(nodeName, nodeString) == 0)
      /* Match */
      return (ftell(parserStruct->currentFile) - strlen(lineBuf));
  }

  return EOF;
}

int FileParser_NodeCount(const char* nodeString, FileParser* parserStruct)
{
  int count = 0;
  long currentPosition = 0;

  while ((currentPosition = FileParser_NextNode(currentPosition, nodeString, parserStruct)) != EOF) count++;

  return count;
}

void FileParser_EmptyLayer(const int width, const int height, char*** textArray)
{
  int row, col;

  for (row = 0; row < height; ++row)
    for (col = 0; col < width; ++col)
      free((*textArray)[col+(row*100)]);
}

void FileParser_DestroyLayer(const int width, const int height, char*** textArray)
{
  FileParser_EmptyLayer(width, height, textArray);

  free(*textArray);
}
