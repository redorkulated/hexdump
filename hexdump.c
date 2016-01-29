/*
	Hexdump printer
	2015 Michael Fallows
	http://michael.fallo.ws
*/
/*
	void HEXDUMP(void * DataPointer,size_t DataLen) - Produces output to stdout
	void HEXDUMP_TO(void * DataPointer,size_t DataLen,FILE *FilePointer) - Produces output to FilePointer

	Output is in the format of:

	0x0000:  b8ab 0120 0001 0000 0000 0001 0474 6573  .............tes
	0x0010:  7407 6578 616d 706c 6503 636f 6d00 00ff  t.example.com...
	0x0020:  0001 0000 2910 0000 0000 0000 00         ....)........
	 -- 45 bytes from pointer 0x51fc040 --
*/
#ifndef _HEXDUMP_H
#define _HEXDUMP_H 1

#include <stdio.h>
#include <unistd.h>
#include <string.h>
#define HEXDUMP_ROWLEN 16
#define HEXDUMP_REPLACECHAR 46
#define HEXDUMP(d,l) HEXDUMP_TO(d,l,stdout)
void HEXDUMP_TO(void *d,size_t l,FILE *w) __attribute((nonnull));
void HEXDUMP_TO(void *d,size_t l,FILE *w){
	if(!d || !w){ l= 0; }
	unsigned char *c= (unsigned char *) (d); /* Current Byte */
	unsigned char *e= c + (l); /* End Byte */
	unsigned char b[HEXDUMP_ROWLEN+1]= {0}; /* Buffer */
	for(;c < e;c++){
		/* New Line needed */
		if(!((c - (unsigned char *) (d)) % HEXDUMP_ROWLEN)){
			/* If not first line print data from last line */
			if(c != (unsigned char *) (d)){ fwrite(b,1,HEXDUMP_ROWLEN+1,(w)); }

			/* Print offset */
			fprintf((w),"\n\t0x%04lx:  ",(c - (unsigned char *) (d)));

			/* Fill buffer with spaces */
			memset(b,32,sizeof(b));
		}
		/* Print hex char */
		fprintf((w),"%02x",*c);

		/* Add char to print buffer (or a . if non-printable) */
		b[((c - (unsigned char *) (d)) % HEXDUMP_ROWLEN) + 1]= (*c > 32 && *c < 127?*c:HEXDUMP_REPLACECHAR);

		/* If second in pair, print trailing space */
		if(((c - (unsigned char *) (d)) % 2)){ fputc(32,(w)); }
	}
	/* Print trailing spaces if we do not have enough bytes */
	for(;((c - (unsigned char *) (d)) % HEXDUMP_ROWLEN);c++){
		fputc(32,(w)); fputc(32,(w));
		if(((c - (unsigned char *) (d)) % 2)){ fputc(32,(w)); }
	}

	/* Print Last line */
	fwrite(b,1,HEXDUMP_ROWLEN+1,(w));
	fprintf((w),"\n\t -- %lu bytes from pointer %p --\n",(e-(unsigned char *) (d)),(d));
}

#endif
