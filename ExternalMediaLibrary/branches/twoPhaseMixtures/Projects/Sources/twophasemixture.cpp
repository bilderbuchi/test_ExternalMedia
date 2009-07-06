#include "twophasemixture.h"

#include <math.h>

//! Constructor
/*!
  This constructor is passing the argument to the constructor of the base class
  and is creating a the property container.
  @param mediumName Medium name
  @param libraryName Library name
  @param substanceName Substance name
  @param solver Solver
  @param uniqueID Unique ID number
*/
TwoPhaseMixture::TwoPhaseMixture(const string &mediumName, const string &libraryName, 
							   const string &substanceName, BaseSolver *const solver,
							   const int &uniqueID)
	: BaseTwoPhaseMedium(mediumName, libraryName, substanceName, solver, uniqueID){
	// Create new medium property struct
	_properties = new TwoPhaseMediumProperties();
}

//! Destructor
/*!
  The destructor is deleting the property container.
*/
TwoPhaseMixture::~TwoPhaseMixture(){
	// Delete medium property struct
	delete _properties; _properties = 0;
}

//! Return equilibrium molar fraction of liquid phase (error handling included)
double *TwoPhaseMixture::x() const{
	if (ISNAN(_properties->x[0]))
		errorMessage("Internal error in TwoPhaseMixture object:\nthe _properties->x field was not set");
	return _properties->x;
}

//! Return equilibrium molar fraction of vapor phase (error handling to be done!)
double *TwoPhaseMixture::y() const{
	if (ISNAN(_properties->y[0]))
		errorMessage("Internal error in TwoPhaseMixture object:\nthe _properties->y field was not set");
	return _properties->y;
}