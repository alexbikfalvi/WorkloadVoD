#ifndef __Chebyshev_h__
#define __Chebyshev_h__

struct SChebyshevSeries {
	double*		c;			// coefficients
	int			order;		// order of expansion
	double		a;			// lower interval point
	double		b;			// upper interval point
	int			order_sp;	// effective single precision order
};

double Chebyshev(SChebyshevSeries* cs, double x);

#endif