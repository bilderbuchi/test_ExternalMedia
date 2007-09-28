#ifndef HELIUMLIBSOLVER_H_
#define HELIUMLIBSOLVER_H_

#include "../Sources/basesolver.h"

/*
  Header file for the Heliumlib solver

  Francesco Casella, Barbara Fiori 2007
*/


class HeliumlibSolver : public BaseSolver{
public:
	HeliumlibSolver(const string &mediumName, const string &libraryName, const string &substanceName);
	~HeliumlibSolver();
	virtual void setFluidConstants();

    virtual void setSat_p(double &p, TwoPhaseMediumProperties *const properties);
	virtual void setSat_T(double &T, TwoPhaseMediumProperties *const properties);
    virtual void setSat_p_state(TwoPhaseMediumProperties *const properties);

	virtual void setState_dT(double &d, double &T, int &phase, TwoPhaseMediumProperties *const properties);
	virtual void setState_ph(double &p, double &h, int &phase, TwoPhaseMediumProperties *const properties);
	virtual void setState_ps(double &p, double &s, int &phase, TwoPhaseMediumProperties *const properties);
	virtual void setState_pT(double &p, double &T, TwoPhaseMediumProperties *const properties);
};

#endif // HELIUMLIBSOLVER_H_
