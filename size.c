#include <stdlib.h>
#include <stdio.h>

void *memset(void* f, int c,size_t byte);

main ()
{
 int i[10],j;
 memset(i,1,40);
 printf("%d\n",sizeof(i));
for (j=0;j<10;j++)
 printf("%d\n",i[j]);
}
