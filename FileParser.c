#include "FileParser.h"

int FileParser_OpenFile(char* fileString, FileParser* parserStruct)
{
  if (parserStruct->currentFile = fopen(fileString, "rb")) 
    return 1; 
  return 0;
}

int FileParser_CloseFile(FileParser* parserStruct)
{
  if (fclose(parserStruct->currentFile) == 0)
    return 0;
  return EOF;
}

long FileParser_FindNode(char* nodeString, FileParser* parserStruct)
{
  return FileParser_NextNode(fseek(parserStruct->currentFile, 0, SEEK_SET), nodeString, parserStruct);
}

/*************************************************************************
  RetrieveData
    - Given an object and tag, return the data attributed to them.

*************************************************************************/
char* FileParser_RetrieveData(long nodePosition, char* tagString, FileParser* parserStruct)
{
  int tagLength = strlen(tagString);
  char lineBuf[250], *delims = "=\"";

  if (nodePosition >= 0LL)
  {
    fseek(parserStruct->currentFile, nodePosition, SEEK_SET);

    if (fgets(lineBuf, 250, parserStruct->currentFile))
    {
      char* currentTag;

      if ((currentTag = strstr(lineBuf, tagString)))
      {
        /* Increment the substring pointer past the actual tag string. */
        while (tagLength-- > 0) currentTag++;

        return strtok(currentTag, delims);
      }
    }
  }

  return "";
}

long FileParser_NextNode(long currentPosition, char* nodeString, FileParser* parserStruct)
{
  char lineBuf[250], nodeName[15];

  /* Goto the position we're starting from. */
  fseek(parserStruct->currentFile, currentPosition, SEEK_SET);

  while (!feof(parserStruct->currentFile))
  {
    /* Get the rest of the line. */
    if (fgets(lineBuf, 250, parserStruct->currentFile))
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
      return ftell(parserStruct->currentFile) - strlen(lineBuf);
  }

  return -1LL;
}
