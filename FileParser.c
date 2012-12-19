
#include "FileParser.h"

int FileParser_OpenFile(char* fileString, FileParser* parserStruct)
{
  if (parserStruct->currentFile = fopen(fileString, "rt")) 
    return 1; 
  return 0;
}

int FileParser_CloseFile(FileParser* parserStruct)
{
  if (fclose(parserStruct->currentFile) == 0)
    return 0;
  return EOF;
}

fpos_t FileParser_FindNode(char* nodeString, FileParser* parserStruct)
{
  /*char strBuf[150];
  fpos_t objPos;

  while (!feof(parserStruct->currentFile))
  {
    if (fgets(strBuf, 150, parserStruct->currentFile))
    {
      if (strstr(strBuf, nodeString))
      {
        fgetpos(parserStruct->currentFile, &objPos);
        rewind(parserStruct->currentFile);

        return objPos-strlen(strBuf)-2;
      }
    }
  }

  rewind(parserStruct->currentFile);
  fpos_t objPos;*/
  
  /* fgetpos(parserStruct->currentFile, &objPos); */
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

    /* while (!feof(parserStruct->currentFile))
    { */
      /* Get the rest of the line. */
      if (fgets(lineBuf, 150, parserStruct->currentFile))
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
    /* } */

    return "";
  }
  else
  {
    return "";
  }
}

fpos_t FileParser_NextNode(long currentPosition, char* nodeString, FileParser* parserStruct)
{
  char lineBuf[250];
  char* delims = "< ";
  long newPos;
  /* fpos_t objPos; */

  /* fsetpos(parserStruct->currentFile, currentPosition); */
  fseek(parserStruct->currentFile, currentPosition, SEEK_SET);

  while (!feof(parserStruct->currentFile))
  {
    if (fgets(lineBuf, 150, parserStruct->currentFile))
    {
      int lineBufSize = strlen(lineBuf);
      char* currentToken = strtok(lineBuf, delims);

      /* fgetpos(parserStruct->currentFile, &objPos); */
      if (strcmp(currentToken, nodeString) == 0)
      {
        /* return the position of our object. */
        //fgetpos(parserStruct->currentFile, &objPos);
        newPos = ftell(parserStruct->currentFile);

        /* BUG: FSEEK AND REWIND WONT GO BACK TO THE BEGINNING OF THE FILE! */
        fseek(parserStruct->currentFile, 0, SEEK_SET);

        return newPos-(lineBufSize - strlen(nodeString))-2;
      }
    }
  }

  return -1LL;
}
