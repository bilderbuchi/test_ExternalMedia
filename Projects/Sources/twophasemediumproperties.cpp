#include "twophasemediumproperties.h"


// ! Constructor
/*!
  The constructor is calling initializeFields.
*/
TwoPhaseMediumProperties::TwoPhaseMediumProperties(const int nComp){
    // Initialize entire class
	initializeFields(nComp);
}

// ! Initialize all fields to the default/NAN values
/*!
  This function initializes all fields of TwoPhaseMediumProperties to the
  NAN value defined in include.h. The phase is initialized to zero.
*/
void TwoPhaseMediumProperties::initializeFields(const int nComp){
	phase = 0;

	for (int i = 0; i < nComp; i++) {
		x[i] = NAN;
		y[i] = NAN;
	}
	beta = NAN;
	cp = NAN;
	cv = NAN;
	d = NAN;
	dd_dp_h = NAN;
	dd_dh_p = NAN;
	h = NAN;
	kappa = NAN;
	p = NAN;
	s = NAN;
	T = NAN;
	dT_dh_p = NAN;
	dT_dp_h = NAN;

	ps = NAN;
	Ts = NAN;

	dl = NAN;
	dv = NAN;
	hl = NAN;
	hv = NAN;
	sl = NAN;
	sv = NAN;

	eta = NAN;
	lambda = NAN;
	Pr = NAN;
	sigma = NAN;

	a = NAN;

    d_Ts_dp = NAN;
	d_dl_dp = NAN;
	d_dv_dp = NAN;
    d_hl_dp = NAN;
	d_hv_dp = NAN;
}
