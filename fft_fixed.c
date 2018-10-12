#include <stdio.h>
#include <stdlib.h>
#include <math.h>

int bits = 16, int_bits = 0,deci_bits = 0;

struct complex
{
	int real;
	int img;
};

// lookup table
double w_float_real[32] = {1.000000,0.995185,0.980785,0.956940,0.923880,0.881921,0.831470,0.773010,0.707107,0.634393,0.555570,0.471397,0.382683,0.290285,0.195090,0.098017,0.000000,-0.098017,-0.195090,-0.290285,-0.382683,-0.471397,-0.555570,-0.634393,-0.707107,-0.773010,-0.831470,-0.881921,-0.923880,-0.956940,-0.980785,-0.995185};
double w_float_img[32] = {0.000000,-0.098017,-0.195090,-0.290285,-0.382683,-0.471397,-0.555570,-0.634393,-0.707107,-0.773010,-0.831470,-0.881921,-0.923880,-0.956940,-0.980785,-0.995185,-1.000000,-0.995185,-0.980785,-0.956940,-0.923880,-0.881921,-0.831470,-0.773010,-0.707107,-0.634393,-0.555570,-0.471397,-0.382683,-0.290285,-0.195090,-0.098017};
struct complex w_fixed[32];

// finding log to base 2
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

// rounding a number
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

// addition of complex number
struct complex complex_add(struct complex a , struct complex b)
{
	struct complex added;
	added.real = a.real + b.real;
	added.img = a.img + b.img;

	return added;
}

// subtraction of complex number
struct complex complex_subtract(struct complex a , struct complex b)
{
	struct complex sub;
	sub.real = a.real - b.real;
	sub.img = a.img - b.img;

	return sub;
}

// multiplication of complex number
struct complex complex_multiply(struct complex a , struct complex b)
{
	struct complex mul;
	mul.real = (a.real * b.real - a.img *b.img) / (double)(1 << deci_bits);
	mul.img = (a.real * b.img + a.img *b.real) / (double)(1 << deci_bits);

	return mul;
}

// converting a number into complex (structure)
struct complex make_complex(int x , int y)
{
	struct complex cmplx;
	cmplx.real = x;
	cmplx.img = y;

	return cmplx;
}

// finding fft using ‎Cooley–Tukey FFT algorithm
struct complex* fft(struct complex* a,int  n)
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
		a_even = fft(a_even , n/2);
		a_odd = fft(a_odd , n/2);
		for(int k = 0 ; k<n/2 ; k++)
		{
			output[k] = complex_add(a_even[k] , complex_multiply( w_fixed[k*64/n] , a_odd[k]));
			output[k + (n/2)] = complex_subtract(a_even[k] , complex_multiply( w_fixed[k*64/n] , a_odd[k]));
		}
		return output;
	}
}

int main()
{
	// input
	// double input_float_real[] = {0.1,0.2,0.3,0.4,0.5,0.6,0.7,0.8,0.9,1.0,1.1,1.2,1.3,1.4,1.5,1.6,0.1,0.2,0.3,0.4,0.5,0.6,0.7,0.8,0.9,1.0,1.1,1.2,1.3,1.4,1.5,1.6,0.1,0.2,0.3,0.4,0.5,0.6,0.7,0.8,0.9,1.0,1.1,1.2,1.3,1.4,1.5,1.6,0.1,0.2,0.3,0.4,0.5,0.6,0.7,0.8,0.9,1.0,1.1,1.2,1.3,1.4,1.5,1.6};
	// double input_float_img[] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
	double input_float_real[] = { 0.92670136, 0.92526534, 1.06134841, -0.8881673, 0.0970546, 1.89288216, -0.15537744, 0.17452029, -1.16409703, -0.62238235, 1.73710805, 0.87225893, 1.68857848, 2.19616514, 0.92045271, 1.81368249, 1.76766088, 2.37320675, 0.12441137, 0.69430363, 4.10958044, 2.9494473, 0.98999581, 1.86393524, 0.94992101, 0.58780095, 1.65149978, 1.48801442, -0.76901673, -0.38560469, -0.27367721, 1.07830558, 0.41459356, 0.51731829, 1.51016401, 1.8491849, 1.80154993, -0.17776092, 0.80802265, -0.77624968, 1.13077533, 1.41831872, 1.49171826, 1.66977257, 1.70925721, 1.15018871, 0.44849218, 2.45037054, 2.11556392, -0.95568807, 3.02255698, 1.78621665, -0.69232579, 0.77796521, 3.26874094, 0.17147518, 2.47544443, -0.01667211, 1.05575585, 1.98515679, 1.97223382, 2.0646376, -0.80155675, 1.25385524 };
	double input_float_img[] = { 0.65980587, 1.83689466, 0.64897778, 1.35691023, 0.75463888, 0.45913017, 0.58907004, -1.21947475, 2.23889855, 1.53966982, 0.37422832, -0.08597866, 0.20838714, 2.17532383, -0.2233321, 1.56531937, 2.63987056, 1.51840256, 1.57200803, 0.24761504, -0.35231468, 1.63810255, 0.8283496, 1.20943272, 2.11829619, 1.23173226, 0.64046625, -1.18849812, 1.72744721, 1.09235085, 3.28064847, 1.38242922, 1.40439443, 1.63716572, 1.87952573, 0.72300287, 1.25325819, 0.42500207, -0.76676234, 0.76137775, 0.98722207, 2.14538474, 0.37760292, 2.46963168, 1.10771446, 1.80038362, 1.33840776, 2.10510554, -1.10364182, 1.08624815, 0.02671055, 3.91015891, 0.86348538, 1.9817514, 1.82263764, 0.5383923, 0.78279545, 1.06524052, 1.58682072, 1.51115433, 1.53869124, 1.41987451, 0.51629822, 1.25623659};

	int temp=0;

	int n = (int)(sizeof(input_float_real)/sizeof(double));
	int *input_fixed_real = (int *)malloc(n * sizeof(int));
	int *input_fixed_img = (int *)malloc(n * sizeof(int));
	printf("Size of the given array is %d\n", n);

	// finding q format
	temp = input_float_real[0];
	for(int i = 1; i < n;i++)
	{
		if(abs(input_float_real[i])>temp)	temp = abs(input_float_real[i]);
		if(abs(input_float_img[i])>temp)	temp = abs(input_float_img[i]);
	}

	int_bits = log_2(temp) + log_2(n);
	deci_bits = bits - int_bits;
	printf("bits allocated for integer: %d\n",int_bits);
	printf("bits allocated for fraction: %d\n",deci_bits);


	//converting to fixed point
	for(int i = 0; i < n; i++)
	{
		input_fixed_real[i] = rounded(input_float_real[i] * (1 << deci_bits));
		input_fixed_img[i] = rounded(input_float_img[i] * (1 << deci_bits));
	}

	// converting the lookup table to fixed point
	for(int i = 0; i < 32; i++)
	{
		w_fixed[i].real = w_float_real[i] * (1 << deci_bits);
		w_fixed[i].img = w_float_img[i] * (1 << deci_bits);
	}

  struct complex* complex_input;
  complex_input = (struct complex *)malloc(n * sizeof(struct complex));

  for(int i = 0 ; i < n ; i++)	complex_input[i] = make_complex(input_fixed_real[i],input_fixed_img[i]);

	// computing the fast fourier transform
  struct complex* output_fixed = fft(complex_input , n);
	double* output_float_real = (double *)malloc(n * sizeof(double));
	double* output_float_img = (double *)malloc(n * sizeof(double));

	// converting them back to float
	for(int i = 0; i < n; i++)
	{
		output_float_real[i] = (output_fixed[i].real / (double)(1 << deci_bits));
		output_float_img[i] = (output_fixed[i].img / (double)(1 << deci_bits));
	}

	// printing the outputs
	for(int i = 0 ; i < n ; i++)
	{
		if(output_fixed[i].img >= 0)	printf("%d +%d i 		", output_fixed[i].real , output_fixed[i].img);
		else	printf("%d %d i 		", output_fixed[i].real , output_fixed[i].img);
		if(output_float_img[i] >= 0)	printf("%lf +%lf i \n", output_float_real[i], output_float_img[i]);
		else	printf("%lf %lf i \n", output_float_real[i] , output_float_img[i]);
	}
}
