#include "stdafx.h"
#include "DistLogn.h"
#include "Erf.h"
#include <math.h>

#define SQRT_2	1.41421356237310

double DistLogn(double x, double mu, double sigma)
{
	return 0.5 + 0.5*Erf((log(x)-mu)/(SQRT_2*sigma));
}

double DistLognInv(double x, double mu, double sigma)
{
	return exp(mu + SQRT_2*sigma*ErfInv(2*x-1));
}

