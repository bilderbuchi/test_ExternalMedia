#ifndef TESTSOLVER_H_
#define TESTSOLVER_H_

#include "basesolver.h"

//! Test solver class
/*!
  This class defines a dummy solver object, computing properties of
  a fluid roughly resembling warm water at low pressure, without
  the need of any further external code. The class is useful for debugging
  purposes, to test whether the C compiler and the Modelica tools are set
  up correctly before tackling problems with the actual - usually way more
  complex - external code. It is *not* meant to be used as an actual 
  fluid model for any real application.

  To keep complexity down to the absolute medium, the current version of the 
  solver can only compute the fluid properties in the liquid phase region:
      1e5 Pa < p < 2e5 Pa
      300 K  < T < 350 K ;
  results returned with inputs outside that range (possibly corresponding 
  to two-phase or vapour points) are not reliable.
  Saturation properies are computed in the range 
      1e5 Pa < psat < 2e5 Pa ;
  results obtained outside that range might be unrealistic.

  To instantiate this solver, it is necessary to set the library name package
  constant in Modelica as follows:

  libraryName = "TestMedium";

  At the moment the test solver class can only handle pure substances.

  Francesco Casella, Christoph Richter, Oct 2006
  Copyright Politecnico di Milano and TU Braunschweig
*/
class TestSolver : public BaseSolver{
public:
	TestSolver(const string &mediumName, const string &libraryName, const string &substanceName);
	~TestSolver();
	virtual void setFluidConstants();

	virtual void setSat_p(double &p, TwoPhaseMediumProperties *const properties, const int nComp = 1);
	virtual void setSat_T(double &T, TwoPhaseMediumProperties *const properties, const int nComp = 1);
    virtual void setSat_p_state(TwoPhaseMediumProperties *const properties);

	virtual void setState_dT(double &d, double &T, int &phase, TwoPhaseMediumProperties *const properties, const int nComp = 1);
	virtual void setState_ph(double &p, double &h, int &phase, TwoPhaseMediumProperties *const properties, const int nComp = 1);
	virtual void setState_ps(double &p, double &s, int &phase, TwoPhaseMediumProperties *const properties, const int nComp = 1);
	virtual void setState_pT(double &p, double &T, TwoPhaseMediumProperties *const properties, const int nComp = 1);
};

#endif // TESTSOLVER_H_
