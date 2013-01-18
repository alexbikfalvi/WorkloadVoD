#include "stdafx.h"
#include "DistPareto.h"
#include <math.h>

double DistPareto(double x, double k, double alpha)
{
	return 1.0-pow(k/x, alpha);
}

double DistParetoInv(double x, double k, double alpha)
{
	return k/pow(1.0-x, 1.0/alpha);
}

