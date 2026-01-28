#include<stdio.h>
#include<stdlib.h>
#include<time.h>

int main()
{
double start, end , msec;
int c[256][256], a[256][256], b[256][256];
int n=256;

// Initialize matrices
for (int i = 0; i < n; i++) {
	for (int j = 0; j < n; j++) {
		a[i][j] = 1;
		b[i][j] = 1;
		c[i][j] = 0;
	}
}

start=(double)clock()/CLOCKS_PER_SEC;
for (int i = 0; i < n; i++)
	for (int k = 0; k < n ; k++)
		for (int j = 0; j < n ; j++)
			c[i][j] += a[i][k]* b[k][j];
end=(double)clock()/CLOCKS_PER_SEC;
msec=(end-start)*1000.0;
printf("Time taken: %f msec\n", msec);
printf("Sample result c[0][0]=%d\n", c[0][0]);

return 0;
}