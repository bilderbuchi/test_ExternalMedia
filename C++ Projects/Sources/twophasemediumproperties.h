#ifndef TWOPHASEMEDIUMPROPERTIES_H_
#define TWOPHASEMEDIUMPROPERTIES_H_

#include "include.h"

//! Two phase medium property struct
/*!
  The two phase medium propery struct defines all the properties that
  are computed by external Modelica medium models extending from
  PartialExternalTwoPhaseMedium. It also contains the saturation properties
  for the dew and the bubble line which makes it a real two phase medium
  property struct.

  Francesco Casella, Christoph Richter Sep 2006
*/
struct TwoPhaseMediumProperties{
	//! Medium name
	string mediumName;
	//! Name of the external library
	string libraryName;
	//! Substance name
	string substanceName;
	// ! Unique ID number
	/*!
	  The unique ID number is included in the two phase medium property
	  record for error reporting reasons.
	*/
	int uniqueID;

	//! Phase input
	/*!
	  This phase flag is defined according to the phase flag in Modelica.Media:
	  2 for two-phase, 1 for one-phase, 0 if not known.
	*/
	int phase;

	// ! Unique ID of corresponding 1-phase dew state medium object, set by setDewState
	int dewUniqueIDOnePhase;

	// ! Unique ID of corresponding 2-phase dew state medium object, set by setDewState
	int dewUniqueIDTwoPhase;

	// ! Unique ID of corresponding 1-phase bubble state medium object, set by setBubbleState
	int bubbleUniqueIDOnePhase;

	// ! Unique ID of corresponding 2-phase bubble state medium object, set by setBubbleState
	int bubbleUniqueIDTwoPhase;

	//! Isothermal expansion coefficient
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
	//! Compressibility
	double kappa;
	//! Pressure
	double p;
	//! Specific entropy
	double s;
	//! Temperature
	double T;

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
};

#endif /*TWOPHASEMEDIUMPROPERTIES_H_*/
