#include <stdio.h>

int readConfig()
{
  int val;
  FILE* configFile=fopen("config","r");
  char inbuff[256];
  fscanf(configFile, "%s %d\n", &inbuff[0], &val);
  return val;
}

int main()
{
  int val=readConfig();
  printf("%d", val);
  return 0;
}
