/*Description	: Read lines from file or input/pipe
	Author		: Anoop Sundaresh
	Date		: 02/12/2018
*/

#include "types.h"
#include "stat.h"
#include "user.h"

char buf[512];

void head(int fd, int noOfLines)
{
	int n, lines;

	lines = 0;
	while(((n = read(fd, buf, sizeof(buf))) > 0) && lines < noOfLines) {
		for(int i=0; i<n && lines < noOfLines; i++){
			if (buf[i] != '\n')
				printf(1, "%c", buf[i]);
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
	int fd, i, lineCount;

	// If user has used redirection on the command line
	if(argc <= 1){
		lineCount = 10;
		head(0, lineCount);
		exit();
	}

	else {
		switch(argc){
			case 2 :
			{
				lineCount = 10;
				i = 1;
				break;
			}
			case 3 :
			{
				char l_str[512];
				strcpy(l_str, argv[1]);
				char *l_cptr = l_str;
				l_cptr = l_cptr + 1; //Compensating for the hyphen(-)
				lineCount = atoi(l_cptr);
				i = 2;
				break;
			}
			case 4 :
			{
				lineCount = atoi (argv[2]);
				i = 3;
				break;
			}
			default:
				printf(1, "head : Unknon Syntax\n");
				exit();
		}

		for(; i < argc; i++){
			if((fd = open(argv[i], 0)) < 0){
				printf(1, "head: cannot open %s\n", argv[i]);
				exit();
			}
			head(fd, lineCount);
			close(fd);
		}
	}
	
	exit();
}