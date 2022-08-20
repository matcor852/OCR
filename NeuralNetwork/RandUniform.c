#include "RandUniform.h"



double lcgy(int *seed) {
	 static int a=16807, m=2147483647,
	 q=127773, r=2836;
	 double minv = (double) 1.0/m;
	 *seed = a*(*seed % q)-r*(*seed / q);
	 if (*seed < 0) *seed = *seed + m;
	 return (double) *seed * minv;
}

void r250(int *x,double *r,int n)
{
	 static int q=103,p=250;
	 static double rmaxin=2147483648.0; /* 2**31 */
	 int i,k;
	 for (k=1;k<=n;k++) {
		 x[k+p]=x[k+p-q]^x[k];
		 r[k]=(double)x[k+p]/rmaxin;
	 }
	 for (i=1;i<=p;i++) x[i]=x[n+i];
}

double ran_number(void/*int *seed*/)
{
	int seed = (int) time(NULL);
	static int firsttime = 1, i, j  =NR, x[NR250p1];
	static double r[NRp1];
	if (j >= NR) {
		 if (firsttime == 1) {
			for (i = 1; i <= 250; i++)
				x[i] = (int)((double)2147483647.0f  *lcgy(&seed));
			firsttime = 0;
		 }
		 r250(x, r, NR);
		 j = 0;
	}
	j++;
	return r[j];
}

#ifdef __linux__
int getseed()
{
	int i, rdev,rnum,nb=sizeof(int),rs;
	struct timeval tp;

	rdev=open("/dev/random",O_RDONLY);
	if (rdev==-1) {
		if (gettimeofday(&tp,(struct timezone *)NULL)==0) {
			i=tp.tv_sec+tp.tv_usec;
			i=(i%1000000)|1;
			return i;
		} else return -1;
	} else {
		rs=read(rdev,&rnum,nb);
		if (rs>0) return MASK&rnum;
		else return -1;
	}
}
#endif
