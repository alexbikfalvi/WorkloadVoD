#include "stdafx.h"
#include "DistExp.h"
#include <math.h>

double DistExp(double x, double mu)
{
	return 1.0 - exp(-mu*x);
}

double DistExpInv(double x, double mu)
{
	return -log(1.0-x)/mu;
}
