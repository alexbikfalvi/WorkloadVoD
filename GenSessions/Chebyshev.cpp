#include "stdafx.h"
#include "Chebyshev.h"

double Chebyshev(SChebyshevSeries* cs, double x)
{
	double d = 0.0;
	double dd = 0.0;

	double y = (2.0*x - cs->a - cs->b) / (cs->b - cs->a); 
	double y2 = 2.0 * y;

	double e = 0.0;

	for(unsigned int j = cs->order; j>=1; j--)
	{
		double temp = d;
		d = y2*d - dd + cs->c[j];
		e += fabs(y2*temp) + fabs(dd) + fabs(cs->c[j]);
		dd = temp;
	}

	{ 
		double temp = d;
		d = y*d - dd + 0.5 * cs->c[0];
		e += fabs(y*temp) + fabs(dd) + 0.5 * fabs(cs->c[0]);
	}

	return d;
}
