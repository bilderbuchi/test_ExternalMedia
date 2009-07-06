/*!
  \file externalmedialib.cpp
  \brief Interface layer

  C/C++ layer for external medium models extending from 
  PartialExternalTwoPhaseMedium.

  Francesco Casella, Christoph Richter, Sep 2006
  modified by Andreas Joos, Jun 2009
*/

#include "externalmedialib.h"
#include "mediummap.h"
#include "twophasemedium.h"
#include "twophasemixture.h"

// Header of private function
void TwoPhaseMedium_setStateDefault_(BaseTwoPhaseMedium *medium, int choice, double d, double h, double p, double s, double T, int phase, int nComp = 1);


//! Create medium
/*!
  This function creates a new medium with the specified medium name, library name,
  and substance name. The old unique ID is required to check whether a medium
  has already been created.
  @param mediumName Medium name
  @param libraryName Library name
  @param substanceName Substance name
  @param oldUniqueID Old unique ID number
*/
int TwoPhaseMedium_createMedium_(const char *mediumName, const char *libraryName, const char *substanceName, int oldUniqueID){
	// Allocate a new object and return a unique ID if oldUniqueID == 0
	if (oldUniqueID == 0){
		return MediumMap::addMedium(mediumName, libraryName, substanceName);
	} else {
	// Do nothing if oldUniqueID > 0 (medium object already allocated)
		return oldUniqueID;
	}
}

//! Get molar mass
/*!
  This function returns the molar mass of the specified medium.
  @param mediumName Medium name
  @param libraryName Library name
  @param substanceName Substance name
*/
double TwoPhaseMedium_getMolarMass_(const char *mediumName, const char *libraryName, const char *substanceName){
	// Return molar mass
	return SolverMap::getSolver(mediumName, libraryName, substanceName)->molarMass();
}

//! Get critical temperature
/*!
  This function returns the critical temperature of the specified medium.
  @param mediumName Medium name
  @param libraryName Library name
  @param substanceName Substance name
*/
double TwoPhaseMedium_getCriticalTemperature_(const char *mediumName, const char *libraryName, const char *substanceName){
	// Return critical temperature
	return SolverMap::getSolver(mediumName, libraryName, substanceName)->criticalTemperature();
}

//! Get critical pressure
/*!
  This function returns the critical pressure of the specified medium.
  @param mediumName Medium name
  @param libraryName Library name
  @param substanceName Substance name
*/
double TwoPhaseMedium_getCriticalPressure_(const char *mediumName, const char *libraryName, const char *substanceName){
	// Return critical pressure
	return SolverMap::getSolver(mediumName, libraryName, substanceName)->criticalPressure();
}

//! Get critical molar volume
/*!
  This function returns the critical molar volume of the specified medium.
  @param mediumName Medium name
  @param libraryName Library name
  @param substanceName Substance name
*/
double TwoPhaseMedium_getCriticalMolarVolume_(const char *mediumName, const char *libraryName, const char *substanceName){
	// Return critical molar volume
	return SolverMap::getSolver(mediumName, libraryName, substanceName)->criticalMolarVolume();
}

//! Compute properties from d, T, and phase
/*!
  This function computes the properties for the specified inputs. If the function
  is called with uniqueID == 0 a new transient unique ID is assigned and the function
  is called again with this new transient unique ID number.
  @param d Density
  @param T Temperature
  @param phase Phase (2 for two-phase, 1 for one-phase, 0 if not known)
  @param uniqueID Unique ID number
  @param state_uniqueID Pointer to return unique ID number for state record
  @param state_phase Pointer to return phase for state record
  @param state_d Pointer to return density for state record
  @param state_h Pointer to return specific enthalpy for state record
  @param state_p Pointer to return pressure for state record
  @param state_s Pointer to return specific entropy for state record
  @param state_T Pointer to return temperature for state record
  @param mediumName Medium name
  @param libraryName Library name
  @param substanceName Substance name
*/
void TwoPhaseMedium_setState_dT_(double d, double T, int phase, int uniqueID, int *state_uniqueID, int *state_phase, double *state_d, double *state_h, double *state_p, double *state_s, double *state_T,
								 const char *mediumName, const char *libraryName, const char *substanceName){
	if (uniqueID == 0){
	  // setState_dT was called with uniqueID == 0
	  // create a new transient medium object and get a transientUniqueID
	  int transientUniqueID = MediumMap::addTransientMedium(mediumName, libraryName, substanceName);
      // run setState_dT_ with the transientUniqueID
	  TwoPhaseMedium_setState_dT_(d, T, phase, transientUniqueID, state_uniqueID, state_phase, state_d, state_h, state_p, state_s, state_T, mediumName, libraryName, substanceName);
	} else {
      // setState_dT was called with the uniqueID of an existing medium object
 	  // Call the medium object's setState_dT function
	  BaseTwoPhaseMedium *medium = MediumMap::medium(uniqueID);
	  medium->setState_dT(d, T, phase);
	  // Return values
	  if (state_uniqueID != NULL)
  		  *state_uniqueID = uniqueID;
	  if (state_phase != NULL)
	 	  *state_phase = MediumMap::medium(uniqueID)->phase();
	  if (state_d != NULL)
		  *state_d = medium->d();
	  if (state_h != NULL)
		  *state_h = medium->h();
	  if (state_p != NULL)
		  *state_p = medium->p();
	  if (state_s != NULL)
		  *state_s = medium->s();
	  if (state_T != NULL)
		  *state_T = medium->T();
    }
}

//! Compute properties from p, h, and phase
/*!
  This function computes the properties for the specified inputs. If the function
  is called with uniqueID == 0 a new transient unique ID is assigned and the function
  is called again with this new transient unique ID number.
  @param p Pressure
  @param h Specific enthalpy
  @param phase Phase (2 for two-phase, 1 for one-phase, 0 if not known)
  @param uniqueID Unique ID number
  @param state_uniqueID Pointer to return unique ID number for state record
  @param state_phase Pointer to return phase for state record
  @param state_d Pointer to return density for state record
  @param state_h Pointer to return specific enthalpy for state record
  @param state_p Pointer to return pressure for state record
  @param state_s Pointer to return specific entropy for state record
  @param state_T Pointer to return temperature for state record
  @param mediumName Medium name
  @param libraryName Library name
  @param substanceName Substance name
*/
void TwoPhaseMedium_setState_ph_(double p, double h, int phase, int uniqueID, int *state_uniqueID, int *state_phase, double *state_d, double *state_h, double *state_p, double *state_s, double *state_T,
								 const char *mediumName, const char *libraryName, const char *substanceName){
	if (uniqueID == 0){
	  // setState_ph was called with uniqueID == 0
	  // create a new transient medium object and get a transientUniqueID
	  int transientUniqueID = MediumMap::addTransientMedium(mediumName, libraryName, substanceName);
      // run setState_ph_ with the transientUniqueID
	  TwoPhaseMedium_setState_ph_(p, h, phase, transientUniqueID, state_uniqueID, state_phase, state_d, state_h, state_p, state_s, state_T, mediumName, libraryName, substanceName);
	} else {
      // setState_ph was called with the uniqueID of an existing medium object
 	  // Call the medium object's setState_ph function
	  BaseTwoPhaseMedium *medium = MediumMap::medium(uniqueID);
	  medium->setState_ph(p, h, phase);
	  // Return values
	  if (state_uniqueID != NULL)
  		  *state_uniqueID = uniqueID;
	  if (state_phase != NULL)
	 	  *state_phase = MediumMap::medium(uniqueID)->phase();
	  if (state_d != NULL)
		  *state_d = medium->d();
	  if (state_h != NULL)
		  *state_h = medium->h();
	  if (state_p != NULL)
		  *state_p = medium->p();
	  if (state_s != NULL)
		  *state_s = medium->s();
	  if (state_T != NULL)
		  *state_T = medium->T();
    }
}

//! Compute properties from p, s, and phase
/*!
  This function computes the properties for the specified inputs. If the function
  is called with uniqueID == 0 a new transient unique ID is assigned and the function
  is called again with this new transient unique ID number.
  @param p Pressure
  @param s Specific entropy
  @param phase Phase (2 for two-phase, 1 for one-phase, 0 if not known)
  @param uniqueID Unique ID number
  @param state_uniqueID Pointer to return unique ID number for state record
  @param state_phase Pointer to return phase for state record
  @param state_d Pointer to return density for state record
  @param state_h Pointer to return specific enthalpy for state record
  @param state_p Pointer to return pressure for state record
  @param state_s Pointer to return specific entropy for state record
  @param state_T Pointer to return temperature for state record
  @param mediumName Medium name
  @param libraryName Library name
  @param substanceName Substance name
*/
void TwoPhaseMedium_setState_ps_(double p, double s, int phase, int uniqueID, int *state_uniqueID, int *state_phase, double *state_d, double *state_h, double *state_p, double *state_s, double *state_T,
								 const char *mediumName, const char *libraryName, const char *substanceName){
	if (uniqueID == 0){
	  // setState_ps was called with uniqueID == 0
	  // create a new transient medium object and get a transientUniqueID
	  int transientUniqueID = MediumMap::addTransientMedium(mediumName, libraryName, substanceName);
      // run setState_ps_ with the transientUniqueID
	  TwoPhaseMedium_setState_ps_(p, s, phase, transientUniqueID, state_uniqueID, state_phase, state_d, state_h, state_p, state_s, state_T, mediumName, libraryName, substanceName);
	} else {
      // setState_ps was called with the uniqueID of an existing medium object
 	  // Call the medium object's setState_ps function
	  BaseTwoPhaseMedium *medium = MediumMap::medium(uniqueID);
	  medium->setState_ps(p, s, phase);
	  // Return values
	  if (state_uniqueID != NULL)
  		  *state_uniqueID = uniqueID;
	  if (state_phase != NULL)
	 	  *state_phase = MediumMap::medium(uniqueID)->phase();
	  if (state_d != NULL)
		  *state_d = medium->d();
	  if (state_h != NULL)
		  *state_h = medium->h();
	  if (state_p != NULL)
		  *state_p = medium->p();
	  if (state_s != NULL)
		  *state_s = medium->s();
	  if (state_T != NULL)
		  *state_T = medium->T();
    }
}

//! Compute properties from p, T, and phase
/*!
  This function computes the properties for the specified inputs. If the function
  is called with uniqueID == 0 a new transient unique ID is assigned and the function
  is called again with this new transient unique ID number.

  Attention: The phase input is ignored for this function!
  @param p Pressure
  @param T Temperature
  @param phase Phase (2 for two-phase, 1 for one-phase, 0 if not known)
  @param uniqueID Unique ID number
  @param state_uniqueID Pointer to return unique ID number for state record
  @param state_phase Pointer to return phase for state record
  @param state_d Pointer to return density for state record
  @param state_h Pointer to return specific enthalpy for state record
  @param state_p Pointer to return pressure for state record
  @param state_s Pointer to return specific entropy for state record
  @param state_T Pointer to return temperature for state record
  @param mediumName Medium name
  @param libraryName Library name
  @param substanceName Substance name
*/
void TwoPhaseMedium_setState_pT_(double p, double T, int phase, int uniqueID, int *state_uniqueID, int *state_phase, double *state_d, double *state_h, double *state_p, double *state_s, double *state_T,
								 const char *mediumName, const char *libraryName, const char *substanceName){
	if (uniqueID == 0){
	  // setState_pT was called with uniqueID == 0
	  // create a new transient medium object and get a transientUniqueID
	  int transientUniqueID = MediumMap::addTransientMedium(mediumName, libraryName, substanceName);
      // run setState_pT_ with the transientUniqueID
	  TwoPhaseMedium_setState_pT_(p, T, phase, transientUniqueID, state_uniqueID, state_phase, state_d, state_h, state_p, state_s, state_T, mediumName, libraryName, substanceName);
	} else {
      // setState_pT was called with the uniqueID of an existing medium object
 	  // Call the medium object's setState_pT function
	  BaseTwoPhaseMedium *medium = MediumMap::medium(uniqueID);
	  medium->setState_pT(p, T);
	  // Return values
	  if (state_uniqueID != NULL)
  		  *state_uniqueID = uniqueID;
	  if (state_phase != NULL)
	 	  *state_phase = 1;
	  if (state_d != NULL)
		  *state_d = medium->d();
	  if (state_h != NULL)
		  *state_h = medium->h();
	  if (state_p != NULL)
		  *state_p = medium->p();
	  if (state_s != NULL)
		  *state_s = medium->s();
	  if (state_T != NULL)
		  *state_T = medium->T();
    }
}

//! Compute saturation properties from p
/*!
  This function computes the saturation properties for the specified inputs. If the function
  is called with uniqueID == 0 a new transient unique ID is assigned and the function
  is called again with this new transient unique ID number.
  @param p Pressure
  @param uniqueID Unique ID number
  @param sat_psat Pointer to return pressure for saturation record
  @param sat_Tsat Pointer to return temperature for saturation record
  @param sat_uniqueID Pointer to return unique ID number for saturation record
  @param mediumName Medium name
  @param libraryName Library name
  @param substanceName Substance name
*/
void TwoPhaseMedium_setSat_p_(double p, int uniqueID, double *sat_psat, double *sat_Tsat, int *sat_uniqueID,
							  const char *mediumName, const char *libraryName, const char *substanceName){
	// Pointer to the medium object
	BaseTwoPhaseMedium *medium;
	if (uniqueID == 0){
	  // setSat_p was called with uniqueID == 0
	  // create a new transient medium object and get a transientUniqueID
	  int transientUniqueID = MediumMap::addTransientMedium(mediumName, libraryName, substanceName);
      // run setSat_p with the transientUniqueID
	  TwoPhaseMedium_setSat_p_(p, transientUniqueID, sat_psat, sat_Tsat, sat_uniqueID, mediumName, libraryName, substanceName);
	} else {
      // setSat_p was called with the uniqueID of an existing medium object
 	  // get the pointer to the medium object
 	  medium = MediumMap::medium(uniqueID);
 	  // Call the medium object's setSat_p function
	  medium->setSat_p(p);
	  // Return values
	  if (sat_uniqueID != NULL)
  		  *sat_uniqueID = uniqueID;
	  if (sat_psat != NULL)
		  *sat_psat = medium->ps();
	  if (sat_Tsat != NULL)
		  *sat_Tsat = medium->Ts();
	}
}

//! Compute saturation properties from T
/*!
  This function computes the saturation properties for the specified inputs. If the function
  is called with uniqueID == 0 a new transient unique ID is assigned and the function
  is called again with this new transient unique ID number.
  @param T Temperature
  @param uniqueID Unique ID number
  @param sat_psat Pointer to return pressure for saturation record
  @param sat_Tsat Pointer to return temperature for saturation record
  @param sat_uniqueID Pointer to return unique ID number for saturation record
  @param mediumName Medium name
  @param libraryName Library name
  @param substanceName Substance name
*/
void TwoPhaseMedium_setSat_T_(double T, int uniqueID, double *sat_psat, double *sat_Tsat, int *sat_uniqueID,
							  const char *mediumName, const char *libraryName, const char *substanceName){
	// Pointer to the medium object
	BaseTwoPhaseMedium *medium;
	if (uniqueID == 0){
	  // setSat_T was called with uniqueID == 0
	  // create a new transient medium object and get a transientUniqueID
	  int transientUniqueID = MediumMap::addTransientMedium(mediumName, libraryName, substanceName);
      // run setSat_T with the transientUniqueID
	  TwoPhaseMedium_setSat_T_(T, transientUniqueID, sat_psat, sat_Tsat, sat_uniqueID, mediumName, libraryName, substanceName);
	} else {
      // setSat_T was called with the uniqueID of an existing medium object
 	  // get the pointer to the medium object
 	  medium = MediumMap::medium(uniqueID);
 	  // Call the medium object's setSat_T function
	  medium->setSat_T(T);
	  // Set the output values 
	  if (sat_uniqueID != NULL)
  		  *sat_uniqueID = uniqueID;
	  if (sat_psat != NULL)
		  *sat_psat = medium->ps();
	  if (sat_Tsat != NULL)
		  *sat_Tsat = medium->Ts();
	}
}

//! Compute saturation properties from within BaseProperties
/*!
  This function computes the saturation properties and is designed to be called
  from within the BaseProperties model. The saturation properties are set according
  to the medium pressure
  @param uniqueID Unique ID number
  @param sat_psat Pointer to return pressure for saturation record
  @param sat_Tsat Pointer to return temperature for saturation record
  @param sat_uniqueID Pointer to return unique ID number for saturation record
*/
void TwoPhaseMedium_setSat_p_state_(int uniqueID, double *sat_psat, double *sat_Tsat, int *sat_uniqueID){
	// Check for the validity of the uniqueID - this function should never be 
	// called with a zero unique ID
	if (uniqueID == 0)
		errorMessage("setSat_p_state called without a valid uniqueID");
	// Set a pointer to the medium
	BaseTwoPhaseMedium *medium;
	medium = MediumMap::medium(uniqueID);
	// Call the medium object setSat_p_state() function, which will
	// set the saturation properties corresponding to the medium pressure
	medium->setSat_p_state();
	// Set the output values 
	  if (sat_uniqueID != NULL)
  		  *sat_uniqueID = uniqueID;
	  if (sat_psat != NULL)
		  *sat_psat = medium->ps();
	  if (sat_Tsat != NULL)
		  *sat_Tsat = medium->Ts();
}

//! Compute dew state
/*!
  This function computes the dew state for the specified medium.
  @param uniqueID Unique ID number
  @param phase Phase (2 for two-phase, 1 for one-phase, 0 if not known)
  @param state_uniqueID Pointer to return unique ID number for state record
  @param state_phase Pointer to return phase for state record
  @param mediumName Medium name
  @param libraryName Library name
  @param substanceName Substance name
*/
void TwoPhaseMedium_setDewState_(int uniqueID, int phase, int *state_uniqueID, int *state_phase,
								 const char *mediumName, const char *libraryName, const char *substanceName){
	// Check for the validity of the uniqueID - this function should never be 
	// called with a zero unique ID or phase inputs
	if (uniqueID == 0)
		errorMessage("setDewState_ called without a valid uniqueID");
	if (phase < 1 || phase > 2)
		errorMessage("setDewState_ called with invalid phase");
	// Get the unique ID of the the dewState object, and allocate a new medium
	// object and set the dewState uniqueID if necessary
	int dewUniqueID = MediumMap::medium(uniqueID)->getDewUniqueID(phase);
	// Call the original medium object's setDewState function
	// which will compute the properties of the dew state and store them
	// in the medium with the dew state unique ID
    MediumMap::medium(uniqueID)->setDewState(phase);
    // Return values
	if (state_uniqueID != NULL)
  	  *state_uniqueID = dewUniqueID;
	if (state_phase != NULL)
	  *state_phase = phase;
}

//! Compute bubble state
/*!
  This function computes the  bubble state for the specified medium.
  @param uniqueID Unique ID number
  @param phase Phase (2 for two-phase, 1 for one-phase, 0 if not known)
  @param state_uniqueID Pointer to return unique ID number for state record
  @param state_phase Pointer to return phase for state record
  @param mediumName Medium name
  @param libraryName Library name
  @param substanceName Substance name
*/
void TwoPhaseMedium_setBubbleState_(int uniqueID, int phase, int *state_uniqueID, int *state_phase,
									const char *mediumName, const char *libraryName, const char *substanceName){
	// Check for the validity of the inputs - this function should never be 
	// called with a zero unique ID or phase inputs
	if (uniqueID == 0)
		errorMessage("setBubbleState_ called without a valid uniqueID");
	if (phase < 1 || phase > 2)
		errorMessage("setBubbleState_ called with invalid phase");
	// Get the unique ID of the the dewState object, and allocate a new medium
	// object and set the dewState uniqueID if necessary
	int bubbleUniqueID = MediumMap::medium(uniqueID)->getBubbleUniqueID(phase);
	// Call the original medium object's setBubbleState function
	// which will compute the properties of the dew state and store them
	// in the medium with the dew state unique ID
    MediumMap::medium(uniqueID)->setBubbleState(phase);
    // Return values
	if (state_uniqueID != NULL)
  	  *state_uniqueID = bubbleUniqueID;
	if (state_phase != NULL)
	  *state_phase = phase;
}

//! Return density of specified medium
double TwoPhaseMedium_density_(int uniqueID, int choice, double d, double h, double p, double s, double T, int phase,
							   const char *mediumName, const char *libraryName, const char *substanceName){
	if (uniqueID == 0)
	{
		BaseTwoPhaseMedium *medium = MediumMap::solverMedium(mediumName, libraryName, substanceName);
        TwoPhaseMedium_setStateDefault_(medium, choice, d, h, p, s, T, phase);
		return medium->d();
	}
	else 
	  return MediumMap::medium(uniqueID)->d();
}

//! Return derivative of density wrt pressure at constant specific enthalpy of specified medium
double TwoPhaseMedium_density_derp_h_(int uniqueID, int choice, double d, double h, double p, double s, double T, int phase,
									  const char *mediumName, const char *libraryName, const char *substanceName){
	if (uniqueID == 0)
	{
		BaseTwoPhaseMedium *medium = MediumMap::solverMedium(mediumName, libraryName, substanceName);
        TwoPhaseMedium_setStateDefault_(medium, choice, d, h, p, s, T, phase);
		return medium->dd_dp_h();
	}
	else 
	  return MediumMap::medium(uniqueID)->dd_dp_h();
}

//! Return derivative of density wrt specific enthalpy at constant pressure of specified medium
double TwoPhaseMedium_density_derh_p_(int uniqueID, int choice, double d, double h, double p, double s, double T, int phase,
									  const char *mediumName, const char *libraryName, const char *substanceName){
	if (uniqueID == 0)
	{
		BaseTwoPhaseMedium *medium = MediumMap::solverMedium(mediumName, libraryName, substanceName);
        TwoPhaseMedium_setStateDefault_(medium, choice, d, h, p, s, T, phase);
		return medium->dd_dh_p();
	}
	else 
		return MediumMap::medium(uniqueID)->dd_dh_p();
}

//! Return derivative of density wrt pressure and specific enthalpy of specified medium
double TwoPhaseMedium_density_ph_der_(int uniqueID, double p_der, double h_der, double p, double h, int phase,
									  const char *mediumName, const char *libraryName, const char *substanceName){
	if (uniqueID == 0)
	{
		BaseTwoPhaseMedium *medium = MediumMap::solverMedium(mediumName, libraryName, substanceName);
        medium->setState_ph(p, h, phase);
		return medium->dd_dp_h()*p_der +
		       medium->dd_dh_p()*h_der;
	}
	else 
		return MediumMap::medium(uniqueID)->dd_dp_h()*p_der +
			   MediumMap::medium(uniqueID)->dd_dh_p()*h_der;
}

//! Return pressure of specified medium
double TwoPhaseMedium_pressure_(int uniqueID, int choice, double d, double h, double p, double s, double T, int phase,
								const char *mediumName, const char *libraryName, const char *substanceName){
	if (uniqueID == 0)
	{
		BaseTwoPhaseMedium *medium = MediumMap::solverMedium(mediumName, libraryName, substanceName);
        TwoPhaseMedium_setStateDefault_(medium, choice, d, h, p, s, T, phase);
		return medium->p();
	}
	else 
		return MediumMap::medium(uniqueID)->p();
}

//! Return specific enthalpy of specified medium
double TwoPhaseMedium_specificEnthalpy_(int uniqueID, int choice, double d, double h, double p, double s, double T, int phase,
										const char *mediumName, const char *libraryName, const char *substanceName){
	if (uniqueID == 0)
	{
		BaseTwoPhaseMedium *medium = MediumMap::solverMedium(mediumName, libraryName, substanceName);
        TwoPhaseMedium_setStateDefault_(medium, choice, d, h, p, s, T, phase);
		return medium->h();
	}
	else 
		return MediumMap::medium(uniqueID)->h();
}

//! Return specific entropy of specified medium
double TwoPhaseMedium_specificEntropy_(int uniqueID, int choice, double d, double h, double p, double s, double T, int phase,
									   const char *mediumName, const char *libraryName, const char *substanceName){
	if (uniqueID == 0)
	{
		BaseTwoPhaseMedium *medium = MediumMap::solverMedium(mediumName, libraryName, substanceName);
        TwoPhaseMedium_setStateDefault_(medium, choice, d, h, p, s, T, phase);
		return medium->s();
	}
	else 
		return MediumMap::medium(uniqueID)->s();
}

//! Return temperature of specified medium
double TwoPhaseMedium_temperature_(int uniqueID, int choice, double d, double h, double p, double s, double T, int phase,
								   const char *mediumName, const char *libraryName, const char *substanceName){
	if (uniqueID == 0)
	{
		BaseTwoPhaseMedium *medium = MediumMap::solverMedium(mediumName, libraryName, substanceName);
        TwoPhaseMedium_setStateDefault_(medium, choice, d, h, p, s, T, phase);
		return medium->T();
	}
	else 
		return MediumMap::medium(uniqueID)->T();
}

//! Return derivative of temperature wrt pressure and specific enthalpy of specified medium
double TwoPhaseMedium_temperature_ph_der_(int uniqueID, double p_der, double h_der, double p, double h, int phase,
										  const char *mediumName, const char *libraryName, const char *substanceName){
	if (uniqueID == 0)
	{
		BaseTwoPhaseMedium *medium = MediumMap::solverMedium(mediumName, libraryName, substanceName);
        medium->setState_ph(p,h,phase);
		return medium->dT_dp_h()*p_der +
			   medium->dT_dh_p()*h_der;
	}
	else 
		return MediumMap::medium(uniqueID)->dT_dp_h()*p_der +
			   MediumMap::medium(uniqueID)->dT_dh_p()*h_der;
}

//! Return the enthalpy at pressure p after an isentropic transformation form the specified medium state
double TwoPhaseMedium_isentropicEnthalpy_(double p_iso, int uniqueID, int choice, double d, double h, double p, double s, double T, int phase,
										  const char *mediumName, const char *libraryName, const char *substanceName){
	if (uniqueID == 0)
	{
		BaseTwoPhaseMedium *medium = MediumMap::solverMedium(mediumName, libraryName, substanceName);
        TwoPhaseMedium_setStateDefault_(medium, choice, d, h, p, s, T, phase);
		return medium->h_iso(p_iso);
	}
	else 
		return MediumMap::medium(uniqueID)->h_iso(p_iso);
}

//! Return derivative of saturation temperature of specified medium from saturation properties
double TwoPhaseMedium_saturationTemperature_derp_sat_(double psat, double Tsat, int uniqueID,
													  const char *mediumName, const char *libraryName, const char *substanceName){
	// Use solver medium object if no unique ID is supplied
	if (uniqueID == 0)
	{
		BaseTwoPhaseMedium *medium = MediumMap::solverMedium(mediumName, libraryName, substanceName);
		medium->setSat_p(psat);
		return medium->d_Ts_dp();
	}
	else
		return MediumMap::medium(uniqueID)->d_Ts_dp();
}

//! Return bubble density of specified medium from saturation properties
double TwoPhaseMedium_bubbleDensity_(double psat, double Tsat, int uniqueID,
									 const char *mediumName, const char *libraryName, const char *substanceName){
	// Use solver medium object if no unique ID is supplied
	if (uniqueID == 0)
	{
		BaseTwoPhaseMedium *medium = MediumMap::solverMedium(mediumName, libraryName, substanceName);
		medium->setSat_p(psat);
		return medium->dl();
	}
	else
		return MediumMap::medium(uniqueID)->dl();
}

//! Return dew density of specified medium from saturation properties
double TwoPhaseMedium_dewDensity_(double psat, double Tsat, int uniqueID,
								  const char *mediumName, const char *libraryName, const char *substanceName){
	// Use solver medium object if no unique ID is supplied
	if (uniqueID == 0)
	{
		BaseTwoPhaseMedium *medium = MediumMap::solverMedium(mediumName, libraryName, substanceName);
		medium->setSat_p(psat);
		return medium->dv();
	}
	else
		return MediumMap::medium(uniqueID)->dv();
}

//! Return bubble specific enthalpy of specified medium from saturation properties
double TwoPhaseMedium_bubbleEnthalpy_(double psat, double Tsat, int uniqueID,
									  const char *mediumName, const char *libraryName, const char *substanceName){
	// Use solver medium object if no unique ID is supplied
	if (uniqueID == 0)
	{
		BaseTwoPhaseMedium *medium = MediumMap::solverMedium(mediumName, libraryName, substanceName);
		medium->setSat_p(psat);
		return medium->hl();
	}
	else
		return MediumMap::medium(uniqueID)->hl();
}

//! Return dew specific enthalpy of specified medium from saturation properties
double TwoPhaseMedium_dewEnthalpy_(double psat, double Tsat, int uniqueID,
								   const char *mediumName, const char *libraryName, const char *substanceName){
	// Use solver medium object if no unique ID is supplied
	if (uniqueID == 0)
	{
		BaseTwoPhaseMedium *medium = MediumMap::solverMedium(mediumName, libraryName, substanceName);
		medium->setSat_p(psat);
		return medium->hv();
	}
	else
		return MediumMap::medium(uniqueID)->hv();
}

//! Return bubble specific entropy of specified medium from saturation properties
double TwoPhaseMedium_bubbleEntropy_(double psat, double Tsat, int uniqueID,
									 const char *mediumName, const char *libraryName, const char *substanceName){
	// Use solver medium object if no unique ID is supplied
	if (uniqueID == 0)
	{
		BaseTwoPhaseMedium *medium = MediumMap::solverMedium(mediumName, libraryName, substanceName);
		medium->setSat_p(psat);
		return medium->sl();
	}
	else
		return MediumMap::medium(uniqueID)->sl();
}

//! Return dew specific entropy of specified medium from saturation properties
double TwoPhaseMedium_dewEntropy_(double psat, double Tsat, int uniqueID,
								  const char *mediumName, const char *libraryName, const char *substanceName){
	// Use solver medium object if no unique ID is supplied
	if (uniqueID == 0)
	{
		BaseTwoPhaseMedium *medium = MediumMap::solverMedium(mediumName, libraryName, substanceName);
		medium->setSat_p(psat);
		return medium->sv();
	}
	else
		return MediumMap::medium(uniqueID)->sv();
}

//! Return derivative of bubble density wrt pressure of specified medium from saturation properties
double TwoPhaseMedium_dBubbleDensity_dPressure_(double psat, double Tsat, int uniqueID,
												const char *mediumName, const char *libraryName, const char *substanceName){
	// Use solver medium object if no unique ID is supplied
	if (uniqueID == 0)
	{
		BaseTwoPhaseMedium *medium = MediumMap::solverMedium(mediumName, libraryName, substanceName);
		medium->setSat_p(psat);
		return medium->d_dl_dp();
	}
	else
		return MediumMap::medium(uniqueID)->d_dl_dp();
}

//! Return derivative of dew density wrt pressure of specified medium from saturation properties
double TwoPhaseMedium_dDewDensity_dPressure_(double psat, double Tsat, int uniqueID,
											 const char *mediumName, const char *libraryName, const char *substanceName){
	// Use solver medium object if no unique ID is supplied
	if (uniqueID == 0)
	{
		BaseTwoPhaseMedium *medium = MediumMap::solverMedium(mediumName, libraryName, substanceName);
		medium->setSat_p(psat);
		return medium->d_dv_dp();
	}
	else
		return MediumMap::medium(uniqueID)->d_dv_dp();
}

//! Return derivative of bubble specific enthalpy wrt pressure of specified medium from saturation properties
double TwoPhaseMedium_dBubbleEnthalpy_dPressure_(double psat, double Tsat, int uniqueID,
												 const char *mediumName, const char *libraryName, const char *substanceName){
	// Use solver medium object if no unique ID is supplied
	if (uniqueID == 0)
	{
		BaseTwoPhaseMedium *medium = MediumMap::solverMedium(mediumName, libraryName, substanceName);
		medium->setSat_p(psat);
		return medium->d_hl_dp();
	}
	else
		return MediumMap::medium(uniqueID)->d_hl_dp();
}

//! Return derivative of dew specific enthalpy wrt pressure of specified medium from saturation properties
double TwoPhaseMedium_dDewEnthalpy_dPressure_(double psat, double Tsat, int uniqueID,
											  const char *mediumName, const char *libraryName, const char *substanceName){
	// Use solver medium object if no unique ID is supplied
	if (uniqueID == 0)
	{
		BaseTwoPhaseMedium *medium = MediumMap::solverMedium(mediumName, libraryName, substanceName);
		medium->setSat_p(psat);
		return medium->d_hv_dp();
	}
	else
		return MediumMap::medium(uniqueID)->d_hv_dp();
}

//! Return isobaric expansion coefficient of specified medium
double TwoPhaseMedium_isobaricExpansionCoefficient_(int uniqueID, int choice, double d, double h, double p, double s, double T, int phase,
													const char *mediumName, const char *libraryName, const char *substanceName){
	if (uniqueID == 0)
	{
		BaseTwoPhaseMedium *medium = MediumMap::solverMedium(mediumName, libraryName, substanceName);
        TwoPhaseMedium_setStateDefault_(medium, choice, d, h, p, s, T, phase);
		return medium->beta();
	}
	else 
		return MediumMap::medium(uniqueID)->beta();
}

//! Return isothermal compressibility of specified medium
double TwoPhaseMedium_isothermalCompressibility_(int uniqueID, int choice, double d, double h, double p, double s, double T, int phase,
												 const char *mediumName, const char *libraryName, const char *substanceName){
	if (uniqueID == 0)
	{
		BaseTwoPhaseMedium *medium = MediumMap::solverMedium(mediumName, libraryName, substanceName);
        TwoPhaseMedium_setStateDefault_(medium, choice, d, h, p, s, T, phase);
		return medium->kappa();
	}
	else 
		return MediumMap::medium(uniqueID)->kappa();
}

//! Return specific heat capacity cp of specified medium
double TwoPhaseMedium_specificHeatCapacityCp_(int uniqueID, int choice, double d, double h, double p, double s, double T, int phase,
											  const char *mediumName, const char *libraryName, const char *substanceName){
	if (uniqueID == 0)
	{
		BaseTwoPhaseMedium *medium = MediumMap::solverMedium(mediumName, libraryName, substanceName);
        TwoPhaseMedium_setStateDefault_(medium, choice, d, h, p, s, T, phase);
		return medium->cp();
	}
	else 
		return MediumMap::medium(uniqueID)->cp();
}

//! Return specific heat capacity cv of specified medium
double TwoPhaseMedium_specificHeatCapacityCv_(int uniqueID, int choice, double d, double h, double p, double s, double T, int phase,
											  const char *mediumName, const char *libraryName, const char *substanceName){
	if (uniqueID == 0)
	{
		BaseTwoPhaseMedium *medium = MediumMap::solverMedium(mediumName, libraryName, substanceName);
        TwoPhaseMedium_setStateDefault_(medium, choice, d, h, p, s, T, phase);
		return medium->cv();
	}
	else 
		return MediumMap::medium(uniqueID)->cv();
}

//! Return dynamic viscosity of specified medium
double TwoPhaseMedium_dynamicViscosity_(int uniqueID, int choice, double d, double h, double p, double s, double T, int phase,
										const char *mediumName, const char *libraryName, const char *substanceName){
	if (uniqueID == 0)
	{
		BaseTwoPhaseMedium *medium = MediumMap::solverMedium(mediumName, libraryName, substanceName);
        TwoPhaseMedium_setStateDefault_(medium, choice, d, h, p, s, T, phase);
		return medium->eta();
	}
	else 
		return MediumMap::medium(uniqueID)->eta();
}

//! Return thermal conductivity of specified medium
double TwoPhaseMedium_thermalConductivity_(int uniqueID, int choice, double d, double h, double p, double s, double T, int phase,
										   const char *mediumName, const char *libraryName, const char *substanceName){
	if (uniqueID == 0)
	{
		BaseTwoPhaseMedium *medium = MediumMap::solverMedium(mediumName, libraryName, substanceName);
        TwoPhaseMedium_setStateDefault_(medium, choice, d, h, p, s, T, phase);
		return medium->lambda();
	}
	else 
		return MediumMap::medium(uniqueID)->lambda();
}

//! Return Prandtl number of specified medium
double TwoPhaseMedium_prandtlNumber_(int uniqueID, int choice, double d, double h, double p, double s, double T, int phase,
									 const char *mediumName, const char *libraryName, const char *substanceName){
	if (uniqueID == 0)
	{
		BaseTwoPhaseMedium *medium = MediumMap::solverMedium(mediumName, libraryName, substanceName);
        TwoPhaseMedium_setStateDefault_(medium, choice, d, h, p, s, T, phase);
		return medium->Pr();
	}
	else 
		return MediumMap::medium(uniqueID)->Pr();
}

//! Return surface tension of specified medium
double TwoPhaseMedium_surfaceTension_(double psat, double Tsat, int uniqueID,
									  const char *mediumName, const char *libraryName, const char *substanceName){
	// Use solver medium object if no unique ID is supplied
	if (uniqueID == 0)
	{
		BaseTwoPhaseMedium *medium = MediumMap::solverMedium(mediumName, libraryName, substanceName);
		medium->setSat_p(psat);
		return medium->sigma();
	}
	else
		return MediumMap::medium(uniqueID)->sigma();
}

//! Return velocity of sound of specified medium
double TwoPhaseMedium_velocityOfSound_(int uniqueID, int choice, double d, double h, double p, double s, double T, int phase,
									   const char *mediumName, const char *libraryName, const char *substanceName){
	if (uniqueID == 0)
	{
		BaseTwoPhaseMedium *medium = MediumMap::solverMedium(mediumName, libraryName, substanceName);
        TwoPhaseMedium_setStateDefault_(medium, choice, d, h, p, s, T, phase);
		return medium->a();
	}
	else 
		return MediumMap::medium(uniqueID)->a();
}

//! Return derivative of density wrt pressure at constant specific enthalpy of specified medium
double TwoPhaseMedium_dDensity_dPressure_h_(int uniqueID, int choice, double d, double h, double p, double s, double T, int phase,
											const char *mediumName, const char *libraryName, const char *substanceName){
	if (uniqueID == 0)
	{
		BaseTwoPhaseMedium *medium = MediumMap::solverMedium(mediumName, libraryName, substanceName);
        TwoPhaseMedium_setStateDefault_(medium, choice, d, h, p, s, T, phase);
		return medium->dd_dp_h();
	}
	else 
		return MediumMap::medium(uniqueID)->dd_dp_h();
}

//! Return derivative of density wrt specific enthalpy at constant pressure of specified medium
double TwoPhaseMedium_dDensity_dEnthalpy_p_(int uniqueID, int choice, double d, double h, double p, double s, double T, int phase,
											const char *mediumName, const char *libraryName, const char *substanceName){
	if (uniqueID == 0)
	{
		BaseTwoPhaseMedium *medium = MediumMap::solverMedium(mediumName, libraryName, substanceName);
        TwoPhaseMedium_setStateDefault_(medium, choice, d, h, p, s, T, phase);
		return medium->dd_dh_p();
	}
	else 
		return MediumMap::medium(uniqueID)->dd_dh_p();
}

//! Compute saturation pressure for specified medium and temperature
double TwoPhaseMedium_saturationPressure_(double T, const char *mediumName, const char *libraryName, const char *substanceName){
	// Get medium object
	BaseTwoPhaseMedium *medium = MediumMap::solverMedium(mediumName, libraryName, substanceName);
	// Compute saturation pressure
	medium->setSat_T(T);
	// Return saturation pressure
	return medium->ps();
}

//! Compute saturation temperature for specified medium and pressure
double TwoPhaseMedium_saturationTemperature_(double p, const char *mediumName, const char *libraryName, const char *substanceName){
	// Get medium object
	BaseTwoPhaseMedium *medium = MediumMap::solverMedium(mediumName, libraryName, substanceName);
	// Compute saturation pressure
	medium->setSat_p(p);
	return medium->Ts();
}

//! Compute derivative of saturation temperature for specified medium and pressure
double TwoPhaseMedium_saturationTemperature_derp_(double p, const char *mediumName, const char *libraryName, const char *substanceName){
	// Get medium object
	BaseTwoPhaseMedium *medium = MediumMap::solverMedium(mediumName, libraryName, substanceName);
	// Compute saturation pressure
	medium->setSat_p(p);
	return medium->d_Ts_dp();
}

//! Call the appropriate setState_xx() function of the medium object
/*!
  This function calls the setState_xx() function of the medium object corresponding 
  to the choice input.
  @param medium Medium object (class BaseTwoPhaseMedium)
  @param choice of inputs (dT, ph, ps, pT)
  @param d Density
  @param h Specific Enthalpy
  @param p Pressure
  @param s Specific entropy
  @param T Temperature
  @param phase Phase (2 for two-phase, 1 for one-phase, 0 if not known)
  @param nComp Number of components in the mixture
*/
void TwoPhaseMedium_setStateDefault_(BaseTwoPhaseMedium *medium, int choice, double d, double h, double p, double s, double T, int phase, int nComp){
	if(choice == CHOICE_dT)
		medium->setState_dT(d,T,phase, nComp);
	else if(choice == CHOICE_ph)
		medium->setState_ph(p,h,phase, nComp);
	else if(choice == CHOICE_ps)
		medium->setState_ps(p,s,phase, nComp);
	else if(choice == CHOICE_pT)
		medium->setState_pT(p,T, nComp);
	else
		errorMessage("Wrong choice of inputs in setStateDefault_()\n");
}

//-----------------------------------------
// Functions for Mixtures (TwoPhaseMixture)
//-----------------------------------------

//! Get molar mass
/*!
  This function returns the molar mass of the specified mixture.
  @param mediumName Medium name
  @param libraryName Library name
  @param substanceName Substance name
*/
double TwoPhaseMixture_getMolarMass_(int nComp, double Conc[], const char *mediumName, const char *libraryName, const char *substanceName){
	// Return molar mass
	return SolverMap::getSolver(mediumName, libraryName, substanceName, nComp, Conc)->molarMass();
}

//! Compute properties from d, T, x and phase
/*!
  This function computes the properties for the specified inputs. If the function
  is called with uniqueID == 0 a new transient unique ID is assigned and the function
  is called again with this new transient unique ID number.
  @param d Density
  @param T Temperature
  @param phase Phase (2 for two-phase, 1 for one-phase, 0 if not known)
  @param nComp Number of Components in the Mixtures
  @param Conc Vector of the Concentrations of each Component
  @param uniqueID Unique ID number
  @param state_uniqueID Pointer to return unique ID number for state record
  @param state_phase Pointer to return phase for state record
  @param state_d Pointer to return density for state record
  @param state_h Pointer to return specific enthalpy for state record
  @param state_p Pointer to return pressure for state record
  @param state_s Pointer to return specific entropy for state record
  @param state_T Pointer to return temperature for state record
  @param state_x Pointer to return the liquid phase composition for state record
  @param state_y Pointer to return the vapor phase composition for state record
  @param mediumName Medium name
  @param libraryName Library name
  @param substanceName Substance name
*/
void TwoPhaseMixture_setState_dTx_(double d, double T, int phase, int nComp, double Conc[], int uniqueID, int *state_uniqueID, int *state_phase, double *state_d, double *state_h, double *state_p, double *state_s, double *state_T, double *state_x, double *state_y,
								 const char *mediumName, const char *libraryName, const char *substanceName){
	// Auxilary Pointer to get the equilibrium Concentrations
	double *x, *y;

	if (uniqueID == 0){
	  // setState_dT was called with uniqueID == 0
	  // create a new transient medium object and get a transientUniqueID
	  int transientUniqueID = MediumMap::addTransientMedium(mediumName, libraryName, substanceName, nComp, Conc);
      // run setState_dT_ with the transientUniqueID
	  TwoPhaseMixture_setState_dTx_(d, T, phase, nComp, Conc, transientUniqueID, state_uniqueID, state_phase, state_d, state_h, state_p, state_s, state_T, state_x, state_y, mediumName, libraryName, substanceName);
	} else {
      // setState_dT was called with the uniqueID of an existing medium object
 	  // Call the medium object's setState_dT function
	  BaseTwoPhaseMedium *medium = MediumMap::medium(uniqueID);
	  medium->setState_dT(d, T, phase, nComp);
	  // Return values
	  if (state_uniqueID != NULL)
  		  *state_uniqueID = uniqueID;
	  if (state_phase != NULL)
	 	  *state_phase = MediumMap::medium(uniqueID)->phase();
	  if (state_d != NULL)
		  *state_d = medium->d();
	  if (state_h != NULL)
		  *state_h = medium->h();
	  if (state_p != NULL)
		  *state_p = medium->p();
	  if (state_s != NULL)
		  *state_s = medium->s();
	  if (state_T != NULL)
		  *state_T = medium->T();
	  x = medium->x();
	  y = medium->y();
	  for (int i = 0; i < nComp; i++) {
			if (state_x != NULL)
				  state_x[i] = x[i];
			if (state_y != NULL)
				  state_y[i] = y[i];
	  } 
    }
}

//! Compute properties from p, h, X and phase
/*!
  This function computes the properties for the specified inputs. If the function
  is called with uniqueID == 0 a new transient unique ID is assigned and the function
  is called again with this new transient unique ID number.
  @param p Pressure
  @param h Specific enthalpy
  @param phase Phase (2 for two-phase, 1 for one-phase, 0 if not known)
  @param nComp Number of Components in the Mixtures
  @param Conc Vector of the Concentrations of each Component
  @param uniqueID Unique ID number
  @param state_uniqueID Pointer to return unique ID number for state record
  @param state_phase Pointer to return phase for state record
  @param state_d Pointer to return density for state record
  @param state_h Pointer to return specific enthalpy for state record
  @param state_p Pointer to return pressure for state record
  @param state_s Pointer to return specific entropy for state record
  @param state_T Pointer to return temperature for state record
  @param state_x Pointer to return the liquid phase composition for state record
  @param state_y Pointer to return the vapor phase composition for state record
  @param mediumName Medium name
  @param libraryName Library name
  @param substanceName Substance name
*/
void TwoPhaseMixture_setState_phx_(double p, double h, int phase, int nComp, double Conc[], int uniqueID, int *state_uniqueID, int *state_phase, double *state_d, double *state_h, double *state_p, double *state_s, double *state_T, double *state_x, double *state_y,
								 const char *mediumName, const char *libraryName, const char *substanceName){
	// Auxilary Pointer to get the equilibrium Concentrations
	double *x, *y;

	if (uniqueID == 0){
	  // setState_ph was called with uniqueID == 0
	  // create a new transient medium object and get a transientUniqueID
	  int transientUniqueID = MediumMap::addTransientMedium(mediumName, libraryName, substanceName, nComp, Conc);
      // run setState_ph_ with the transientUniqueID
	  TwoPhaseMixture_setState_phx_(p, h, phase, nComp, Conc, transientUniqueID, state_uniqueID, state_phase, state_d, state_h, state_p, state_s, state_T, state_x, state_y, mediumName, libraryName, substanceName);
	} else {
      // setState_ph was called with the uniqueID of an existing medium object
 	  // Call the medium object's setState_ph function
	  BaseTwoPhaseMedium *medium = MediumMap::medium(uniqueID);
	  medium->setState_ph(p, h, phase, nComp);
	  // Return values
	  if (state_uniqueID != NULL)
  		  *state_uniqueID = uniqueID;
	  if (state_phase != NULL)
	 	  *state_phase = MediumMap::medium(uniqueID)->phase();
	  if (state_d != NULL)
		  *state_d = medium->d();
	  if (state_h != NULL)
		  *state_h = medium->h();
	  if (state_p != NULL)
		  *state_p = medium->p();
	  if (state_s != NULL)
		  *state_s = medium->s();
	  if (state_T != NULL)
		  *state_T = medium->T();
	  x = medium->x();
	  y = medium->y();
	  for (int i = 0; i < nComp; i++) {
			if (state_x != NULL)
				  state_x[i] = x[i];
			if (state_y != NULL)
				  state_y[i] = y[i];
	  } 
    }
}

//! Compute properties from p, s, x and phase
/*!
  This function computes the properties for the specified inputs. If the function
  is called with uniqueID == 0 a new transient unique ID is assigned and the function
  is called again with this new transient unique ID number.
  @param p Pressure
  @param s Specific entropy
  @param phase Phase (2 for two-phase, 1 for one-phase, 0 if not known)
  @param nComp Number of Components in the Mixtures
  @param Conc Vector of the Concentrations of each Component
  @param uniqueID Unique ID number
  @param state_uniqueID Pointer to return unique ID number for state record
  @param state_phase Pointer to return phase for state record
  @param state_d Pointer to return density for state record
  @param state_h Pointer to return specific enthalpy for state record
  @param state_p Pointer to return pressure for state record
  @param state_s Pointer to return specific entropy for state record
  @param state_T Pointer to return temperature for state record
  @param state_x Pointer to return the liquid phase composition for state record
  @param state_y Pointer to return the vapor phase composition for state record
  @param mediumName Medium name
  @param libraryName Library name
  @param substanceName Substance name
*/
void TwoPhaseMixture_setState_psx_(double p, double s, int phase, int nComp, double Conc[], int uniqueID, int *state_uniqueID, int *state_phase, double *state_d, double *state_h, double *state_p, double *state_s, double *state_T, double *state_x, double *state_y,
								 const char *mediumName, const char *libraryName, const char *substanceName){
	
	// Auxilary Pointer to get the equilibrium Concentrations
	double *x, *y;

	if (uniqueID == 0){
	  // setState_ps was called with uniqueID == 0
	  // create a new transient medium object and get a transientUniqueID
	  int transientUniqueID = MediumMap::addTransientMedium(mediumName, libraryName, substanceName, nComp, Conc);
      // run setState_ps_ with the transientUniqueID
	  TwoPhaseMixture_setState_psx_(p, s, phase, nComp, Conc, transientUniqueID, state_uniqueID, state_phase, state_d, state_h, state_p, state_s, state_T, state_x, state_y, mediumName, libraryName, substanceName);
	} else {
      // setState_ps was called with the uniqueID of an existing medium object
 	  // Call the medium object's setState_ps function
	  BaseTwoPhaseMedium *medium = MediumMap::medium(uniqueID);
	  medium->setState_ps(p, s, phase, nComp);
	  // Return values
	  if (state_uniqueID != NULL)
  		  *state_uniqueID = uniqueID;
	  if (state_phase != NULL)
	 	  *state_phase = MediumMap::medium(uniqueID)->phase();
	  if (state_d != NULL)
		  *state_d = medium->d();
	  if (state_h != NULL)
		  *state_h = medium->h();
	  if (state_p != NULL)
		  *state_p = medium->p();
	  if (state_s != NULL)
		  *state_s = medium->s();
	  if (state_T != NULL)
		  *state_T = medium->T();
	  x = medium->x();
	  y = medium->y();
	  for (int i = 0; i < nComp; i++) {
			if (state_x != NULL)
				  state_x[i] = x[i];
			if (state_y != NULL)
				  state_y[i] = y[i];
	  }
    }
}

//! Compute properties from p, T, X and phase
/*!
  This function computes the properties for the specified inputs. If the function
  is called with uniqueID == 0 a new transient unique ID is assigned and the function
  is called again with this new transient unique ID number.

  Attention: The phase input is ignored for this function!
  @param p Pressure
  @param T Temperature
  @param phase Phase (2 for two-phase, 1 for one-phase, 0 if not known)
  @param nComp Number of Components in the Mixtures
  @param Conc Vector of the Concentrations of each Component
  @param uniqueID Unique ID number
  @param state_uniqueID Pointer to return unique ID number for state record
  @param state_phase Pointer to return phase for state record
  @param state_d Pointer to return density for state record
  @param state_h Pointer to return specific enthalpy for state record
  @param state_p Pointer to return pressure for state record
  @param state_s Pointer to return specific entropy for state record
  @param state_T Pointer to return temperature for state record
  @param state_x Pointer to return the liquid phase composition for state record
  @param state_y Pointer to return the vapor phase composition for state record
  @param mediumName Medium name
  @param libraryName Library name
  @param substanceName Substance name
*/
void TwoPhaseMixture_setState_pTx_(double p, double T, int phase, int nComp, double Conc[], int uniqueID, int *state_uniqueID, int *state_phase, double *state_d, double *state_h, double *state_p, double *state_s, double *state_T, double *state_x, double *state_y,
								 const char *mediumName, const char *libraryName, const char *substanceName){
	
	// Auxilary Pointer to get the equilibrium Concentrations
	double *x, *y;

	if (uniqueID == 0){
		// setState_pT was called with uniqueID == 0
		// create a new transient medium object and get a transientUniqueID
		int transientUniqueID = MediumMap::addTransientMedium(mediumName, libraryName, substanceName, nComp, Conc);
		// run setState_pT_ with the transientUniqueID
		TwoPhaseMixture_setState_pTx_(p, T, phase, nComp, Conc, transientUniqueID, state_uniqueID, state_phase, state_d, state_h, state_p, state_s, state_T, state_x, state_y, mediumName, libraryName, substanceName);
	} else {
		// setState_pT was called with the uniqueID of an existing medium object
 		// Call the medium object's setState_pT function
		BaseTwoPhaseMedium *medium = MediumMap::medium(uniqueID);
		medium->setState_pT(p, T, nComp);
		// Return values
		if (state_uniqueID != NULL)
  			  *state_uniqueID = uniqueID;
		if (state_phase != NULL)
	 		  *state_phase = 1;
		if (state_d != NULL)
			  *state_d = medium->d();
		if (state_h != NULL)
			  *state_h = medium->h();
		if (state_p != NULL)
			  *state_p = medium->p();
		if (state_s != NULL)
			  *state_s = medium->s();
		if (state_T != NULL)
			  *state_T = medium->T();
		x = medium->x();
		y = medium->y();
		for (int i = 0; i < nComp; i++) {
			if (state_x != NULL)
				  state_x[i] = x[i];
			if (state_y != NULL)
				  state_y[i] = y[i];
		} 
	}
}

//! Return density of specified mixture
double TwoPhaseMixture_density_(int uniqueID, int choice, double d, double h, double p, double s, double T, int phase, int nComp, double Conc[],
							   const char *mediumName, const char *libraryName, const char *substanceName){
	if (uniqueID == 0)
	{
		BaseTwoPhaseMedium *medium = MediumMap::solverMedium(mediumName, libraryName, substanceName, nComp, Conc);
        TwoPhaseMedium_setStateDefault_(medium, choice, d, h, p, s, T, phase, nComp);
		return medium->d();
	}
	else 
	  return MediumMap::medium(uniqueID)->d();
}

//! Return pressure of specified mixture
double TwoPhaseMixture_pressure_(int uniqueID, int choice, double d, double h, double p, double s, double T, int phase, int nComp, double Conc[],
								const char *mediumName, const char *libraryName, const char *substanceName){
	if (uniqueID == 0)
	{
		BaseTwoPhaseMedium *medium = MediumMap::solverMedium(mediumName, libraryName, substanceName, nComp, Conc);
        TwoPhaseMedium_setStateDefault_(medium, choice, d, h, p, s, T, phase, nComp);
		return medium->p();
	}
	else 
		return MediumMap::medium(uniqueID)->p();
}

//! Return specific enthalpy of specified mixture
double TwoPhaseMixture_specificEnthalpy_(int uniqueID, int choice, double d, double h, double p, double s, double T, int phase, int nComp, double Conc[],
										const char *mediumName, const char *libraryName, const char *substanceName){
	if (uniqueID == 0)
	{
		BaseTwoPhaseMedium *medium = MediumMap::solverMedium(mediumName, libraryName, substanceName, nComp, Conc);
        TwoPhaseMedium_setStateDefault_(medium, choice, d, h, p, s, T, phase, nComp);
		return medium->h();
	}
	else 
		return MediumMap::medium(uniqueID)->h();
}

//! Return specific entropy of specified mixture
double TwoPhaseMixture_specificEntropy_(int uniqueID, int choice, double d, double h, double p, double s, double T, int phase, int nComp, double Conc[],
									   const char *mediumName, const char *libraryName, const char *substanceName){
	if (uniqueID == 0)
	{
		BaseTwoPhaseMedium *medium = MediumMap::solverMedium(mediumName, libraryName, substanceName, nComp, Conc);
        TwoPhaseMedium_setStateDefault_(medium, choice, d, h, p, s, T, phase, nComp);
		return medium->s();
	}
	else 
		return MediumMap::medium(uniqueID)->s();
}

//! Return temperature of specified mixture
double TwoPhaseMixture_temperature_(int uniqueID, int choice, double d, double h, double p, double s, double T, int phase, int nComp, double Conc[],
								   const char *mediumName, const char *libraryName, const char *substanceName){
	if (uniqueID == 0)
	{
		BaseTwoPhaseMedium *medium = MediumMap::solverMedium(mediumName, libraryName, substanceName, nComp, Conc);
        TwoPhaseMedium_setStateDefault_(medium, choice, d, h, p, s, T, phase, nComp);
		return medium->T();
	}
	else 
		return MediumMap::medium(uniqueID)->T();
}

//! Compute bubble temperature for specified mixture composition and pressure
double TwoPhaseMixture_bubbleTemperature_(double p, int nComp, double Conc[], const char *mediumName, const char *libraryName, const char *substanceName){
	// Get medium object
	BaseTwoPhaseMedium *medium = MediumMap::solverMedium(mediumName, libraryName, substanceName, nComp, Conc);
	// Compute saturation pressure
	medium->setSat_p(p);
	return medium->T();
}

//! Compute dew temperature for specified mixture composition and pressure
double TwoPhaseMixture_dewTemperature_(double p, int nComp, double Conc[], const char *mediumName, const char *libraryName, const char *substanceName){
	// Get medium object
	BaseTwoPhaseMedium *medium = MediumMap::solverMedium(mediumName, libraryName, substanceName, nComp, Conc);
	// Compute saturation pressure
	medium->setSat_p(p);
	return medium->Ts();
}

//! Return derivative of density wrt pressure and specific enthalpy of specified medium
double TwoPhaseMixture_density_ph_der_(int uniqueID, double p_der, double h_der, double p, double h, int phase, int nComp, double Conc[],
									  const char *mediumName, const char *libraryName, const char *substanceName){
	if (uniqueID == 0)
	{
		BaseTwoPhaseMedium *medium = MediumMap::solverMedium(mediumName, libraryName, substanceName, nComp, Conc);
        medium->setState_ph(p, h, phase, nComp);
		return medium->dd_dp_h()*p_der +
		       medium->dd_dh_p()*h_der;
	}
	else 
		return MediumMap::medium(uniqueID)->dd_dp_h()*p_der +
			   MediumMap::medium(uniqueID)->dd_dh_p()*h_der;
}


//! Return specific heat capacity cp of specified mixture
double TwoPhaseMixture_specificHeatCapacityCp_(int uniqueID, int choice, double d, double h, double p, double s, double T, int phase, int nComp, double Conc[],
											  const char *mediumName, const char *libraryName, const char *substanceName){
	if (uniqueID == 0)
	{
		BaseTwoPhaseMedium *medium = MediumMap::solverMedium(mediumName, libraryName, substanceName, nComp, Conc);
        TwoPhaseMedium_setStateDefault_(medium, choice, d, h, p, s, T, phase, nComp);
		return medium->cp();
	}
	else 
		return MediumMap::medium(uniqueID)->cp();
}

//! Return specific heat capacity cv of specified mixture
double TwoPhaseMixture_specificHeatCapacityCv_(int uniqueID, int choice, double d, double h, double p, double s, double T, int phase, int nComp, double Conc[],
											  const char *mediumName, const char *libraryName, const char *substanceName){
	if (uniqueID == 0)
	{
		BaseTwoPhaseMedium *medium = MediumMap::solverMedium(mediumName, libraryName, substanceName, nComp, Conc);
        TwoPhaseMedium_setStateDefault_(medium, choice, d, h, p, s, T, phase, nComp);
		return medium->cv();
	}
	else 
		return MediumMap::medium(uniqueID)->cv();
}

//! Return dynamic viscosity of specified mixture
double TwoPhaseMixture_dynamicViscosity_(int uniqueID, int choice, double d, double h, double p, double s, double T, int phase, int nComp, double Conc[],
										const char *mediumName, const char *libraryName, const char *substanceName){
	if (uniqueID == 0)
	{
		BaseTwoPhaseMedium *medium = MediumMap::solverMedium(mediumName, libraryName, substanceName, nComp, Conc);
        TwoPhaseMedium_setStateDefault_(medium, choice, d, h, p, s, T, phase, nComp);
		return medium->eta();
	}
	else 
		return MediumMap::medium(uniqueID)->eta();
}

//! Return thermal conductivity of specified mixture
double TwoPhaseMixture_thermalConductivity_(int uniqueID, int choice, double d, double h, double p, double s, double T, int phase, int nComp, double Conc[],
										   const char *mediumName, const char *libraryName, const char *substanceName){
	if (uniqueID == 0)
	{
		BaseTwoPhaseMedium *medium = MediumMap::solverMedium(mediumName, libraryName, substanceName, nComp, Conc);
        TwoPhaseMedium_setStateDefault_(medium, choice, d, h, p, s, T, phase, nComp);
		return medium->lambda();
	}
	else 
		return MediumMap::medium(uniqueID)->lambda();
}
