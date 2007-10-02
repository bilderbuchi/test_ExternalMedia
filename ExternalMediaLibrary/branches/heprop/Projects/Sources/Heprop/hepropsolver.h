#ifndef HEPROPSOLVER_H_
#define HEPROPSOLVER_H_

#include "../basesolver.h"

/*
  Header file for the Heprop solver

  Francesco Casella, Barbara Fiori 2007
*/


class HepropSolver : public BaseSolver{
public:
	HepropSolver(const string &mediumName, const string &libraryName, const string &substanceName);
	~HepropSolver();
	virtual void setFluidConstants();

    virtual void setSat_p(double &p, TwoPhaseMediumProperties *const properties);
	virtual void setSat_T(double &T, TwoPhaseMediumProperties *const properties);
    virtual void setSat_p_state(TwoPhaseMediumProperties *const properties);

	virtual void setState_dT(double &d, double &T, int &phase, TwoPhaseMediumProperties *const properties);
	virtual void setState_ph(double &p, double &h, int &phase, TwoPhaseMediumProperties *const properties);
	virtual void setState_ps(double &p, double &s, int &phase, TwoPhaseMediumProperties *const properties);
	virtual void setState_pT(double &p, double &T, TwoPhaseMediumProperties *const properties);
};

#endif // HEPROPSOLVER_H_
