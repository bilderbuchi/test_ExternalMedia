#include "heliumlibsolver.h"
#include "twophasemediumproperties.h"
#include "mediummap.h"
#include <math.h>

HeliumlibSolver::HeliumlibSolver(const string &mediumName, const string &libraryName, const string &substanceName)
	: BaseSolver(mediumName, libraryName, substanceName){
  setFluidConstants();
}

HeliumlibSolver::~HeliumlibSolver(){
}

void HeliumlibSolver::setFluidConstants(){
  _fluidConstants.pc = 2.26*101325; 
  _fluidConstants.Tc = 5.25;   
  _fluidConstants.MM = 4.003e-3;  
  _fluidConstants.dc = 69.3;
}

void HeliumlibSolver::setSat_p(double &p, TwoPhaseMediumProperties *const properties){
    errorMessage("setSat_p not implemented\n");
}

void HeliumlibSolver::setSat_T(double &T, TwoPhaseMediumProperties *const properties){
    errorMessage("setSat_T not implemented\n");
}

void HeliumlibSolver::setSat_p_state(TwoPhaseMediumProperties *const properties){
  setSat_p(properties->p, properties);
};

void HeliumlibSolver::setState_ph(double &p, double &h, int &phase, TwoPhaseMediumProperties *const properties){
    errorMessage("setState_ph not implemented\n");
}

void HeliumlibSolver::setState_pT(double &p, double &T, TwoPhaseMediumProperties *const properties){
    double Phi, c, d;
	Phi = GRUNHE(p,T);
	c = SNDHE(p,T);
	d = DHE(p,T);
	properties->p = p;
	properties->T = T;
	properties->h = HHE(p,T);
	properties->d = d;
	properties->phase = 1; // with pT input, always one-phase conditions!
	properties->cp = CPHE(p,T);
	properties->cv = CVHE(p,T);
	properties->dd_dh_p = -d*Phi/(c*c);
	properties->dd_dp_h = (1+Phi)/(c*c);
}

void HeliumlibSolver::setState_dT(double &d, double &T, int &phase, TwoPhaseMediumProperties *const properties){
    errorMessage("setState_dT not implemented\n");
}

void HeliumlibSolver::setState_ps(double &p, double &s, int &phase, TwoPhaseMediumProperties *const properties){
    errorMessage("setState_ps not implemented\n");
}

