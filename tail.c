/*Description	: Read lines from file or input/pipe
	Author		: Anoop Sundaresh
	Date		: 02/20/2018
*/

#include "types.h"
#include "stat.h"
#include "user.h"
#include "fcntl.h"

#define DEFLINES 10

char accumBuff[512];

void tail (int fd, int lineCount) 
{ 
	int n;
	int numLines = 0;
	int skipLines = 0;
	int filePtr;

	if ((filePtr = open("tmpfile", O_CREATE | O_RDWR)) < 0)
	{
		printf (2, "tail: write permission error\n");
		exit ();
	}

	while ((n = read(fd, accumBuff, sizeof(accumBuff))) > 0)
	{
		write (filePtr, accumBuff, n);
		for(int i=0; i<n; i++)
		{
			if(accumBuff[i]=='\n')
				numLines++;
		}
	}

	close (filePtr);

	if (n < 0) 
	{
		printf (2, "tail: read error\n");
		exit ();
	}

	if (numLines < lineCount)
		skipLines = 0;

	skipLines = numLines - lineCount;

	if ((filePtr = open("tmpfile", 0)) < 0)
	{
		printf (2, "tail: read permission error\n");
		exit ();
	}

	int printCounter = 0;
	while ((n = read(filePtr, accumBuff, sizeof(accumBuff))) > 0)
	{
		for (int i = 0; i<n; i++) 
		{
			if (printCounter >= skipLines)
				printf(1, "%c", accumBuff[i]);
			if (accumBuff[i] == '\n')
				printCounter++;
		}
	}

	close (filePtr);
	unlink("tmpfile");
	free (accumBuff);
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
		tail(0, lineCount);
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
				tail(0, lineCount);
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
			tail(0, lineCount);
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
			printf(2, "tail: cannot open %s\n", argv[i]);
			printf(1, "<===Standard Input===>\n");
			lineCount = DEFLINES;
			tail(0, lineCount);
			exit();
		}
		if (lineCount == 0)
			printf(1, "Error : Malformed Expression\n");
		else
		{
			tail(fd, lineCount);
			close(fd);
		}
	}
	exit();
}