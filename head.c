/*Description	: Read lines from file or input/pipe
	Author		: Anoop Sundaresh
	Date		: 02/12/2018
*/

#include "types.h"
#include "stat.h"
#include "user.h"

#define DEFLINES 10

char accumBuff[512];

void head(int fd, int noOfLines)
{
	int n;
	int lines = 0;
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

int noOfLines(char *lCptr)
{
	int lineCount = 0;
	lCptr = lCptr + 1; //Compensating for the hyphen(-)
	lineCount = atoi(lCptr);
	return lineCount;
}

int main(int argc, char *argv[])
{
	int fd = -1; 
	int i = 0;
	int lineCount = 0;

	// If user gives direct input on the command line
	if(argc <= 1){
		lineCount = DEFLINES;
		head(0, lineCount);
		exit();
	}

	else if (argv[1][0] == '-')
	{
		if (argv[1][1] == 'n')
		{
			if (argc >= 4)
			{
				lineCount = atoi(argv[2]);
				i = 3;
			}
			else
			{
				lineCount = atoi(argv[2]);
				head(0, lineCount);
				exit();
			}
		}
		else if (argc == 3)
		{
			lineCount = noOfLines(argv[1]);
			i = 2;
		}
		else if (argc == 2)
		{
			lineCount = noOfLines(argv[1]);
			head(0, lineCount);
			exit();
		}
	}
	else 
	{
		lineCount = DEFLINES;
		i = 1;
	}

	for(; i < argc; i++){
		if((fd = open(argv[i], 0)) < 0){
			printf(1, "head: cannot open %s\n", argv[i]);
			printf(1, "<===Standard Input===>\n");
			lineCount = DEFLINES;
			head(0, lineCount);
			exit();
		}
		if (lineCount == 0)
			printf(1, "Error : Malformed Expression\n");
		else
		{
			head(fd, lineCount);
			close(fd);
		}
	}
	exit();
}