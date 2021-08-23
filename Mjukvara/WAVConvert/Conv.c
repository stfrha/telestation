#include <stdio.h>

void main( void )
{
	FILE*			i;
	FILE*			o;
	unsigned char	c;
	long			l;

	i = fopen("hej1.wav", "rb");
	o = fopen("Out.txt", "wt");

	l = 0;

	while  (!feof(i)) {
		c = fgetc(i);

		fprintf(o, "%05d\t%03d\n", l, c);
		l++;

	}
	fclose(i);
	fclose(o);

}