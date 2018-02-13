/*Description	: Print Hello World on the screen
	Author		: Anoop Sundaresh
	Date		: 02/12/2018
*/

#include "types.h"
#include "stat.h"
#include "user.h"

char buf[512];

void head(int fd)
{
  int n, l;

  l=0;
  while(((n = read(fd, buf, sizeof(buf))) > 0) && l <= 10) {
  	l = 0;
  	for(int i=0; i<n; i++){
      if (buf[i] == '\n'){
      	l++;
      }
      if (l > 10)
      	break;
	}
	printf(1, "l = %d\n", l);
    if (write(1, buf, l) != l) {
      printf(1, "head: write error\n");
      exit();
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
  for(i = 1; i < argc; i++){
    if((fd = open(argv[i], 0)) < 0){
      printf(1, "head: cannot open %s\n", argv[i]);
      exit();
    }
    head(fd);
    close(fd);
  }
  exit();
}