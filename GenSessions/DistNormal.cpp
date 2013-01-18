#include "stdafx.h"
#include "DistNormal.h"
//#include "Erf.h"

#define SQRT_2		1.414213562373095
#define SQRT_2PI	2.50662827463100
#define	SQRT_PI_INV	0.564189583547756

#define MIN(a,b)	((a<b)?a:b)

const double distStdNormalCoefA[5] = {
	1.161110663653770e-002,
	3.951404679838207e-001,
	2.846603853776254e+001,
	1.887426188426510e+002,
	3.209377589138469e+003
};
const double distStdNormalCoefB[5] = {
	1.767766952966369e-001,
	8.344316438579620e+000,
	1.725514762600375e+002,
	1.813893686502485e+003,
	8.044716608901563e+003
};
const double distStdNormalCoefC[9] = {
	2.15311535474403846e-8,
	5.64188496988670089e-1,
	8.88314979438837594e00,
	6.61191906371416295e01,
	2.98635138197400131e02,
	8.81952221241769090e02,
	1.71204761263407058e03,
	2.05107837782607147e03,
	1.23033935479799725E03
};
const double distStdNormalCoefD[9] = {
	1.00000000000000000e00,
	1.57449261107098347e01,
	1.17693950891312499e02,
	5.37181101862009858e02,
	1.62138957456669019e03,
	3.29079923573345963e03,
	4.36261909014324716e03,
	3.43936767414372164e03,
	1.23033935480374942e03
};
const double distStdNormalCoefP[6] = {
	1.63153871373020978e-2,
	3.05326634961232344e-1,
	3.60344899949804439e-1,
	1.25781726111229246e-1,
	1.60837851487422766e-2,
	6.58749161529837803e-4
};
const double distStdNormalCoefQ[6] = {
	1.00000000000000000e00,
	2.56852019228982242e00,
	1.87295284992346047e00,
	5.27905102951428412e-1,
	6.05183413124413191e-2,
	2.33520497626869185e-3
};

const double distStdNormalInvCoefA[6] = {
	-3.969683028665376e+01,  2.209460984245205e+02,
	-2.759285104469687e+02,  1.383577518672690e+02,
	-3.066479806614716e+01,  2.506628277459239e+00
};
const double distStdNormalInvCoefB[5] = {
	-5.447609879822406e+01,  1.615858368580409e+02,
	-1.556989798598866e+02,  6.680131188771972e+01,
	-1.328068155288572e+01
};
const double distStdNormalInvCoefC[6] = {
	-7.784894002430293e-03, -3.223964580411365e-01,
	-2.400758277161838e+00, -2.549732539343734e+00,
	4.374664141464968e+00,  2.938163982698783e+00
};
const double distStdNormalInvCoefD[4] = {
	7.784695709041462e-03,  3.224671290700398e-01,
	2.445134137142996e+00,  3.754408661907416e+00
};

double DistNormal(double x, double mu, double sigma)
{
	return DistStdNormal((x-mu)/sigma);
}

double DistNormalInv(double x, double mu, double sigma)
{
	return mu + DistStdNormalInv(x) * sigma;
}

double DistStdNormal(double x)
{
	register double y, z;

	y = fabs(x);
	if (y <= 0.46875*SQRT_2)
	{
		/* evaluate erf() for |u| <= sqrt(2)*0.46875 */
		z = y*y;
		y = x*((((distStdNormalCoefA[0]*z+distStdNormalCoefA[1])*z+distStdNormalCoefA[2])*z+distStdNormalCoefA[3])*z+distStdNormalCoefA[4])/
			((((distStdNormalCoefB[0]*z+distStdNormalCoefB[1])*z+distStdNormalCoefB[2])*z+distStdNormalCoefB[3])*z+distStdNormalCoefB[4]);
		return 0.5+y;
	}
	
	z = exp(-y*y/2)/2;
	if (y <= 4.0)
	{
		/* evaluate erfc() for sqrt(2)*0.46875 <= |u| <= sqrt(2)*4.0 */
		y = y/SQRT_2;
		y =	((((((((distStdNormalCoefC[0]*y+distStdNormalCoefC[1])*y+distStdNormalCoefC[2])*y+distStdNormalCoefC[3])*y+distStdNormalCoefC[4])*y+distStdNormalCoefC[5])*y+distStdNormalCoefC[6])*y+distStdNormalCoefC[7])*y+distStdNormalCoefC[8])/
			((((((((distStdNormalCoefD[0]*y+distStdNormalCoefD[1])*y+distStdNormalCoefD[2])*y+distStdNormalCoefD[3])*y+distStdNormalCoefD[4])*y+distStdNormalCoefD[5])*y+distStdNormalCoefD[6])*y+distStdNormalCoefD[7])*y+distStdNormalCoefD[8]);

		y = z*y;
	}
	else
	{
		/* evaluate erfc() for |u| > sqrt(2)*4.0 */
		z = z*SQRT_2/y;
		y = 2/(y*y);
		y = y*(((((distStdNormalCoefP[0]*y+distStdNormalCoefP[1])*y+distStdNormalCoefP[2])*y+distStdNormalCoefP[3])*y+distStdNormalCoefP[4])*y+distStdNormalCoefP[5])/
			(((((distStdNormalCoefQ[0]*y+distStdNormalCoefQ[1])*y+distStdNormalCoefQ[2])*y+distStdNormalCoefQ[3])*y+distStdNormalCoefQ[4])*y+distStdNormalCoefQ[5]);
		y = z*(SQRT_PI_INV-y);
	}
	
	return (x < 0.0 ? y : 1-y);
};


double DistStdNormalInv(double p)
{
	register double q, t, u;

	if(p == 0.0) return -1E300;
	if(p == 1.0) return 1E300;

	q = MIN(p,1-p);
	if (q > 0.02425)
	{
		/* Rational approximation for central region. */
		u = q-0.5;
		t = u*u;
		u = u*(((((distStdNormalInvCoefA[0]*t+distStdNormalInvCoefA[1])*t+distStdNormalInvCoefA[2])*t+distStdNormalInvCoefA[3])*t+distStdNormalInvCoefA[4])*t+distStdNormalInvCoefA[5])
		/(((((distStdNormalInvCoefB[0]*t+distStdNormalInvCoefB[1])*t+distStdNormalInvCoefB[2])*t+distStdNormalInvCoefB[3])*t+distStdNormalInvCoefB[4])*t+1);
	}
	else
	{
		/* Rational approximation for tail region. */
		t = sqrt(-2*log(q));
		u = (((((distStdNormalInvCoefC[0]*t+distStdNormalInvCoefC[1])*t+distStdNormalInvCoefC[2])*t+distStdNormalInvCoefC[3])*t+distStdNormalInvCoefC[4])*t+distStdNormalInvCoefC[5])
		/((((distStdNormalInvCoefD[0]*t+distStdNormalInvCoefD[1])*t+distStdNormalInvCoefD[2])*t+distStdNormalInvCoefD[3])*t+1);
	}
	
	/* The relative error of the approximation has absolute value less
	than 1.15e-9.  One iteration of Halley's rational method (third
	order) gives full machine precision */

	t = DistStdNormal(u)-q;			// error
	t = t*SQRT_2PI*exp(u*u/2);		// f(u)/df(u)
	u = u-t/(1+u*t/2);				// Halley's method

	return (p > 0.5 ? -u : u);
}
