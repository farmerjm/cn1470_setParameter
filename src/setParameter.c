#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/CAENHVWrapper.h"
#include <time.h>

clock_t start, end;

CAENHVRESULT ret;
void* result;
float* fparam = NULL;
long* lparam = NULL;
char* params[5] = {"IMon", "VSet", "ISet", "MaxV", "Pw"};

int handle = -1;
const unsigned short ch1[1]={0};

int rampWait(float setVolts, unsigned short ch)
{
  fparam=malloc(sizeof(float));
  short doLoop=1;
  while(doLoop)
  {
    ret=CAENHV_GetChParam(handle, 0, "VMon", 1, &ch, fparam);
    if (ret != CAENHV_OK) printf("Error: CAENHV_GetChParam: %s (num %d)\n\n", CAENHV_GetError(handle), ret);
    if (abs(*fparam-setVolts)>1)
    {
      printf("    Voltage ramping. Curent value:  %f, setVolts %f \n", *fparam, setVolts);
      sleep(1);
    }
    else doLoop=0;
  }

  if (setVolts!=0)
  {
    printf("    Reached target. Waiting for stability...\n");

    float lastFive[5]={0,0,0,0,0};
    unsigned short i, loopCount;
 

    doLoop=1;
    loopCount=0;

    while(doLoop)
    {
      loopCount++;
      CAENHV_GetChParam(handle, 0, "VMon", 1, &ch, fparam);
      for(i=1; i<5; i++) lastFive[5-i]=lastFive[4-i];
      lastFive[0]=*fparam;
   
      if ((lastFive[0]==lastFive[1]) && (lastFive[0] == lastFive[2]) && (lastFive[0] == lastFive[3]) && (lastFive[0]==lastFive[4])) doLoop=0; else sleep(1.5);

      if (loopCount > 30)
      {
        printf("WARNING: Metastable voltage. See errlog for details.");
        fprintf(stderr, "WARNING: Metastable voltage reached on channel %d. Reporting 30 measurements taken over 30 seconds:", ch);       
   
        for(i=0;i<30;i++)
        {
          CAENHV_GetChParam(handle, 0, "VMon", 1, &ch, fparam);
          fprintf(stderr, "    - V = %d", *fparam);
          sleep(1);
        }

        doLoop=0;

      }
    }

    printf("======VOLTAGE STABLE AT %f======\n\n",*fparam);
  }

}
/*  SetParam. Usage:
 *     ./setParam param value channels
 *    
 *     Takes channel 0 by default if none are specified.
 *
 *     Setting the "VSet" parmeter will commence RampWait over all the requested channels,
 *     which will poll the device periodically and return when a stable final voltage is reached.
 *
 */
int main(int argc, char* argv[])
{

  unsigned short i;
  unsigned short numChannels=0;


  unsigned short* output;
  const unsigned short ch1[1] = {0};
  unsigned long result;

  unsigned short flag=0;
  unsigned short ch[4];
  
  char inParam[10];

  int pwon = 1;


  //
  //  Argument processing
  //

  if (argc < 3)
  {
    fprintf(stderr, "Error! Invalid arguments.");
    return 2;
  }

  strcpy(inParam, argv[1]);

  if (argc==4)
  {
    for(i=0; i<strlen(argv[3]); i++)
    {
      ch[i]= (unsigned short) argv[3][i] - '0';
      numChannels++;
    }


    if (strlen(argv[3]) > 4)
    {
      fprintf(stderr, "Error: invalid channel list.");
      return 2;
    }

  }
  else
  {
    ch[0]=0;
    numChannels=1;
  }

  for(i=0; i<5; i++)
  {
    if (!strcmp(inParam, params[i])) flag=1;    
  }

  if (!flag)
  {
    fprintf(stderr, "Error! Parameter not recognized.");
    return 2;
  }
 
  printf("Connecting to module...\n");

  start=clock();
  ret = CAENHV_InitSystem(6, 5, "ttyUSB0_9600_8_1_none_0", "", "", 
&handle);
  end=clock();
  
  if (ret != CAENHV_OK)
  {
    fprintf(stderr, "ERROR:  Connection failed. Exiting process.\n");
    fprintf(stderr, "Please ensure you have selected the right port and have IO access.\n");
    printf("\nCAENHV_InitSystem: %s (num. %d)\n\n", CAENHV_GetError(handle), ret); 
    return 2;
  }
  else
  {
    printf("  Connection established! (Operation took %f seconds.)\n\n", (float) ((end-start) / CLOCKS_PER_SEC ));
  }

    //
    //  Now, set the parameter:
    //

    float setParam=atof(argv[2]);

    //ret = CAENHV_GetChParamProp(handle, 0, 0, inParam, "Type", &result);
    //printf("\nCAENHV_GetChParamProp: %s (num. %d)\n\n", CAENHV_GetError(handle), ret);

    //if (ret != CAENHV_OK) printf("Error %d GetChParamProp!", ret);
    
    if (!strcmp(inParam, "Pw") && (setParam == 1)) 
    {
      printf("Power enabled. Setting voltage to zero.\n\n");
      float zeroVolts = 0;
      CAENHV_SetChParam(handle, 0, "VSet", numChannels, ch, &zeroVolts); 
    }

    //CAENHV_SetChParam(handle, 0, inParam, numChannels, ch, &setParam);
    for(i=0;i<numChannels;i++)
    {
      CAENHV_SetChParam(handle, 0, inParam, 1, &ch[i], &setParam);
    }

    if (!strcmp(inParam, "Pw") && (setParam==0))
    {
      printf("Voltage ramping down to zero. This may take a few minutes.\\nn");
    }

    if (!strcmp(inParam, "VSet"))
    {
      for (i=0; i<numChannels;i++)
      {
        printf("Checking voltage ramp for channel %i:\n", i);
        rampWait(setParam, ch[i]);
      }
    }

/*    else
    {
      if (result == PARAM_TYPE_NUMERIC)
      {
        fparam=malloc(sizeof(float));
        ret = CAENHV_GetChParam(handle, 0, inParam, 1, ch1, fparam); 
      }
 
  else
      {
        lparam=malloc(sizeof(long));
        ret = CAENHV_GetChParam(handle, 0, inParam, 1, ch1, lparam);
      }
    }
*/
  
  return 0;

}
