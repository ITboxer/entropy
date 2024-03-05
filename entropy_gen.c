#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>

#include <linux/types.h>
#include <linux/random.h>

#define N 1024

int main()
{
  struct {
    int ent_count;
    int size;
    unsigned char data[N];
  } entropy;
  
  int src, dst;
  FILE *source;
  
  if ( (dst = open("/dev/random", O_WRONLY)) < 0) {
    perror("random" );
    exit(1);
  }
  
  if ( (src=open("/dev/urandom", O_RDONLY )) < 0 ) {
    perror("urandom");
    exit(1);
  }
  
  for ( ; ; ) {
    
    char buf[N];

    if ( N != read( src, entropy.data, N ) ) {
      perror("read");
      exit(1);
    }
    
    entropy.ent_count = entropy.size = N;

    if ( ioctl(dst, RNDADDENTROPY, &entropy) < 0 ) {
      perror("ioctl");
      exit(1);
    }

    sleep( 1 );
  }

  exit( 0 );
}
