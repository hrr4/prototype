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
  char lineBuf[250];
  char* delims = " =<>\"";

  if (nodePosition >= 0LL)
  {
   /* fsetpos(parserStruct->currentFile, nodePosition); */
    fseek(parserStruct->currentFile, nodePosition, SEEK_SET);

    /* Get the rest of the line. */
    if (fgets(lineBuf, 250, parserStruct->currentFile))
    {
      /* Tokenize the buffer, lets look for our tag! */
      char* currentToken = strtok(lineBuf, delims);

      while (currentToken != NULL)
      {
        if (strcmp(tagString, currentToken) == 0)
        {
          /* Found the tag! We need to retrieve the data now. */
          /* Should be the next token!... Hopefully....*/
          fseek(parserStruct->currentFile, 0, SEEK_SET);

          return strtok(NULL, delims);
        }

        currentToken = strtok(NULL, delims);
      }
    }

    return "";
  }
  else
  {
    return "";
  }
}

long FileParser_NextNode(long currentPosition, char* nodeString, FileParser* parserStruct)
{
  char lineBuf[250], nodeName[15];
  long newPos;
  static int lineNums = 0;

  fseek(parserStruct->currentFile, currentPosition, SEEK_SET);

  while (!feof(parserStruct->currentFile))
  {
    /* Get the rest of the line. */
    if (fgets(lineBuf, 250, parserStruct->currentFile))
    {
      char* linePos = lineBuf;

      /* ++lineNums; */

      /* Check for spaces because of XML nesting. */
      while (*linePos == ' ') linePos++;

      if (sscanf(linePos, "<%s", nodeName))
        ++lineNums;
    }

    /* See if we're at the right node. */
    if (strcmp(nodeName, nodeString) == 0)
    {
      /* Match */
      int length = strlen(lineBuf);/* +1+lineLength;*/
      long pos = ftell(parserStruct->currentFile) - /* lineNums - */ length;

      fseek(parserStruct->currentFile, pos, SEEK_SET);
      fgets(lineBuf, 250, parserStruct->currentFile);
      return pos;
    }
  }

  return -1LL;
}
