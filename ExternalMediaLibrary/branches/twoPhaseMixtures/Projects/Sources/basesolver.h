#ifndef BASESOLVER_H_
#define BASESOLVER_H_

#include "include.h"
#include "fluidconstants.h"

class TwoPhaseMediumProperties;
struct FluidConstants;

//! Base solver class.
/*!
  This is the base class for all external solver objects
  (e.g. TestSolver, FluidPropSolver).

  Francesco Casella, Christoph Richter, Sep 2006
  Copyright Politecnico di Milano and TU Braunschweig
  modified by Andreas Joos (Hamburg University of Technology), Jun 2009
*/
class BaseSolver{
public:
	BaseSolver(const string &mediumName, const string &libraryName, const string &substanceName, const int nComp = 1, double* Conc = 0);
	virtual ~BaseSolver();

    virtual void setFluidConstants();
	virtual void setSat_p(double &p, TwoPhaseMediumProperties *const properties, const int nComp = 1);
	virtual void setSat_T(double &T, TwoPhaseMediumProperties *const properties, const int nComp = 1);
	virtual void setSat_p_state(TwoPhaseMediumProperties *const properties);

	virtual void setState_dT(double &d, double &T, int &phase, TwoPhaseMediumProperties *const properties, const int nComp = 1);
	virtual void setState_ph(double &p, double &h, int &phase, TwoPhaseMediumProperties *const properties, const int nComp = 1);
	virtual void setState_ps(double &p, double &s, int &phase, TwoPhaseMediumProperties *const properties, const int nComp = 1);
	virtual void setState_pT(double &p, double &T, TwoPhaseMediumProperties *const properties, const int nComp = 1);

	virtual void setBubbleState(int phase, TwoPhaseMediumProperties *const properties,
		                        TwoPhaseMediumProperties *const bubbleProperties);
   	virtual void setDewState(int phase, TwoPhaseMediumProperties *const properties,
	                         TwoPhaseMediumProperties *const bubbleProperties);

	virtual bool computeDerivatives(TwoPhaseMediumProperties *const properties);

	virtual double isentropicEnthalpy(double &p, TwoPhaseMediumProperties *const properties);
    double molarMass() const;
	double criticalTemperature() const;
	double criticalPressure() const;
	double criticalDensity() const;
	double criticalMolarVolume() const;
	double criticalEnthalpy() const;
	double criticalEntropy() const;

	//! Medium name
	string mediumName;
	//! Library name
	string libraryName;
	//! Substance name
	string substanceName;
	//! Number of components
	int nComp;
	//! Concentrations
	double *Conc;

protected:
	//! Fluid constants
	FluidConstants _fluidConstants; 
};

#endif // BASESOLVER_H_
