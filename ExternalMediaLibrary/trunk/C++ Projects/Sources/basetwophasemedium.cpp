/* *****************************************************************
 * Implementation of the base class BaseTwoPhaseMedium
 *
 * The BaseTwoPhaseMedium class defines all the variables and member
 * functions which are needed to use external Modelica medium models
 * extending from PartialExternalTwoPhaseMedium.
 * 
 * The functions defined here are not fluid-specific, thus need not
 * be adapted to your own specific fluid property computation code.
 *
 * Francesco Casella, Christoph Richter Sep 2006
 ********************************************************************/

#include "basetwophasemedium.h"

#include <math.h>
#include "mediummap.h"

BaseTwoPhaseMedium::BaseTwoPhaseMedium(const string &mediumName, const string &libraryName, 
									   const string &substanceName, BaseSolver *const solver, 
									   const int &uniqueID)
	: _solver(solver){
}

BaseTwoPhaseMedium::~BaseTwoPhaseMedium(){
}

int BaseTwoPhaseMedium::uniqueID() const{
	return _properties->uniqueID;
}

string BaseTwoPhaseMedium::mediumName() const{
	return _properties->mediumName;
}

string BaseTwoPhaseMedium::libraryName() const{
	return _properties->libraryName;
}

string BaseTwoPhaseMedium::substanceName() const{
	return _properties->substanceName;
}

TwoPhaseMediumProperties *BaseTwoPhaseMedium::properties() const{
	return _properties;
}

BaseSolver *BaseTwoPhaseMedium::solver() const{
	return _solver;
}

void BaseTwoPhaseMedium::setSolver(BaseSolver *const solver){
	_solver = solver;
}

double BaseTwoPhaseMedium::beta() const{
	if (ISNAN(_properties->beta))
		errorMessage("Internal error: setState did not set the beta field");
	return _properties->beta;
}

double BaseTwoPhaseMedium::cp() const{
	if (ISNAN(_properties->cp))
		errorMessage("Internal error: setState did not set the cp field");
	return _properties->cp;
}

double BaseTwoPhaseMedium::cv() const{
	if (ISNAN(_properties->cv))
		errorMessage("Internal error: setState did not set the cv field");
	return _properties->cv;
}

double BaseTwoPhaseMedium::d() const{
	if (ISNAN(_properties->d))
		errorMessage("Internal error: setState did not set the d field");
	return _properties->d;
}

double BaseTwoPhaseMedium::dd_dp_h() const{
	if (ISNAN(_properties->dd_dp_h))
		errorMessage("Internal error: setState did not set the dd_dp_h field");
	return _properties->dd_dp_h;
}

double BaseTwoPhaseMedium::dd_dh_p() const{
	if (ISNAN(_properties->dd_dh_p))
		errorMessage("Internal error: setState did not set the dd_dh_p field");
	return _properties->dd_dh_p;
}

double BaseTwoPhaseMedium::h() const{
	if (ISNAN(_properties->h))
		errorMessage("Internal error: setState did not set the h field");
	return _properties->h;
}

double BaseTwoPhaseMedium::kappa() const{
	if (ISNAN(_properties->kappa))
		errorMessage("Internal error: setState did not set the kappa field");
	return _properties->kappa;
}

double BaseTwoPhaseMedium::p() const{
	if (ISNAN(_properties->p))
		errorMessage("Internal error: setState did not set the p field");
	return _properties->p;
}

double BaseTwoPhaseMedium::s() const{
	if (ISNAN(_properties->s))
		errorMessage("Internal error: setState did not set the s field");
	return _properties->s;
}

double BaseTwoPhaseMedium::T() const{
	if (ISNAN(_properties->T))
		errorMessage("Internal error: setState did not set the T field");
	return _properties->T;
}

double BaseTwoPhaseMedium::dT_dp_h() const{
	if (ISNAN(_properties->dT_dp_h))
		errorMessage("Internal error: setState did not set the dT_dp_h field");
	return _properties->dT_dp_h;
}

double BaseTwoPhaseMedium::dT_dh_p() const{
	if (ISNAN(_properties->dT_dh_p))
		errorMessage("Internal error: setState did not set the dT_dh_p field");
	return _properties->dT_dh_p;
}

double BaseTwoPhaseMedium::ps() const{
	if (ISNAN(_properties->ps))
		errorMessage("Internal error: setState did not set the ps field");
	return _properties->ps;
}

double BaseTwoPhaseMedium::Ts() const{
	if (ISNAN(_properties->Ts))
		errorMessage("Internal error: setState did not set the Ts field");
	return _properties->Ts;
}

double BaseTwoPhaseMedium::dl() const{
	if (ISNAN(_properties->dl))
		errorMessage("Internal error: setState did not set the dl field");
	return _properties->dl;
}

double BaseTwoPhaseMedium::dv() const{
	if (ISNAN(_properties->dv))
		errorMessage("Internal error: setState did not set the dv field");
	return _properties->dv;
}

double BaseTwoPhaseMedium::hl() const{
	if (ISNAN(_properties->hl))
		errorMessage("Internal error: setState did not set the hl field");
	return _properties->hl;
}

double BaseTwoPhaseMedium::hv() const{
	if (ISNAN(_properties->hv))
		errorMessage("Internal error: setState did not set the hv field");
	return _properties->hv;
}

double BaseTwoPhaseMedium::sl() const{
	if (ISNAN(_properties->sl))
		errorMessage("Internal error: setState did not set the sl field");
	return _properties->sl;
}

double BaseTwoPhaseMedium::sv() const{
	if (ISNAN(_properties->sv))
		errorMessage("Internal error: setState did not set the sv field");
	return _properties->sv;
}

double BaseTwoPhaseMedium::d_Ts_dp() const{
	if (ISNAN(_properties->d_Ts_dp))
		errorMessage("Internal error: setState did not set the d_Ts_dp field");
	return _properties->d_Ts_dp;
}

double BaseTwoPhaseMedium::d_dl_dp() const{
	if (ISNAN(_properties->d_dl_dp))
		errorMessage("Internal error: setState did not set the d_dl_dp field");
	return _properties->d_dl_dp;
}

double BaseTwoPhaseMedium::d_dv_dp() const{
	if (ISNAN(_properties->d_dv_dp))
		errorMessage("Internal error: setState did not set the d_dv_dp field");
	return _properties->d_dv_dp;
}

double BaseTwoPhaseMedium::d_hl_dp() const{
	if (ISNAN(_properties->d_hl_dp))
		errorMessage("Internal error: setState did not set the d_hl_dp field");
	return _properties->d_hl_dp;
}

double BaseTwoPhaseMedium::d_hv_dp() const{
	if (ISNAN(_properties->d_hv_dp))
		errorMessage("Internal error: setState did not set the d_hv_dp field");
	return _properties->d_hv_dp;
}

double BaseTwoPhaseMedium::eta() const{
	if (ISNAN(_properties->eta))
		errorMessage("Internal error: setState did not set the eta field");
	return _properties->eta;
}

double BaseTwoPhaseMedium::lambda() const{
	if (ISNAN(_properties->lambda))
		errorMessage("Internal error: setState did not set the lambda field");
	return _properties->lambda;
}

double BaseTwoPhaseMedium::Pr() const{
	if (ISNAN(_properties->Pr))
		errorMessage("Internal error: setState did not set the Pr field");
	return _properties->Pr;
}

double BaseTwoPhaseMedium::sigma() const{
	if (ISNAN(_properties->sigma))
		errorMessage("Internal error: setState did not set the sigma field");
	return _properties->sigma;
}

void BaseTwoPhaseMedium::setSat_p(double &p){
	_solver->setSat_p(p, _properties);
}

void BaseTwoPhaseMedium::setSat_T(double &T){
	_solver->setSat_T(T, _properties);
}

void BaseTwoPhaseMedium::setSat_p_state(){
	_solver->setSat_p_state(_properties);
}

void BaseTwoPhaseMedium::setState_dT(double &d, double &T, int &phase){
	_solver->setState_dT(d, T, phase, _properties);
}

void BaseTwoPhaseMedium::setState_ph(double &p, double &h, int &phase){
	_solver->setState_ph(p, h, phase, _properties);
}

void BaseTwoPhaseMedium::setState_ps(double &p, double &s, int &phase){
	_solver->setState_ps(p, s, phase, _properties);
}

void BaseTwoPhaseMedium::setState_pT(double &p, double &T){
	_solver->setState_pT(p, T, _properties);
}

int BaseTwoPhaseMedium::getDewUniqueID(int phase){
	if (phase == 1){
        // Dew state on the one-phase side
		if (_properties->dewUniqueIDOnePhase == 0){
			if (_properties->uniqueID > 0)
				// allocate a permanent medium object for the dew state
				_properties->dewUniqueIDOnePhase = 
					MediumMap::addMedium(_properties->mediumName, 
						                 _properties->libraryName,
										 _properties->substanceName);
			else 
				// allocate a transient medium object for the dew state
				_properties->dewUniqueIDOnePhase =
					MediumMap::addTransientMedium(_properties->mediumName, 
												  _properties->libraryName,
												  _properties->substanceName);
		}
		// return the dew state unique ID
		return _properties->dewUniqueIDOnePhase;
	}
	else {
		// Dew state on the two-phase side
		if (_properties->dewUniqueIDTwoPhase == 0){
			if (_properties->uniqueID > 0)
				// allocate a permanent medium object for the dew state
				_properties->dewUniqueIDTwoPhase = 
					MediumMap::addMedium(_properties->mediumName, 
						                 _properties->libraryName,
										 _properties->substanceName);
			else 
				// allocate a transient medium object for the dew state
				_properties->dewUniqueIDTwoPhase =
					MediumMap::addTransientMedium(_properties->mediumName, 
												  _properties->libraryName,
												  _properties->substanceName);
		}
		// return the dew state unique ID
		return _properties->dewUniqueIDTwoPhase;
	}
}

int BaseTwoPhaseMedium::getBubbleUniqueID(int phase){
	if (phase == 1){
        // Bubble state on the one-phase side
		if (_properties->bubbleUniqueIDOnePhase == 0){
			if (_properties->uniqueID > 0)
				// allocate a permanent medium object for the bubble state
				_properties->bubbleUniqueIDOnePhase = 
					MediumMap::addMedium(_properties->mediumName, 
						                 _properties->libraryName,
										 _properties->substanceName);
			else 
				// allocate a transient medium object for the bubble state
				_properties->bubbleUniqueIDOnePhase =
					MediumMap::addTransientMedium(_properties->mediumName, 
												  _properties->libraryName,
												  _properties->substanceName);
		}
		// return the bubble state unique ID
		return _properties->bubbleUniqueIDOnePhase;
	}
	else {
		// Bubble state on the two-phase side
		if (_properties->bubbleUniqueIDTwoPhase == 0){
			if (_properties->uniqueID > 0)
				// allocate a permanent medium object for the bubble state
				_properties->bubbleUniqueIDTwoPhase = 
					MediumMap::addMedium(_properties->mediumName, 
						                 _properties->libraryName,
										 _properties->substanceName);
			else 
				// allocate a transient medium object for the bubble state
				_properties->bubbleUniqueIDTwoPhase =
					MediumMap::addTransientMedium(_properties->mediumName, 
												  _properties->libraryName,
												  _properties->substanceName);
		}
		// return the bubble state unique ID
		return _properties->bubbleUniqueIDTwoPhase;
	}
}

void BaseTwoPhaseMedium::setBubbleState(int phase){
	// Get a pointer to the bubble state medium object
	BaseTwoPhaseMedium *bubbleMedium;
	if (phase == 1)
		bubbleMedium = MediumMap::medium(_properties->bubbleUniqueIDOnePhase);
	else
		bubbleMedium = MediumMap::medium(_properties->bubbleUniqueIDTwoPhase);

	// Call the solver to set the bubble state medium properties
	_solver->setBubbleState(phase, _properties, bubbleMedium->_properties);
}

void BaseTwoPhaseMedium::setDewState(int phase){
	// Get a pointer to the dew state medium object
	BaseTwoPhaseMedium *dewMedium;
	if (phase == 1)
		dewMedium = MediumMap::medium(_properties->dewUniqueIDOnePhase);
	else
		dewMedium = MediumMap::medium(_properties->dewUniqueIDTwoPhase);

	// Call the solver to set the dew state medium properties
	_solver->setDewState(phase, _properties, dewMedium->_properties);
}
