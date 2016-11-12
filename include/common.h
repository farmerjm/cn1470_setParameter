#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "CAENHVWrapper.h"


CAENHVRESULT ret;
void* result;
float* fparam = NULL;
long* lparam = NULL;
char* params[5] = {"IMon", "VSet", "ISet", "MaxV", "Pw"};

int handle = -1;
const unsigned short ch1[1]={0};

char[100] connString="_9600_8_1_none_0";

int readConfig()
{
  int val;
  FILE* configFile=fopen("../config","r");
  char inbuff[256];
  fscanf(configFile, "%s %d\n", &inbuff[0], &val);
  return val;
}

char[] buildConnectionString()
{
  char portNumber=readConfig()+'0';
  char port[8] ="ttyUSBx";
  port[6]=portNumber;
  strcat(port, connString);
  return connString;
}
