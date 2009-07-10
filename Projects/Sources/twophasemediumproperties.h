#ifndef TWOPHASEMEDIUMPROPERTIES_H_
#define TWOPHASEMEDIUMPROPERTIES_H_

#include "include.h"

//! Two phase medium property class
/*!
  The two phase medium propery struct defines all the properties that
  are computed by external Modelica medium models extending from
  PartialExternalTwoPhaseMedium. It also contains the saturation properties
  for the dew and the bubble line which makes it a real two phase medium
  property struct.

  Francesco Casella, Christoph Richter, Sep 2006
  Copyright Politecnico di Milano and TU Braunschweig
  modified by Andreas Joos (Hamburg University of Technology), Jun 2009
*/
class TwoPhaseMediumProperties{
public:
	TwoPhaseMediumProperties(const int nComp = 1);
	void initializeFields(const int nComp = 1);

	//! Phase 
	/*!
	  This phase flag is defined according to the phase flag in Modelica.Media:
	  2 for two-phase, 1 for one-phase.
	*/
	int phase;

	//! Specific liquid phase composition
	double x[MAX_NO_COMPONENTS];
	//! Specific vapor phase composition
	double y[MAX_NO_COMPONENTS];

	//! Isobaric expansion coefficient
	double beta;
	//! Specific heat capacity cp
	double cp;
	//! Specific heat capacity cv
	double cv;
	//! Density
	double d;
	//! Derivative of density wrt pressure at constant enthalpy
	double dd_dp_h;
	//! Derivative of density wrt enthalpy at constant pressure
	double dd_dh_p;
	//! Specific enthalpy
	double h;
	//! Specific internal energy
	double u;
	//! Compressibility
	double kappa;
	//! Pressure
	double p;
	//! Specific entropy
	double s;
	//! Temperature
	double T;
	//! Derivative of temperature wrt pressure at constant enthalpy
	double dT_dp_h;
	//! Derivative of temperature wrt enthalpy at constant pressure
	double dT_dh_p;

	//! Saturation pressure
	double ps;
	//! Saturation temperature
	double Ts;

	//! Density at bubble line (for pressure ps)
	double dl;
	//! Density at dew line (for pressure ps)
	double dv;
	//! Specific enthalpy at bubble line (for pressure ps)
	double hl;
	//! Specific enthalpy at dew line (for pressure ps)
	double hv;
	//! Specific entropy at bubble line (for pressure ps)
	double sl;
	//! Specific entropy at dew line (for pressure ps)
	double sv;

	//! Dynamic viscosity
	double eta;
	//! Thermal conductivity
	double lambda;
	//! Prandtl number
	double Pr;
	//! Surface tension
	double sigma;

	//! Velocity of sound
	double a;

	//! Derivative of Ts wrt pressure
    double d_Ts_dp;
	//! Derivative of dls wrt pressure
	double d_dl_dp;
	//! Derivative of dvs wrt pressure
	double d_dv_dp;
	//! Derivative of hls wrt pressure
    double d_hl_dp;
	//! Derivative of hvs wrt pressure
	double d_hv_dp;

	//! Derivative of u wrt pressure
	double du_dp;
	//! Derivative of u wrt h
	double du_dh;
};

#endif // TWOPHASEMEDIUMPROPERTIES_H_
