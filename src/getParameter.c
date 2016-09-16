#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/CAENHVWrapper.h"

CAENHVRESULT ret;
void* result;
float* fparam = NULL;
long* lparam = NULL;
char* params[5] = {"IMon", "VSet", "ISet", "MaxV", "Pw"};

int handle = -1;
const unsigned short ch1[1]={0};

int main(int argc, char* argv[])
{
  short i;

  short numChannels = 0;
 
  unsigned short* output;
  unsigned long result;

  unsigned short flag=0;
  unsigned short ch[4];    
 
  //
  // Connect:
  //
  ret = CAENHV_InitSystem(6, 5, "ttyUSB2_9600_8_1_none_0", "", "", &handle);
 
  if (ret != CAENHV_OK) 
  {
    fprintf(stderr, "ERROR:  Connection failed. Exiting process.\n");
    fprintf(stderr, "Please ensure you have selected the right port and have IO access.\n");
    printf("\nCAENHV_InitSystem: %s (num. %d)\n\n", CAENHV_GetError(handle), ret); 
    return 2;
  }



  //
  // Process inputs:
  //

  char inParam[10];

  int pwon = 1;

  strcpy(inParam, argv[1]);

  for (i=0; i<5; i++) if (!strcmp(inParam, params[i])) flag=1;

  if (!flag)
  {
    printf("Error! Parameter not recognized.");
    return 1;
  }

  if (argc<2)
  {
    printf("Invalid arguments!");
    return 1;
  }

  if (argc==2)
  {
    ch[0]=0;
    numChannels=1;
  }
  else
  {

    if (strlen(argv[2]) > 4)
    {
      printf("Error: invalid channel list.");
      return 0;
    }

    for (i=0; i<strlen(argv[2]); i++)
    {
      ch[i]=(unsigned short) argv[2][i] - '0';
      numChannels++;
    }

  }



  //
  //  Query instrument for parameter:
  //

 printf("Query stage\n\n");
  float* fparam = malloc(sizeof(float));

  for (i=0; i<numChannels;i++)
  {
    ret = CAENHV_GetChParam(handle, 0, inParam, 1, &ch[i], fparam);

    if (ret != CAENHV_OK) printf("CAENHV_GetChParam: %s (num. %d)\n\n", CAENHV_GetError(handle), ret);

  }


  //Report result: 

  for (i=0; i<numChannels; i++) 
  {
   printf("Channel %i %s: %f\n", ch[i], inParam, fparam[i]); 
  }

  return 0;
}
