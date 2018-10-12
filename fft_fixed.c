#include <stdio.h>
#include <stdlib.h>
#include <math.h>

int bits = 16, int_bits = 0,deci_bits = 0;

struct complex
{
	int real;
	int img;
	// double real;
	// double img;
};

int log_2(int x)
{
	int count = 0;
	while(x/2 != 0)
	{
		count++;
		x = x/2;
	}
	return count + 1;
}

int rounded(float x)
{
	if(x > 0)
	{
		if(x - (int)x < 0.5)	return (int)x;
		else	return (int)x + 1;
	}
	else
	{
		if(x - (int)x > 0.5)	return (int)x;
		else	return (int)x - 1;
	}

}

struct complex complex_add(struct complex a , struct complex b)
{
	struct complex added;
	added.real = a.real + b.real;
	added.img = a.img + b.img;

	return added;
}

struct complex complex_subtract(struct complex a , struct complex b)
{
	struct complex sub;
	sub.real = a.real - b.real;
	sub.img = a.img - b.img;

	return sub;
}

struct complex complex_multiply(struct complex a , struct complex b)
{
	struct complex mul;
	mul.real = (a.real * b.real - a.img *b.img) / (double)(1 << deci_bits);
	mul.img = (a.real * b.img + a.img *b.real) / (double)(1 << deci_bits);

	return mul;
}

struct complex make_complex(int input)
{
	struct complex cmplx;
	cmplx.real = input;
	cmplx.img = 0;

	return cmplx;
}

struct complex complex_power(struct complex w , int power)
{
	struct complex ans;
	ans.real = 1 * (1 << deci_bits);
	ans.img = 0 * (1 << deci_bits);
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

struct complex* fft(struct complex* a,int  n ,struct complex w)
{
	if(n == 1)	return a;
	else
	{
		struct complex *a_even , *a_odd , *output;
	    a_even = (struct complex *)malloc(n/2 * sizeof(struct complex));
	    a_odd = (struct complex *)malloc(n/2 * sizeof(struct complex));
	    output = (struct complex *)malloc(n * sizeof(struct complex));

		for(int i = 0 ; i < n/2 ; i++)
		{
			a_even[i] = a[2*i];
			a_odd[i] = a[2*i + 1];
		}
		a_even = fft(a_even , n/2 , complex_power(w , 2));
		a_odd = fft(a_odd , n/2 , complex_power(w , 2));
		for(int k = 0 ; k<n/2 ; k++)
		{
			output[k] = complex_add(a_even[k] , complex_multiply( complex_power(w , k) , a_odd[k]));
			output[k + (n/2)] = complex_subtract(a_even[k] , complex_multiply( complex_power(w , k) , a_odd[k]));
		}
		return output;
	}
}

int main()
{
	double input_float[] = {0.1,0.2,0.3,0.4,0.5,0.6,0.7,0.8,0.9,1.0,1.1,1.2,1.3,1.4,1.5,1.6};
	int temp=0;
	int n = (int)(sizeof(input_float)/sizeof(double));
	int *input_fixed = (int *)malloc(n * sizeof(int));
	printf("Size of the given array is %d\n", n);

	temp = input_float[0];
	for(int i = 1; i < n;i++)	if(abs(input_float[i])>temp)	temp = abs(input_float[i]);

	int_bits = log_2(temp) + log_2(n);
	deci_bits = bits - int_bits;
	printf("bits allocated for integer: %d\n",int_bits);
	printf("bits allocated for decimal: %d\n",deci_bits);


	//converting to fixed point
	for(int i = 0; i < n; i++)
	{
		input_fixed[i] = rounded(input_float[i] * (1 << deci_bits));
	}

  struct complex w;
  w.real = rounded(cos(2*M_PI/n) * (1 << deci_bits));
  w.img = rounded(-sin(2*M_PI/n) * (1 << deci_bits));


  struct complex* complex_input;
  complex_input = (struct complex *)malloc(n * sizeof(struct complex));

  for(int i = 0 ; i < n ; i++)	complex_input[i] = make_complex(input_fixed[i]);

  struct complex* output_fixed = fft(complex_input , n , w);
	double* output_float_real = (double *)malloc(n * sizeof(double));
	double* output_float_img = (double *)malloc(n * sizeof(double));

	for(int i = 0; i < n; i++)
	{
		output_float_real[i] = (output_fixed[i].real / (double)(1 << deci_bits));
		output_float_img[i] = (output_fixed[i].img / (double)(1 << deci_bits));
	}
	for(int i = 0 ; i < n ; i++)
	{
		if(output_fixed[i].img >= 0)	printf("%d +%d i 		", output_fixed[i].real , output_fixed[i].img);
		else	printf("%d %d i 		", output_fixed[i].real , output_fixed[i].img);
		if(output_float_img[i] >= 0)	printf("%lf +%lf i \n", output_float_real[i], output_float_img[i]);
		else	printf("%lf %lf i \n", output_float_real[i] , output_float_img[i]);
	}
}
