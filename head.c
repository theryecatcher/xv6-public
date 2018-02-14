/*Description	: Print Hello World on the screen
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
	int fd, i;

	// If user has used redirection on the command line
	if(argc <= 1){
		head(0, 10);
		exit();
	}

	// If user dierctly gives file as input
	else if(argc == 2)
	{
		for(i = 1; i < argc; i++){
			if((fd = open(argv[i], 0)) < 0){
				printf(1, "head: cannot open %s\n", argv[i]);
				exit();
			}
			head(fd, 10);
			close(fd);
		}
	}

	// if user specifies no. of lines to be read
	else if(argc == 3)
	{
		char l_str[512];
		strcpy(l_str, argv[1]);
		char *l_cptr = l_str;
		l_cptr = l_cptr + 1; //Compensating for the hyphen(-)
		int lineCount = atoi(l_cptr);

		for(i = 2; i < argc; i++){
			if((fd = open(argv[i], 0)) < 0){
				printf(1, "head: cannot open %s\n", argv[i]);
				exit();
			}
			head(fd, lineCount);
			close(fd);
		}
	}

	// if user specifies no. of lines to be read as -n
	else if(argc == 4)
	{
		int lineCount = atoi (argv[2]);

		for(i = 3; i < argc; i++){
			if((fd = open(argv[i], 0)) < 0){
				printf(1, "head: cannot open %s\n", argv[i]);
				exit();
			}
			head(fd, lineCount);
			close(fd);
		}
	}

	else
		printf(1, "head : Unknon Syntax\n");

	exit();
}