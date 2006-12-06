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
	return _properties->beta;
}

double BaseTwoPhaseMedium::cp() const{
	return _properties->cp;
}

double BaseTwoPhaseMedium::cv() const{
	return _properties->cv;
}

double BaseTwoPhaseMedium::d() const{
	return _properties->d;
}

double BaseTwoPhaseMedium::dd_dp_h() const{
	return _properties->dd_dp_h;
}

double BaseTwoPhaseMedium::dd_dh_p() const{
	return _properties->dd_dh_p;
}

double BaseTwoPhaseMedium::h() const{
	return _properties->h;
}

double BaseTwoPhaseMedium::kappa() const{
	return _properties->kappa;
}

double BaseTwoPhaseMedium::p() const{
	return _properties->p;
}

double BaseTwoPhaseMedium::s() const{
	return _properties->s;
}

double BaseTwoPhaseMedium::T() const{
	return _properties->T;
}

double BaseTwoPhaseMedium::ps() const{
	return _properties->ps;
}

double BaseTwoPhaseMedium::Ts() const{
	return _properties->Ts;
}

double BaseTwoPhaseMedium::dl() const{
	return _properties->dl;
}

double BaseTwoPhaseMedium::dv() const{
	return _properties->dv;
}

double BaseTwoPhaseMedium::hl() const{
	return _properties->hl;
}

double BaseTwoPhaseMedium::hv() const{
	return _properties->hv;
}

double BaseTwoPhaseMedium::sl() const{
	return _properties->sl;
}

double BaseTwoPhaseMedium::sv() const{
	return _properties->sv;
}

double BaseTwoPhaseMedium::d_Ts_dp() const{
	return _properties->d_Ts_dp;
}

double BaseTwoPhaseMedium::d_dl_dp() const{
	return _properties->d_dl_dp;
}

double BaseTwoPhaseMedium::d_dv_dp() const{
	return _properties->d_dv_dp;
}

double BaseTwoPhaseMedium::d_hl_dp() const{
	return _properties->d_hl_dp;
}

double BaseTwoPhaseMedium::d_hv_dp() const{
	return _properties->d_hv_dp;
}

double BaseTwoPhaseMedium::eta() const{
	return _properties->eta;
}

double BaseTwoPhaseMedium::lambda() const{
	return _properties->lambda;
}

double BaseTwoPhaseMedium::Pr() const{
	return _properties->Pr;
}

double BaseTwoPhaseMedium::sigma() const{
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
