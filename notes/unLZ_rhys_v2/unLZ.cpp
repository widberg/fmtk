
#define DBG if(0)

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#pragma pack(1)

typedef unsigned short word;
typedef unsigned int   uint;
typedef unsigned char  byte;

uint flen( FILE* f ) {
  fseek( f, 0, SEEK_END );
  uint len = ftell(f);
  fseek( f, 0, SEEK_SET );
  return len;
}

enum {
  winlog=14,
  winsize=(1<<winlog),
  winmask=(1<<winlog)-1
};
byte win[winsize];

int main( int argc, char** argv ) {

  if( argc<3 ) return 2;

  uint c,d,r,l,i,j,flag,flagmask,flagbit,pos,lenbits=2;

  FILE* f = fopen( argv[1], "rb" ); if( f==0 ) return 2;
  FILE* g = fopen( argv[2], "wb" ); if( g==0 ) return 2;

  for( i=0; i<winsize; i++ ) win[i]=0;

  l = getc(f);
  l|= getc(f)<<8;
  l|= getc(f)<<16;
  l|= getc(f)<<24;
printf( "len1=%i\n", l );

  flagbit=0;

  for( pos=0;; ) {

    if( flagbit<=1 ) {
      flagmask = getc(f)<<24;
      flagmask|= getc(f)<<16;
      flagmask|= getc(f)<<8;
      flagmask|= getc(f);
      flagbit = 32-1;
      lenbits = winlog-(flagmask&3);
DBG printf( "mask=%08X\n", flagmask );
    }

    flag = (flagmask>>flagbit)&1; flagbit--;

    c = getc(f); if( c==-1 ) break;

DBG printf( "c=%02X flag=%i flagbit=%i lenbits=%i\n", c, flag, flagbit, lenbits );

    if( flag==0 ) {
      // literal

DBG printf( "literal %02X '%c'  %i\n", c, c>=32?c:'?', pos );

      win[ (pos++)&winmask ] = c; putc( c, g );

    } else {
      // match

      d = getc(f);
      j = (c<<8)+d;

      l = (j >> lenbits) + 3;
      d = (j & ((1<<lenbits)-1)) + 1;

DBG printf( "match d=%i/%X l=%i/%X %04X\n", d,d,l,l, j );

      for( j=0; j<l; j++ ) {
        c = win[ (pos-d)&winmask ];
DBG printf( " %02X '%c'  %i %i\n", c, c>=32?c:'?', pos, (pos-d+j)&winmask );
        win[ (pos++)&winmask ] = c; putc( c, g );
      }

    }

  }


}
