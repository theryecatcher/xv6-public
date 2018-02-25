/*Description	: Read lines from file or input/pipe
	Author		: Anoop Sundaresh
	Date		: 02/12/2018
*/

#include "types.h"
#include "stat.h"
#include "user.h"

char accumBuff[512];

void head(int fd, int noOfLines)
{
	int n, lines;

	lines = 0;
	while(((n = read(fd, accumBuff, sizeof(accumBuff))) > 0) && lines < noOfLines) {
		for(int i=0; i<n && lines < noOfLines; i++){
			if (accumBuff[i] != '\n')
				printf(1, "%c", accumBuff[i]);
			else
			{
				printf(1, "\n");
				lines++;
			}
		}
	}
	if(n < 0){
		printf(1, "head: read error\n");
		exit();
	}
}

int main(int argc, char *argv[])
{
	int fd = -1; 
	int i = 0;
	int lineCount = 0;

	// If user gives direct input on the command line
	if(argc <= 1){
		lineCount = 10;
		head(0, lineCount);
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
			printf(1, "head: cannot open %s\n", argv[i]);
			exit();
		}
		if (lineCount == 0)
			printf(1, "Error : Undefined Integer for number of lines\n");
		else
		{
			head(fd, lineCount);
			close(fd);
		}
	}
	
	exit();
}