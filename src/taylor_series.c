#include <stdio.h>
#include <math.h>
#include <time.h>
#include <stdlib.h>

#define TWO_PI (2*M_PI)

double const inv_factorials[20] = {1., 1., 1./2, 1./6, 1./24, 1./120, 1./720, 1./5040, 
							1./40320, 1./362880, 1./3628800, 1./39916800, 
							1./479001600, 1./6227020800, 1./87178291200, 
							1./1307674368000, 1./20922789888000, 
							1./355687428096000, 1./6402373705728000, 
							1./121645100408832000};
unsigned int const fact_length = (int)(sizeof(inv_factorials)/sizeof(inv_factorials[0]));

// precision is ass, dont @ me
double cosine(double x){
	x -= (int)(x / TWO_PI) * TWO_PI;	// <=> fmod(x, TWO_PI); mapping all values to intervall [0, 2pi)
	x *= x;								// prep for later use due to x^(2*i) = (x^2)^i
	
	// using taylor series to calculate cosine
	double sign_val = 1., pwr_x = 1., res = 0.;
	for(unsigned int i = 0; i<fact_length; i+=2){
		res += sign_val * pwr_x * inv_factorials[i];	// <=> sum_{i=0}^n (-1)^i * x^(2*i) / (2*i)! or 1 - x^2/2! + x^4/4! - x^6/6! + ...

		sign_val *= -1;
		pwr_x *= x;
	}
	
	return res;
}

// analogous to cosine
double sine(double x){
	x -= (int)(x / TWO_PI) * TWO_PI;

	double sign_val = 1., pwr_x = x, res = 0.;
	x *= x;
	for(unsigned int i = 1; i<fact_length; i+=2){
		res += sign_val * pwr_x * inv_factorials[i];// <=> sum_{i=0}^n (-1)^i * x^(2*i+1)/(2*i+1)! or 1 - x^3/3! + x^5/5! - x^7/7! + ...

		sign_val *= -1;
		pwr_x *= x;
	}

	return res;
}

double avg_func_time(int steps, double (*func)(double)){
	double avg_time = 0;

	for(int i = 0; i<steps*(int)(M_PI); i++){
		clock_t begin = clock();
		(*func)(i/steps);
		clock_t end = clock();
		avg_time += (double)(end - begin) / CLOCKS_PER_SEC;
	}

	return avg_time /= (steps*(int)(M_PI));
}

int main(void){
	clock_t start_overall = clock();
	double avg_time = avg_func_time(200000, &cosine);
	clock_t end_overall = clock();
	double time_overall = (double)(end_overall - start_overall) / CLOCKS_PER_SEC;

	printf("average cosine time: %0.11f seconds\n", avg_time);
	printf("total time: %0.11f seconds\n", time_overall);

	srand(time(NULL));
	double x = 2;
	printf("\ncosine(%0.3f) = %0.11f\n", x, cosine(x));
	printf("sin(%0.3f) = %0.11f\n", x, sine(x));

	return 0;
}