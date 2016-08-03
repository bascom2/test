#include <stdio.h>
#include <string.h>

void foo()
{
}

void f(char *s)
{
	char buff[1024];

	strcpy(buff, s);
}

int main(int argc, char *argv[])
{
	if (argc > 1) f(argv[1]);

	return 0;
}
