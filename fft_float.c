#include <stdio.h>
#include <stdlib.h>
#include <math.h>

// defining a complex number as structure
struct complex
{
	double real;
	double img;
};

// adding two complex numbers
struct complex complex_add(struct complex a , struct complex b)
{
	struct complex added;
	added.real = a.real + b.real;
	added.img = a.img + b.img;

	return added;
}

// subtracting two complex numbers
struct complex complex_subtract(struct complex a , struct complex b)
{
	struct complex sub;
	sub.real = a.real - b.real;
	sub.img = a.img - b.img;

	return sub;
}

// multiplying two complex numbers
struct complex complex_multiply(struct complex a , struct complex b)
{
	struct complex mul;
	mul.real = a.real * b.real - a.img *b.img;
	mul.img = a.real * b.img + a.img *b.real;

	return mul;
}

// making some number to complex structure
struct complex make_complex(double x , double y)
{
	struct complex cmplx;
	cmplx.real = x;
	cmplx.img = y;

	return cmplx;
}

// compting complexnumber to the power of a integer
struct complex complex_power(struct complex w , int power)
{
	struct complex ans;
	ans.real = 1;
	ans.img = 0;
	if(power == 0)	return ans;
	else
	{
		ans = w;
		for(int i = 1 ; i<power;i++)
		{
			ans = complex_multiply(ans , w);
		}
		return ans;
	}
}

// function to compute fft
struct complex* fft(struct complex* a,int  n ,struct complex w)
{
	if(n == 1)	return a;	//returning the number (one-point dft is itself)
	else
	{
		struct complex *a_even , *a_odd , *output;

    a_even = (struct complex *)malloc(n/2 * sizeof(struct complex));
    a_odd = (struct complex *)malloc(n/2 * sizeof(struct complex));
    output = (struct complex *)malloc(n * sizeof(struct complex));
		struct complex wn;
		wn.real = 1;
		wn.img = 0;

		//seperating odd and even terms
		for(int i = 0 ; i < n/2 ; i++)
		{
			a_even[i] = a[2*i];
			a_odd[i] = a[2*i + 1];
		}
		// calling he function recursively
		a_even = fft(a_even , n/2 , complex_multiply(w , w));
		a_odd = fft(a_odd , n/2 , complex_multiply(w , w));
		for(int k = 0 ; k<n/2 ; k++)
		{
			output[k] = complex_add(a_even[k] , complex_multiply( wn , a_odd[k]));
			output[k + (n/2)] = complex_subtract(a_even[k] , complex_multiply( wn , a_odd[k]));
			wn = complex_multiply(w, wn);
		}
		return output;	//returning the output
	}
}

int main()
{
	double input[] = {1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16};
	int n = (int)(sizeof(input)/sizeof(double));
  printf("Size of the given array is %d\n", n);

	// initializing wn
  struct complex w;
  w.real = cos(2*M_PI/n);
  w.img = -sin(2*M_PI/n);

  struct complex* complex_input;
  complex_input = (struct complex *)malloc(n * sizeof(struct complex));

  for(int i = 0 ; i < n ; i++)	complex_input[i] = make_complex(input[i] , 0);

  struct complex* output = fft(complex_input , n , w);

	// printing the outputs
	for(int i = 0 ; i < n ; i++)
	{
		if(output[i].img >= 0)	printf("%lf +%lfi \n", output[i].real , output[i].img);
		else	printf("%lf %lfi \n", output[i].real , output[i].img);
	}
}
