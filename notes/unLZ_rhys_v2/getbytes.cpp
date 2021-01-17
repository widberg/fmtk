#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include <string.h>

typedef unsigned char byte;
typedef unsigned int  uint;

uint flen( FILE* f ) {
  fseek( f, 0, SEEK_END );
  uint len = ftell(f);
  fseek( f, 0, SEEK_SET );
  return len;
}

#ifndef __min
#define __max(a,b)  (((a) > (b)) ? (a) : (b))
#define __min(a,b)  (((a) < (b)) ? (a) : (b))
#endif

const uint l = 1<<16;
byte buf[l];

int main( int argc, char** argv ) {

  uint i,j, a,b,x;

  if( argc<4 ) return 1;

  FILE* f = fopen( argv[1], "rb" );
  if( !f ) return 1;

  FILE* g = fopen( argv[2], "wb" );
  if( !g ) return 1;

  sscanf( argv[3], "%08X", &a );
  printf( "offset = %08X %i\n", a, a );

  if( argc>4 ) {
    sscanf( argv[4], "%08X", &b );
  } else {
    b = flen(f) - a;
  }
  printf( "length = %08X %i\n", b, b );

  fseek(f,a,SEEK_SET);

  for( i=0; i<b; i+=l ) {
    j = __min(b,i+l) - i;
    x = fread( buf,1,j,f );
    fwrite( buf,1,x,g );
    if( x<j ) break;
  }

  return 0;
}
