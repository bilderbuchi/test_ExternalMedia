#ifndef BASETWOPHASEMEDIUM_H_
#define BASETWOPHASEMEDIUM_H_

#include "include.h"
#include "basesolver.h"
#include "twophasemediumproperties.h"

//! Base two-phase medium class
/*!
  This class defines all the variables and member
  functions which are needed to use external Modelica medium models
  extending from PartialExternalTwoPhaseMedium.

  The functions defined here are not fluid-specific, thus need not
  be adapted to your own specific fluid property computation code.

  Francesco Casella, Christoph Richter, Sep 2006
  Copyright Politecnico di Milano and TU Braunschweig
  modified by Andreas Joos (Hamburg University of Technology), Jun 2009
*/
class BaseTwoPhaseMedium{
public:
	BaseTwoPhaseMedium(const string &mediumName, const string &libraryName,
					   const string &substanceName, BaseSolver *const solver,
					   const int &uniqueID);
	virtual ~BaseTwoPhaseMedium();

	void reinitMedium(const string &mediumName, const string &libraryName,
					  const string &substanceName, BaseSolver *const solver,
					  const int &uniqueID);

	int uniqueID() const;
	string mediumName() const;
	string libraryName() const;
	string substanceName() const;

	TwoPhaseMediumProperties *properties() const;

	BaseSolver *solver() const;
	virtual void setSolver(BaseSolver *const solver);

    int phase() const;
	double beta() const;
	double cp() const;
	double cv() const;
	double d() const;
	double dd_dp_h() const;
	double dd_dh_p() const;
	double h() const;
	double u() const;
	double kappa() const;
	double p() const;
	double s() const;
	double T() const;
	double h_iso(double &p) const;
	double dT_dp_h() const;
	double dT_dh_p() const;

	double du_dp() const;
	double du_dh() const;

	double ps() const;
	double Ts() const;

	double dl() const;
	double dv() const;
	double hl() const;
	double hv() const;
	double sl() const;
	double sv() const;

	virtual double *x() const;
	virtual double *y() const;

	double d_Ts_dp() const;
	double d_dl_dp() const;
	double d_dv_dp() const;
	double d_hl_dp() const;
	double d_hv_dp() const;

	double eta() const;
	double lambda() const;
	double Pr() const;
	double sigma() const;

	double a() const;

	virtual void setSat_p(double &p);
	virtual void setSat_T(double &T);
	virtual void setSat_p_state();

	virtual void setState_dT(double &d, double &T, int &phase, const int nComp = 1);
	virtual void setState_ph(double &p, double &h, int &phase, const int nComp = 1);
	virtual void setState_ps(double &p, double &s, int &phase, const int nComp = 1);
	virtual void setState_pT(double &p, double &T, const int nComp = 1);

	virtual int  getBubbleUniqueID(int phase);
	virtual int  getDewUniqueID(int phase);

    virtual void setDewState(int phase);
    virtual void setBubbleState(int phase);

protected:
	//! Medium property record
	TwoPhaseMediumProperties *_properties;
	//! Solver
	BaseSolver *_solver;

	//! Unique ID
	int _uniqueID;

	//! Medium name
	string _mediumName;
	//! Library name
	string _libraryName;
	//! Substance name
	string _substanceName;

	//! Unique ID of corresponding 1-phase dew state medium object, set by setDewState
	int _dewUniqueIDOnePhase;
	//! Unique ID of corresponding 2-phase dew state medium object, set by setDewState
	int _dewUniqueIDTwoPhase;
	//! Unique ID of corresponding 1-phase bubble state medium object, set by setBubbleState
	int _bubbleUniqueIDOnePhase;
	//! Unique ID of corresponding 2-phase bubble state medium object, set by setBubbleState
	int _bubbleUniqueIDTwoPhase;
};

#endif // BASETWOPHASEMEDIUM_H_
