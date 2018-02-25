#include "types.h"
#include "stat.h"
#include "user.h"
#include "fcntl.h"

char accumBuff[512];

void tail (int fd, int lineCount) 
{ 
  int n;
  int numLines = 0;
  int skipLines = 0;
  int noOfChars = 0;

  char *readBuffer = (char*) malloc(500000);
  
  while ((n = read(fd, accumBuff, sizeof(accumBuff))) > 0) {
    for (int i = 0; i<n; i++) {
      readBuffer[noOfChars] = (char)accumBuff[i];
      noOfChars++;
      if(accumBuff[i] == '\n')
        numLines++;
    }
  }

  if (n < 0) {
    printf (1, "tail: read error \n");
    free (readBuffer);
    exit ();
  }

  if (numLines < lineCount)
    skipLines = 0;

  skipLines = numLines - lineCount;

  int printCounter = 0;
  for (int i = 0; i < noOfChars; i++) {
    if (printCounter >= skipLines)
      printf(1,"%c",readBuffer[i]);
    if (readBuffer[i] == '\n')
      printCounter++;
  }
  free (readBuffer);
}

int main (int argc, char *argv[]) 
{
  int fd = -1; 
	int i = 0;
	int lineCount = 0;

  if(argc <= 1){
		lineCount = 10;
		tail(0, lineCount);
		exit();
	}

  else if (argv[1][0] == '-')
	{
		if (argc >= 4 && argv[1][1] == 'n')
		{
			lineCount = atoi(argv[2]);
			i = 3;
		}
		else if (argc >= 3)
		{
			char lStr[10];
			strcpy(lStr, argv[1]);
			char *lCptr = lStr;
			lCptr = lCptr + 1; //Compensating for the hyphen(-)
			lineCount = atoi(lCptr);
			i = 2;
		}
	}

  else 
	{
		lineCount = 10;
		i = 1;
	}

	for(; i < argc; i++){
		if((fd = open(argv[i], 0)) < 0){
			printf(1, "tail: cannot open %s\n", argv[i]);
			exit();
		}
		if (lineCount == 0)
			printf(1, "Error : Undefined Integer for number of lines\n");
		else
		{
			tail(fd, lineCount);
			close(fd);
		}
	}

  exit();
}