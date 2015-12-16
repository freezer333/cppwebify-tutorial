#include <stdio.h> /* Achim Flammenkamp, Bielefeld, D, achim@uni-bielefeld.de */

#ifdef __APPLE__
#include <malloc/malloc.h>/* ifdefs:  LONG=uns64  _ANSI_EXTENSION  true_64bit_words  */
#else
#include <malloc.h>
#endif

#include <stdlib.h>/* SUM_{p<x} 1/p= ln(ln(x))+0.5772156649015-0.315718451893 */
#include <math.h>  /* extern double sqrt(double), log(double), floor(double); */
#ifndef  uns32              /*  Version 1.0a 1998-05-19  source availible     */
#define  uns32  unsigned    /*  Version 2.0a 1998-05-23  5 years in the WWW   */
#endif                      /*  Version 2.0b 2003-06-03  0-1-typo fixed Argh! */
#ifndef  uns64              /*  Version 2.0c 2003-06-04  works up to 2^64-1   */
# ifdef   _ANSI_EXTENSION   /*  uses < 1938 MB for minimal sieve size to 2^64 */
#  define  uns64  unsigned long long
# else
#  define  uns64  unsigned long
# endif
#endif
#ifdef   LONG
# ifdef   _ANSI_EXTENSION
#  define  UL  "%llu"
# else
#  define  UL  "%lu"
# endif
# define  HALF  uns32
#else
# define  LONG  uns32
# define  UL  "%u"
# define  HALF  unsigned short
#endif

#define  BITS  8
#define  LCM  (2*3*5)
#define  SIEVE_SIZE  (16<<10)

#ifdef  DEBUG
#define  use(x)  fprintf(out, UL"\n",x),  anz++;
#elif defined(TEST)
#define  use(x)  anz++;
#else
#define  use(x)  do { /*  if(max<=x-last)printf(UL" "UL"\n",x,max=x-last);  */ \
                      /* sum += 1.0/x, */ fprintf(out, UL"\n",x), last=x; anz++; } while(0);
double  sum=0.0;
LONG  max=0, last=2;
#endif


// This was the main function - I've changed it to a callable function instead
int generate_args(int argc, char *argv[], FILE * out)
{
uns32 register  size, hi, h, i, j, ji, js;
uns32  k, hj, c, cj;
unsigned char  *sieve, *prime_diff;
unsigned char  bi, b, m;
LONG  stop, start, stlcm, *index, ii, jj, hh, anz;
HALF  psize;
int  lim;

if (argc <= 1)
   return  fprintf(stderr,"usage: %s stop [start [size] ]\nversion 2.0c(%u:%u)"
                   "  2003-06-04\n",argv[0],(unsigned)sizeof(LONG)*BITS,LCM), 0;
if (argc <= 3 || 1!=sscanf(argv[3],"%u",&k)  ||  !(size= k&~(uns32)1))
   size = SIEVE_SIZE;
if (argc <= 2 || 1!=sscanf(argv[2],UL,&start))
   start= 0;
if (argc <= 1 || 1!=sscanf(argv[1],UL,&stop))
   stop= 1;
anz=0;
if (stop < start)  goto finish;
j = floor( sqrt((double)stop+0.1) );
if (!(j&1)) j--;
if ((j-1)/2 >= (5-1)/2*(LCM/5)*size)  /* to prevent overflow if j == 2^32-1 */
   return  fprintf(stderr,"error: sieve size must be at least %u\n",
                   1+(j-1)/(5-1)/(LCM/5)+!((j-1)/(5-1)/(LCM/5)&1)), 1;
if (!(sieve = calloc(size, sizeof(unsigned char))))
   return  fprintf(stderr,"error: can't get %u kB storage for sieve\n",
                         ((unsigned)sizeof(unsigned char)*size+(1<<10)-1)>>10), 2;
psize = floor((1.015*j)/(log((double)j)-1)); /* upper bound for Pi(stop^1/2) */
if (!(index = (LONG*) malloc((sizeof(LONG)+1)*psize)))
   return  fprintf(stderr,"error: can't get storage for %u primes\n",psize), 4;
prime_diff = (unsigned char*)(index+psize);
#ifdef  DEBUG
   fprintf(stderr,"# sieve primes <= %u   (%u kB)\n",
                  psize,(unsigned)sizeof(HALF)*((LCM*psize)/BITS+(1<<9)-1)>>10);
#endif
if (start <= 2)   use(2);
if (start <= 3)   use(3);
if (start%2 == 0)  start += 1;
if (start%3 == 0)  start += 2;
stlcm = start/LCM;
hh = size*(stlcm/size);
/*** sifting the sieve primes ***/
k=0;
m=0;
h= i= cj= 0;
js=jj=0;
b=1;
c=1;
for(;;)
{  switch (c)
   {  do
      {  case 1:
            i++;
            if ((m+=1) >= LCM/3)  m -= LCM/3;
            jj += h;
            h++;
            jj += h;
            if (!(sieve[i]&b))
            {  c= 2;
               break;
            }
         case 2:
            i++;
            if (i == size)
            {  i=0;
               cj += size;
               b += b;
               if (!b)  break;
            }
            if ((m+=1) >= LCM/3)  m -= LCM/3;
            jj += h;
            if (!(sieve[i]&b))
            {  c= 1;
               break;
            }
      } while (1);
   }
   if (8*jj > stop/3)  break;
   j = 3*(cj+i)+c;
   bi= m - !!m - m/BITS;
   prime_diff[k]= BITS*2*(j/LCM-js); /* difference of successive primes < 480 */
   prime_diff[k] |= bi;
   js= j/LCM;
   index[k]= ((bi&1) != (bi>>2&1) ? 5 : 0);
   ii= (8*jj)/(LCM/3);
   if (ii < stlcm)
      ii += j*((stlcm-ii)/j);
   if (ii < stlcm)
   {  hi = (index[k] ? 19 : 1);  /* inverse zu index[k]  ---  0 <--> 1 Argh!! */
      hj= js+js;
      ji= 2*(3*m+c);
      if (ji >= LCM)  ji-=LCM,  hj++;
      switch (c)
      {  do
         {  case 1:  ii += 2*hj;
                     hi += 2*ji;
                     while (hi >= LCM)  hi -= LCM,  ii++;
                     if (ii >= stlcm  &&  hi!=5 && hi!=25)  break;
            case 2:  ii += hj;
                     hi += ji;
                     while (hi >= LCM)  hi -= LCM,  ii++;
                     if (ii >= stlcm  &&  hi!=5 && hi!=25)  break;
         } while(1);
      }
      index[k] = (BITS*hi)/LCM;
   }
#ifdef  DEBUG
   fprintf(stderr,"k=%u prime=%u prime_mod=%u index=%u modulo=%u\n",
           k,j,prime_diff[k]%BITS,ii-hh,index[k] );
#endif
   index[k] |= BITS*(ii-hh);
   k++;
   if (jj >= size)
      continue;
/*** sift with prime pre-sieve ***/
#ifdef  true_64bit_words
   ii = 8*jj;
#define  hi ii
#else
   hi = 8*jj;
#endif
   ji= 2*(cj+i)+1;
   hj= 2*(ji+c)-3;
   bi= 1;
   switch(m)
   {   do
       {  case 0:   while(hi >= size)
                    {  hi -= size;  bi+=bi;
                       if (!bi)  goto go_on;
                    }
                    sieve[hi] |= bi;  hi += 2*j;
          case 2:   while(hi >= size)
                    {  hi -= size;  bi+=bi;
                       if (!bi)  goto go_on;
                    }
                    sieve[hi] |= bi;  hi += hj;
          case 3:   while(hi >= size)
                    {  hi -= size;  bi+=bi;
                       if (!bi)  goto go_on;
                    }
                    sieve[hi] |= bi;  hi += ji;
          case 4:   while(hi >= size)
                    {  hi -= size;  bi+=bi;
                       if (!bi)  goto go_on;
                    }
                    sieve[hi] |= bi;  hi += hj;
          case 5:   while(hi >= size)
                    {  hi -= size;  bi+=bi;
                       if (!bi)  goto go_on;
                    }
                    sieve[hi] |= bi;  hi += ji;
          case 6:   while(hi >= size)
                    {  hi -= size;  bi+=bi;
                       if (!bi)  goto go_on;
                    }
                    sieve[hi] |= bi;  hi += hj;
          case 7:   while(hi >= size)
                    {  hi -= size;  bi+=bi;
                       if (!bi)  goto go_on;
                    }
                    sieve[hi] |= bi;  hi += 2*j;
          case 9:   while(hi >= size)
                    {  hi -= size;  bi+=bi;
                       if (!bi)  goto go_on;
                    }
                    sieve[hi] |= bi;  hi += ji;
                    lim= size - LCM/3*j;
                    while ((int)hi < lim)
                    {  sieve[hi] |= bi;  hi += 2*j;
                       sieve[hi] |= bi;  hi += hj;
                       sieve[hi] |= bi;  hi += ji;
                       sieve[hi] |= bi;  hi += hj;
                       sieve[hi] |= bi;  hi += ji;
                       sieve[hi] |= bi;  hi += hj;
                       sieve[hi] |= bi;  hi += 2*j;
                       sieve[hi] |= bi;  hi += ji;
                    }
       } while (1);
       do {
          case 1:   while(hi >= size)
                    {  hi -= size;  bi+=bi;
                       if (!bi)  goto go_on;
                    }
                    sieve[hi] |= bi;  hi += ji;
          case 8:   while(hi >= size)
                    {  hi -= size;  bi+=bi;
                       if (!bi)  goto go_on;
                    }
                    sieve[hi] |= bi;  hi += hj;
                    lim= size - LCM/3* 5;
                    while ((int)hi < lim)
                    {  sieve[hi] |= bi;  hi += ji;
                       sieve[hi] |= bi;  hi += hj;
                       sieve[hi] |= bi;  hi += ji;
                       sieve[hi] |= bi;  hi += hj;
                       sieve[hi] |= bi;  hi += ji;
                       sieve[hi] |= bi;  hi += hj;
                       sieve[hi] |= bi;  hi += ji;
                       sieve[hi] |= bi;  hi += hj;
                       sieve[hi] |= bi;  hi += ji;
                       sieve[hi] |= bi;  hi += hj;
                    }
       } while (1);
   }
   go_on: ;
#ifdef  true_64bit_words
#undef  hi
#endif
}
#ifdef  DEBUG
   fprintf(stderr,"# {primes <= %u <= sqrt("UL")} = %u\n",j,stop,k+2);
#endif
/****** main sifting starts *****/
for (i=size;i--;)
   sieve[i]=0;
sieve[0] |= !hh;  /* 1 is not prime */
if (start <= 5)   use(5);
if (start%5 == 0)  start += 2*(3-start%3);
hh *= LCM;
while (1)
{  j= prime_diff[0]/BITS;
   for (h=1;h<k;h++)   /* sieve with next sieve prime (h=0 is prime 5) */
   {  j += prime_diff[h]/BITS;
      ii = index[h]/BITS;
      if (ii >= size)
      {  index[h] -= size*BITS;
         continue;
      }
      hj= (size <= LCM/2*j ? 0 : size - LCM/2*j);
      i=ji=js= j;
      ji +=js;
      i += ji;
#ifdef  true_64bit_words
#define  hi ii
#else
      hi = ii;
#endif
      switch( prime_diff[h]%BITS )
      {
         case 0:   switch( index[h]%BITS )
                   {  do {
                         case 0:   if (hi >= size)  goto out0;
                                   sieve[hi] |=  1;  hi += i;
                         case 1:   if (hi >= size)  goto out1;
                                   sieve[hi] |=  2;  hi += ji;
                         case 2:   if (hi >= size)  goto out2;
                                   sieve[hi] |=  4;  hi += js;
                         case 3:   if (hi >= size)  goto out3;
                                   sieve[hi] |=  8;  hi += ji;
                         case 4:   if (hi >= size)  goto out4;
                                   sieve[hi] |= 16;  hi += js;
                         case 5:   if (hi >= size)  goto out5;
                                   sieve[hi] |= 32;  hi += ji;
                         case 6:   if (hi >= size)  goto out6;
                                   sieve[hi] |= 64;  hi += i;
                         case 7:   if (hi >= size)  goto out7;
                                   sieve[hi] |=128;  hi += js+1;
                         lim= hj-1;
                         while ((int)hi < lim)
                         {  sieve[hi] |=  1;  hi += i;
                            sieve[hi] |=  2;  hi += ji;
                            sieve[hi] |=  4;  hi += js;
                            sieve[hi] |=  8;  hi += ji;
                            sieve[hi] |= 16;  hi += js;
                            sieve[hi] |= 32;  hi += ji;
                            sieve[hi] |= 64;  hi += i;
                            sieve[hi] |=128;  hi += js+1;
                         }
                      } while (1);
                   }
         case 1:   js+=1;  i+=1;  ji+=1;
                   switch( index[h]%BITS )
                   {  do {
                         case 5:   if (hi >= size)  goto out5;
                                   sieve[hi] |= 32;  hi += ji;
                         case 4:   if (hi >= size)  goto out4;
                                   sieve[hi] |= 16;  hi += js;
                         case 0:   if (hi >= size)  goto out0;
                                   sieve[hi] |=  1;  hi += ji-1;
                         case 7:   if (hi >= size)  goto out7;
                                   sieve[hi] |=128;  hi += js;
                         case 3:   if (hi >= size)  goto out3;
                                   sieve[hi] |=  8;  hi += ji;
                         case 2:   if (hi >= size)  goto out2;
                                   sieve[hi] |=  4;  hi += i;
                         case 6:   if (hi >= size)  goto out6;
                                   sieve[hi] |= 64;  hi += js;
                         case 1:   if (hi >= size)  goto out1;
                                   sieve[hi] |=  2;  hi += i;
                         lim= hj-7;
                         while ((int)hi < lim)
                         {  sieve[hi] |= 32;  hi += ji;
                            sieve[hi] |= 16;  hi += js;
                            sieve[hi] |=  1;  hi += ji-1;
                            sieve[hi] |=128;  hi += js;
                            sieve[hi] |=  8;  hi += ji;
                            sieve[hi] |=  4;  hi += i;
                            sieve[hi] |= 64;  hi += js;
                            sieve[hi] |=  2;  hi += i;
                         }
                      } while (1);
                   }
         case 2:   i+=2;  ji+=2;
                   switch( index[h]%BITS )
                   {  do {
                         case 0:   if (hi >= size)  goto out0;
                                   sieve[hi] |=  1;  hi += js;
                         case 6:   if (hi >= size)  goto out6;
                                   sieve[hi] |= 64;  hi += ji;
                         case 1:   if (hi >= size)  goto out1;
                                   sieve[hi] |=  2;  hi += js;
                         case 7:   if (hi >= size)  goto out7;
                                   sieve[hi] |=128;  hi += ji;
                         case 3:   if (hi >= size)  goto out3;
                                   sieve[hi] |=  8;  hi += i;
                         case 5:   if (hi >= size)  goto out5;
                                   sieve[hi] |= 32;  hi += js+1;
                         case 2:   if (hi >= size)  goto out2;
                                   sieve[hi] |=  4;  hi += i;
                         case 4:   if (hi >= size)  goto out4;
                                   sieve[hi] |= 16;  hi += ji;
                         lim= hj-11;
                         while ((int)hi < lim)
                         {  sieve[hi] |=  1;  hi += js;
                            sieve[hi] |= 64;  hi += ji;
                            sieve[hi] |=  2;  hi += js;
                            sieve[hi] |=128;  hi += ji;
                            sieve[hi] |=  8;  hi += i;
                            sieve[hi] |= 32;  hi += js+1;
                            sieve[hi] |=  4;  hi += i;
                            sieve[hi] |= 16;  hi += ji;
                         }
                      } while (1);
                   }
         case 3:   js+=1;  i+=3;  ji+=1;
                   switch( index[h]%BITS )
                   {  do {
                         case 5:   if (hi >= size)  goto out5;
                                   sieve[hi] |= 32;  hi += ji+1;
                         case 2:   if (hi >= size)  goto out2;
                                   sieve[hi] |=  4;  hi += js;
                         case 1:   if (hi >= size)  goto out1;
                                   sieve[hi] |=  2;  hi += ji;
                         case 7:   if (hi >= size)  goto out7;
                                   sieve[hi] |=128;  hi += i;
                         case 4:   if (hi >= size)  goto out4;
                                   sieve[hi] |= 16;  hi += js;
                         case 3:   if (hi >= size)  goto out3;
                                   sieve[hi] |=  8;  hi += i;
                         case 0:   if (hi >= size)  goto out0;
                                   sieve[hi] |=  1;  hi += ji;
                         case 6:   if (hi >= size)  goto out6;
                                   sieve[hi] |= 64;  hi += js;
                         lim= hj-13;
                         while ((int)hi < lim)
                         {  sieve[hi] |= 32;  hi += ji+1;
                            sieve[hi] |=  4;  hi += js;
                            sieve[hi] |=  2;  hi += ji;
                            sieve[hi] |=128;  hi += i;
                            sieve[hi] |= 16;  hi += js;
                            sieve[hi] |=  8;  hi += i;
                            sieve[hi] |=  1;  hi += ji;
                            sieve[hi] |= 64;  hi += js;
                         }
                      } while (1);
                   }
         case 4:   js+=1;  i+=3;  ji+=3;
                   switch( index[h]%BITS )
                   {  do {
                         case 5:   if (hi >= size)  goto out5;
                                   sieve[hi] |= 32;  hi += js;
                         case 6:   if (hi >= size)  goto out6;
                                   sieve[hi] |= 64;  hi += ji;
                         case 0:   if (hi >= size)  goto out0;
                                   sieve[hi] |=  1;  hi += i;
                         case 3:   if (hi >= size)  goto out3;
                                   sieve[hi] |=  8;  hi += js;
                         case 4:   if (hi >= size)  goto out4;
                                   sieve[hi] |= 16;  hi += i;
                         case 7:   if (hi >= size)  goto out7;
                                   sieve[hi] |=128;  hi += ji;
                         case 1:   if (hi >= size)  goto out1;
                                   sieve[hi] |=  2;  hi += js;
                         case 2:   if (hi >= size)  goto out2;
                                   sieve[hi] |=  4;  hi += ji-1;
                         lim= hj-17;
                         while ((int)hi < lim)
                         {  sieve[hi] |= 32;  hi += js;
                            sieve[hi] |= 64;  hi += ji;
                            sieve[hi] |=  1;  hi += i;
                            sieve[hi] |=  8;  hi += js;
                            sieve[hi] |= 16;  hi += i;
                            sieve[hi] |=128;  hi += ji;
                            sieve[hi] |=  2;  hi += js;
                            sieve[hi] |=  4;  hi += ji-1;
                         }
                      } while (1);
                   }
         case 5:   js+=2;  i+=4;  ji+=2;
                   switch( index[h]%BITS )
                   {  do {
                         case 0:   if (hi >= size)  goto out0;
                                   sieve[hi] |=  1;  hi += ji;
                         case 4:   if (hi >= size)  goto out4;
                                   sieve[hi] |= 16;  hi += i;
                         case 2:   if (hi >= size)  goto out2;
                                   sieve[hi] |=  4;  hi += js-1;
                         case 5:   if (hi >= size)  goto out5;
                                   sieve[hi] |= 32;  hi += i;
                         case 3:   if (hi >= size)  goto out3;
                                   sieve[hi] |=  8;  hi += ji;
                         case 7:   if (hi >= size)  goto out7;
                                   sieve[hi] |=128;  hi += js;
                         case 1:   if (hi >= size)  goto out1;
                                   sieve[hi] |=  2;  hi += ji;
                         case 6:   if (hi >= size)  goto out6;
                                   sieve[hi] |= 64;  hi += js;
                         lim= hj-19;
                         while ((int)hi < lim)
                         {  sieve[hi] |=  1;  hi += ji;
                            sieve[hi] |= 16;  hi += i;
                            sieve[hi] |=  4;  hi += js-1;
                            sieve[hi] |= 32;  hi += i;
                            sieve[hi] |=  8;  hi += ji;
                            sieve[hi] |=128;  hi += js;
                            sieve[hi] |=  2;  hi += ji;
                            sieve[hi] |= 64;  hi += js;
                         }
                      } while (1);
                   }
         case 6:   js+=1;  i+=5;  ji+=3;
                   switch( index[h]%BITS )
                   {  do {
                         case 5:   if (hi >= size)  goto out5;
                                   sieve[hi] |= 32;  hi += i;
                         case 1:   if (hi >= size)  goto out1;
                                   sieve[hi] |=  2;  hi += js;
                         case 6:   if (hi >= size)  goto out6;
                                   sieve[hi] |= 64;  hi += i;
                         case 2:   if (hi >= size)  goto out2;
                                   sieve[hi] |=  4;  hi += ji;
                         case 3:   if (hi >= size)  goto out3;
                                   sieve[hi] |=  8;  hi += js;
                         case 7:   if (hi >= size)  goto out7;
                                   sieve[hi] |=128;  hi += ji+1;
                         case 0:   if (hi >= size)  goto out0;
                                   sieve[hi] |=  1;  hi += js;
                         case 4:   if (hi >= size)  goto out4;
                                   sieve[hi] |= 16;  hi += ji;
                         lim= hj-23;
                         while ((int)hi < lim)
                         {  sieve[hi] |= 32;  hi += i;
                            sieve[hi] |=  2;  hi += js;
                            sieve[hi] |= 64;  hi += i;
                            sieve[hi] |=  4;  hi += ji;
                            sieve[hi] |=  8;  hi += js;
                            sieve[hi] |=128;  hi += ji+1;
                            sieve[hi] |=  1;  hi += js;
                            sieve[hi] |= 16;  hi += ji;
                         }
                      } while (1);
                   }
         case 7:   js+=2;  i+=6;  ji+=4;
                   switch( index[h]%BITS )
                   {  do {
                         case 0:   if (hi >= size)  goto out0;
                                   sieve[hi] |=  1;  hi += js-1;
                         case 7:   if (hi >= size)  goto out7;
                                   sieve[hi] |=128;  hi += i;
                         case 6:   if (hi >= size)  goto out6;
                                   sieve[hi] |= 64;  hi += ji;
                         case 5:   if (hi >= size)  goto out5;
                                   sieve[hi] |= 32;  hi += js;
                         case 4:   if (hi >= size)  goto out4;
                                   sieve[hi] |= 16;  hi += ji;
                         case 3:   if (hi >= size)  goto out3;
                                   sieve[hi] |=  8;  hi += js;
                         case 2:   if (hi >= size)  goto out2;
                                   sieve[hi] |=  4;  hi += ji;
                         case 1:   if (hi >= size)  goto out1;
                                   sieve[hi] |=  2;  hi += i;
                         lim= hj-29;
                         while ((int)hi < lim)
                         {  sieve[hi] |=  1;  hi += js-1;
                            sieve[hi] |=128;  hi += i;
                            sieve[hi] |= 64;  hi += ji;
                            sieve[hi] |= 32;  hi += js;
                            sieve[hi] |= 16;  hi += ji;
                            sieve[hi] |=  8;  hi += js;
                            sieve[hi] |=  4;  hi += ji;
                            sieve[hi] |=  2;  hi += i;
                         }
                      } while (1);
                   }
      }
      out0:  index[h] = 0;  goto out;
      out1:  index[h] = 1;  goto out;
      out2:  index[h] = 2;  goto out;
      out3:  index[h] = 3;  goto out;
      out4:  index[h] = 4;  goto out;
      out5:  index[h] = 5;  goto out;
      out6:  index[h] = 6;  goto out;
      out7:  index[h] = 7;  goto out;
      out:
      hi -= size;
      index[h] |= BITS*hi;
#ifdef  true_64bit_words
#undef  hi
#endif
   }
/*** output of remaining (prime) numbers ***/
   i=(start<=hh ? 0 : (start-hh)/LCM);
   hh += LCM*i;
   bi= sieve[i];
   switch (start<=hh ? 0 : (BITS*(unsigned)(start-hh))/LCM)
   {
      for (;i<size;i++)
      {  bi= sieve[i];
      case 0:
         if (!(bi&1))
         {  ii= hh+1;
            if (ii > stop)  goto end;
            use(ii);
         }
      case 1:
         if (!(bi&2))
         {  ii= hh+7;
            if (ii > stop)  goto end;
            use(ii);
         }
      case 2:
         if (!(bi&4))
         {  ii= hh+11;
            if (ii > stop)  goto end;
            use(ii);
         }
      case 3:
         if (!(bi&8))
         {  ii= hh+13;
            if (ii > stop)  goto end;
            use(ii);
         }
      case 4:
         if (!(bi&16))
         {  ii= hh+17;
            if (ii > stop)  goto end;
            use(ii);
         }
      case 5:
         if (!(bi&32))
         {  ii= hh+19;
            if (ii > stop)  goto end;
            use(ii);
         }
      case 6:
         if (!(bi&64))
         {  ii= hh+23;
            if (ii > stop)  goto end;
            use(ii);
         }
      case 7:
         if (!(bi&128))
         {  ii= hh+29;
            if (ii > stop)  goto end;
            use(ii);
         }
         hh += LCM;
         sieve[i]=0;
      }
   }
}
end:
free(index);
free(sieve);
finish:
fprintf(out, "# {"UL" <= primes <= "UL"} = "UL"\n",start,stop,anz);
#ifdef  TEST
printf("allocated memory: %.3lf MB\n",((sizeof(LONG)+1)*psize+size*sizeof(unsigned char))/1024.0/1024.0);
#elif  defined(DEBUG)
#else
fprintf(out, "%.12lf  %.12lf\n",sum,(sum-(log(log((double)stop))+0.5772156649015-0.315718451893))*sqrt((double)stop));
#endif
return  anz;
}



int generate(int under, FILE *out) {
  char * name = "primes";
  char param [50];
  sprintf(param, "%d", under);
  char * values[] = { name, param};
  generate_args(2, values, out);
}
