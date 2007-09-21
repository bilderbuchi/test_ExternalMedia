#include "heliumlibsolver.h"
#include "heliumlib.h"
#include "../Sources/twophasemediumproperties.h"
#include "../Sources/mediummap.h"
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
	properties->ps = -1; // Dummy value
	properties->Ts = -1; // Dummy value
    // errorMessage("setSat_p not implemented\n");
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
	float pp, TT;
	pp = (float)p;
	TT = (float)T;
	Phi = GRUNHE(&pp,&TT);
	c = SNDHE(&pp,&TT);
	d = DHE(&pp,&TT);
	properties->p = p;
	properties->T = T;
	properties->h = HHE(&pp,&TT);
	properties->s = SHE(&pp,&TT);
	properties->d = d;
	properties->phase = 1; // with pT input, always one-phase conditions!
	properties->cp = CPHE(&pp,&TT);
	properties->cv = CVHE(&pp,&TT);
	properties->dd_dh_p = -d*Phi/(c*c);
	properties->dd_dp_h = (1+Phi)/(c*c);
}

void HeliumlibSolver::setState_dT(double &d, double &T, int &phase, TwoPhaseMediumProperties *const properties){
    errorMessage("setState_dT not implemented\n");
}

void HeliumlibSolver::setState_ps(double &p, double &s, int &phase, TwoPhaseMediumProperties *const properties){
    errorMessage("setState_ps not implemented\n");
}

