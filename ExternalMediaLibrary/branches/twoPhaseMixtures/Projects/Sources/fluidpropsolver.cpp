/* *****************************************************************
 * Implementation of class FluidProp solver
 *
 * Francesco Casella, Christoph Richter, Oct 2006 - May 2007
 * modified by Andreas Joos (Hamburg University of Technology), Jun 2009
 ********************************************************************/

#include <sstream>
#include "fluidpropsolver.h"
#include "twophasemediumproperties.h"

#if (FLUIDPROP == 1)
#define _AFXDLL
FluidPropSolver::FluidPropSolver(const string &mediumName,
								 const string &libraryName,
								 const string &substanceName)
	: BaseSolver(mediumName, libraryName, substanceName){
	string ErrorMsg;
	string Comp[MAX_NO_COMPONENTS];
    double Conc[MAX_NO_COMPONENTS];

    // Build FluidProp object with the libraryName and substanceName info
	Comp[0] = substanceName.c_str();
    FluidProp.SetFluid(libraryName.substr(libraryName.find(".")+1), 1, Comp, Conc, &ErrorMsg);
	if (isError(ErrorMsg))  // An error occurred
	{
		// Build error message and pass it to the Modelica environment
		char error[300];
		sprintf(error, "FluidProp error: %s\n", ErrorMsg.c_str());
		errorMessage(error);
	}

	// Set SI units and if requested molar specific units, Molar Mass always in g/mol!
	if (libraryName.find("Molar") != string::npos)
		FluidProp.SetUnits("SI", "PerMole", "Mmol", "g/mol", &ErrorMsg);
	else
		FluidProp.SetUnits("SI", "PerMass", "Mmol", "g/mol", &ErrorMsg);
	if (isError(ErrorMsg))  // An error occurred
	{
		// Build error message and pass it to the Modelica environment
		char error[300];
		sprintf(error, "FluidProp error: %s\n", ErrorMsg.c_str());
		errorMessage(error);
	}

	// Set fluid constants
	setFluidConstants();
}

FluidPropSolver::FluidPropSolver(const string &mediumName,
								 const string &libraryName,
								 const string &substanceName,
								 const int nComp,
								 double* Conc)
	: BaseSolver(mediumName, libraryName, substanceName, nComp, Conc){
	string ErrorMsg;
	string Comp[MAX_NO_COMPONENTS];

	// Anfang �BERPR�FEN
	// aux variables
	string substanceName_cpy = substanceName;
	int loc, nb = 0;

	//Build table of substance name from "-" separated string
	while ((loc = substanceName_cpy.find("-")) != string::npos) {
		if (loc == 0) {
			substanceName_cpy = substanceName_cpy.erase(0, loc+1);
			continue;
		}
		Comp[nb] = substanceName_cpy.substr(0, loc);
		substanceName_cpy = substanceName_cpy.erase(0, loc+1);
		nb++;
	}
	if (substanceName_cpy.length() != 0)
		Comp[nb++] = substanceName_cpy;

	// StanMix needs one String for the components names, RefProp an array with a string for each component 
	if (libraryName.substr(libraryName.find(".")+1) == "StanMix") {
		for (int i = 1; i < nb; i++) {
			if (i != nb)
				Comp[0] += "-";
			Comp[0] += Comp[i];
		}
	}
	// ENDE �BERPR�FEN

	// Build FluidProp object with the libraryName and substanceName info
	FluidProp.SetFluid(libraryName.substr(libraryName.find(".")+1), nComp, Comp, Conc, &ErrorMsg);
	if (isError(ErrorMsg))  // An error occurred
	{
		// Build error message and pass it to the Modelica environment
		char error[300];
		sprintf(error, "FluidProp error: %s\n", ErrorMsg.c_str());
		errorMessage(error);
	}

	// Set SI units and if requested molar specific units, Molar Mass always in g/mol!
	if (libraryName.find("Molar") != string::npos)
		FluidProp.SetUnits("SI", "PerMole", "Mmol", "g/mol", &ErrorMsg);
	else
		FluidProp.SetUnits("SI", "PerMass", "Mmol", "g/mol", &ErrorMsg);
	if (isError(ErrorMsg))  // An error occurred
	{
		// Build error message and pass it to the Modelica environment
		char error[300];
		sprintf(error, "FluidProp error: %s\n", ErrorMsg.c_str());
		errorMessage(error);
	}

	// Set fluid constants
	setFluidConstants();
}

FluidPropSolver::~FluidPropSolver(){
}

void FluidPropSolver::setFluidConstants(){
  string ErrorMsg;

  _fluidConstants.MM = FluidProp.Mmol(&ErrorMsg);
  if (isError(ErrorMsg))  // An error occurred
	{
	// Build error message and pass it to the Modelica environment
	char error[300];
	sprintf(error, "FluidProp error in FluidPropSolver::setFluidConstants: can't compute molar mass\n %s\n", ErrorMsg.c_str());
	errorMessage(error);
	}

  _fluidConstants.Tc = FluidProp.Tcrit(&ErrorMsg);
  if (isError(ErrorMsg))  // An error occurred
	{
	// Build error message and pass it to the Modelica environment
	char error[300];
	sprintf(error, "FluidProp error in FluidPropSolver::setFluidConstants: can't compute critical temperature\n %s\n", ErrorMsg.c_str());
	errorMessage(error);
	}

  _fluidConstants.pc = FluidProp.Pcrit(&ErrorMsg);
  if (isError(ErrorMsg))  // An error occurred
	{
	// Build error message and pass it to the Modelica environment
	char error[300];
	sprintf(error, "FluidProp error in FluidPropSolver::setFluidConstants: can't compute critical pressure\n %s\n", ErrorMsg.c_str());
	errorMessage(error);
	}

  _fluidConstants.dc = FluidProp.Density("PT", _fluidConstants.pc, _fluidConstants.Tc, &ErrorMsg);
  if (isError(ErrorMsg))  // An error occurred
	{
	// Build error message and pass it to the Modelica environment
	char error[300];
	sprintf(error, "FluidProp error in FluidPropSolver::setFluidConstants: can't compute critical density\n %s\n", ErrorMsg.c_str());
	errorMessage(error);
	}
}

void FluidPropSolver::setSat_p(double &p, TwoPhaseMediumProperties *const properties, const int nComp){
	string ErrorMsg;
	// FluidProp variables (in SI units)
    double P_, T_, v_, d_, h_, s_, u_, q_, x_[MAX_NO_COMPONENTS], y_[MAX_NO_COMPONENTS], 
		   cv_, cp_, c_, alpha_, beta_, chi_, fi_, ksi_,
		   psi_, zeta_, theta_, kappa_, gamma_, eta_, lambda_,
		   d_liq_, d_vap_, h_liq_, h_vap_, T_sat_, dd_liq_dP_, dd_vap_dP_, dh_liq_dP_, 
		   dh_vap_dP_, dT_sat_dP_;

	// Compute all FluidProp variables at pressure p and steam quality 0
	FluidProp.AllPropsSat("Pq", p , 0.0, P_, T_, v_, d_, h_, s_, u_, q_, x_, y_, cv_, cp_, c_,
		                  alpha_, beta_, chi_, fi_, ksi_, psi_, zeta_, theta_, kappa_, gamma_, eta_, lambda_,  
	    			      d_liq_, d_vap_, h_liq_, h_vap_, T_sat_, dd_liq_dP_, dd_vap_dP_, dh_liq_dP_, 
						  dh_vap_dP_, dT_sat_dP_, &ErrorMsg);
	if (isError(ErrorMsg)) {  // An error occurred
		// Build error message and pass it to the Modelica environment
		char error[300];
		sprintf(error, "FluidProp error in FluidPropSolver::setSat_p(%f)\n %s\n", p, ErrorMsg.c_str());
		errorMessage(error);
	}

    // Fill in the TwoPhaseMedium variables (in SI units)
	
	//equilibrium concentrations
	for (int i = 0; i < nComp; i++) {
		properties->x[i] = x_[i];
		properties->y[i] = y_[i];
	}

    properties->ps = p;             // saturation pressure
	properties->Ts = T_sat_;		// saturation temperature
	properties->T  = T_;			// bubble point temperature

	properties->dl = d_liq_;	// bubble density
	properties->dv = d_vap_;	// dew density
	properties->hl = h_liq_;	// bubble specific enthalpy
	properties->hv = h_vap_;	// dew specific enthalpy

	properties->d_Ts_dp = dT_sat_dP_;  // derivative of Ts by pressure
	properties->d_dl_dp = dd_liq_dP_; // derivative of dls by pressure
	properties->d_dv_dp = dd_vap_dP_; // derivative of dvs by pressure
    properties->d_hl_dp = dh_liq_dP_; // derivative of hls by pressure
	properties->d_hv_dp = dh_vap_dP_; // derivative of hvs by pressure
}

void FluidPropSolver::setSat_T(double &T, TwoPhaseMediumProperties *const properties, const int nComp){
	string ErrorMsg;
	// FluidProp variables (in SI units)
    double P_, T_, v_, d_, h_, s_, u_, q_, x_[MAX_NO_COMPONENTS], y_[MAX_NO_COMPONENTS], 
		   cv_, cp_, c_, alpha_, beta_, chi_, fi_, ksi_,
		   psi_, zeta_ , theta_, kappa_, gamma_, eta_, lambda_,
		   d_liq_, d_vap_, h_liq_, h_vap_, T_sat_, dd_liq_dP_, dd_vap_dP_, dh_liq_dP_, 
		   dh_vap_dP_, dT_sat_dP_;

	// Compute all FluidProp variables at temperature T and steam quality 0
	FluidProp.AllPropsSat("Tq", T , 0.0, P_, T_, v_, d_, h_, s_, u_, q_, x_, y_, cv_, cp_, c_,
		                  alpha_, beta_, chi_, fi_, ksi_, psi_, zeta_, theta_, kappa_, gamma_, eta_, lambda_,  
	    			      d_liq_, d_vap_, h_liq_, h_vap_, T_sat_, dd_liq_dP_, dd_vap_dP_, dh_liq_dP_, 
						  dh_vap_dP_, dT_sat_dP_, &ErrorMsg);
	if (isError(ErrorMsg)) {  // An error occurred
		// Build error message and pass it to the Modelica environment
		char error[300];
		sprintf(error, "FluidProp error in FluidPropSolver::setSat_T(%f)\n %s\n", T, ErrorMsg.c_str());
		errorMessage(error);
	}

    // Fill in the TwoPhaseMedium variables (in SI units)

	//equilibrium concentrations
	for (int i = 0; i < nComp; i++) {
		properties->x[i] = x_[i];
		properties->y[i] = y_[i];
	}

    properties->ps = P_;        // saturation pressure
	properties->Ts = T;			// saturation temperature

	properties->dl = d_liq_;	// bubble density
	properties->dv = d_vap_;	// dew density
	properties->hl = h_liq_;	// bubble specific enthalpy
	properties->hv = h_vap_;	// dew specific enthalpy

	properties->d_Ts_dp = dT_sat_dP_;  // derivative of Ts by pressure
	properties->d_dl_dp = dd_liq_dP_; // derivative of dls by pressure
	properties->d_dv_dp = dd_vap_dP_; // derivative of dvs by pressure
    properties->d_hl_dp = dh_liq_dP_; // derivative of hls by pressure
	properties->d_hv_dp = dh_vap_dP_; // derivative of hvs by pressure
}

void FluidPropSolver::setSat_p_state(TwoPhaseMediumProperties *const properties){
  // Do nothing: the saturation properties have already been computed by
  // setState and stored in the properties struct
};

// Computes the properties of the state vector *and* the saturation properties at the medium pressure
// for later use by setState_p_state
// Note: the phase input is currently not supported
void FluidPropSolver::setState_ph(double &p, double &h, int &phase, TwoPhaseMediumProperties *const properties, const int nComp){
	string ErrorMsg;
	// FluidProp variables (in SI units)
    double P_, T_, v_, d_, h_, s_, u_, q_, x_[MAX_NO_COMPONENTS], y_[MAX_NO_COMPONENTS], 
		   cv_, cp_, c_, alpha_, beta_, chi_, fi_, ksi_,
		   psi_, zeta_ , theta_, kappa_, gamma_, eta_, lambda_,
		   d_liq_, d_vap_, h_liq_, h_vap_, T_sat_, dd_liq_dP_, dd_vap_dP_, dh_liq_dP_, 
		   dh_vap_dP_, dT_sat_dP_;

	// Compute all FluidProp variables
	if (p > _fluidConstants.pc)
	  FluidProp.AllProps("Ph", p , h, P_, T_, v_, d_, h_, s_, u_, q_, x_, y_, cv_, cp_, c_,
	  	                 alpha_, beta_, chi_, fi_, ksi_, psi_, 
						 zeta_, theta_, kappa_, gamma_, eta_, lambda_, &ErrorMsg);
	else
	  FluidProp.AllPropsSat("Ph", p , h, P_, T_, v_, d_, h_, s_, u_, q_, x_, y_, cv_, cp_, c_,
	  	                    alpha_, beta_, chi_, fi_, ksi_, psi_, zeta_, theta_, kappa_, gamma_, eta_, lambda_,  
	    			        d_liq_, d_vap_, h_liq_, h_vap_, T_sat_, dd_liq_dP_, dd_vap_dP_, dh_liq_dP_, 
						    dh_vap_dP_, dT_sat_dP_, &ErrorMsg);

	if (isError(ErrorMsg)) {  // An error occurred
		// Build error message and pass it to the Modelica environment
		char error[300];
		sprintf(error, "FluidProp error in FluidPropSolver::setState_ph(%f, %f)\n %s\n", p, h, ErrorMsg.c_str());
		errorMessage(error);
	}

    // set the phase property
	if (phase == 0) {
		properties->phase = (properties->h > properties->hl && 
			                 properties->h < properties->hv &&
							 properties->p < _fluidConstants.pc)  ?  2 : 1;
	} else
		properties->phase = phase;

    // Fill in the TwoPhaseMedium variables (in SI units)

	//equilibrium concentrations
	for (int i = 0; i < nComp; i++) {
		properties->x[i] = x_[i];
		properties->y[i] = y_[i];
	}

	properties->beta = theta_;			// isothermal expansion coefficient
	properties->cp = cp_; 		        // specific heat capacity cp
	properties->cv = cv_;		        // specific heat capacity cv
	properties->d = d_;				    // density
	properties->dd_dp_h = psi_;         // derivative of density by pressure at constant h
	properties->dd_dh_p = ksi_;         // derivative of density by enthalpy at constant p
	properties->h = h;					// specific enthalpy
	properties->u = u_;					// specific internal energy
	properties->kappa = kappa_;			// compressibility
	properties->p = p;					// pressure
	properties->s = s_;     		    // specific entropy
	properties->T = T_;         		// temperature
    properties->a = c_;					// speed of sound
    if (properties->phase == 1)
	{
		properties->dT_dp_h = v_*(T_*theta_ - 1)/cp_;	// dT/dp|h
		properties->dT_dh_p = 1/cp_;					// dT/dh|p
	}
	else
	{
		properties->dT_dp_h = dT_sat_dP_;				// dT/dp|h
		properties->dT_dh_p = 0;						// dT/dh|p
	}

	properties->ps = p;				// saturation pressure
	properties->Ts = T_sat_;		// saturation temperature

	properties->dl = d_liq_;	// bubble density
	properties->dv = d_vap_;	// dew density
	properties->hl = h_liq_;	// bubble specific enthalpy
	properties->hv = h_vap_;	// dew specific enthalpy

	properties->d_Ts_dp = dT_sat_dP_;  // derivative of Ts by pressure
	properties->d_dl_dp = dd_liq_dP_; // derivative of dls by pressure
	properties->d_dv_dp = dd_vap_dP_; // derivative of dvs by pressure
    properties->d_hl_dp = dh_liq_dP_; // derivative of hls by pressure
	properties->d_hv_dp = dh_vap_dP_; // derivative of hvs by pressure

	properties->eta = eta_;	    // dynamic viscosity
	properties->lambda = lambda_;	// thermal conductivity

	properties->du_dp = 1/beta_;  // derivative of u by pressure
	properties->du_dh = 1/beta_ * 1/psi_ * ksi_ ; // derivative of u by enthalpy
}

// Computes the properties of the state vector *and* the saturation properties at the medium pressure
// for later use by setState_p_state
// Note: the phase input is currently not supported
void FluidPropSolver::setState_pT(double &p, double &T, TwoPhaseMediumProperties *const properties, const int nComp){
	string ErrorMsg;

	// FluidProp variables (in SI units)
    double P_, T_, v_, d_, h_, s_, u_, q_, x_[MAX_NO_COMPONENTS], y_[MAX_NO_COMPONENTS], 
		   cv_, cp_, c_, alpha_, beta_, chi_, fi_, ksi_,
		   psi_, zeta_ , theta_, kappa_, gamma_, eta_, lambda_,
		   d_liq_, d_vap_, h_liq_, h_vap_, T_sat_, dd_liq_dP_, dd_vap_dP_, dh_liq_dP_, 
		   dh_vap_dP_, dT_sat_dP_;

	// Compute all FluidProp variables
	if (p > _fluidConstants.pc)
	  FluidProp.AllProps("PT", p , T, P_, T_, v_, d_, h_, s_, u_, q_, x_, y_, cv_, cp_, c_,
		                 alpha_, beta_, chi_, fi_, ksi_, psi_,
						 zeta_, theta_, kappa_, gamma_, eta_, lambda_, &ErrorMsg);
	else
	  FluidProp.AllPropsSat("PT", p , T, P_, T_, v_, d_, h_, s_, u_, q_, x_, y_, cv_, cp_, c_,
		                    alpha_, beta_, chi_, fi_, ksi_, psi_, zeta_, theta_, kappa_, gamma_, eta_, lambda_,  
	    			        d_liq_, d_vap_, h_liq_, h_vap_, T_sat_, dd_liq_dP_, dd_vap_dP_, dh_liq_dP_, 
						    dh_vap_dP_, dT_sat_dP_, &ErrorMsg);
	if (isError(ErrorMsg)) {  // An error occurred
		// Build error message and pass it to the Modelica environment
		char error[300];
		sprintf(error, "FluidProp error in FluidPropSolver::setState_pT(%f, %f)\n %s\n", p, T, ErrorMsg.c_str());
		errorMessage(error);
	}

    // set the phase property
	properties->phase = 1;  // Always one-phase with pT inputs

	// Fill in the TwoPhaseMedium variables (in SI units)

	//equilibrium concentrations
	for (int i = 0; i < nComp; i++) {
		properties->x[i] = x_[i];
		properties->y[i] = y_[i];
	}
	properties->beta = theta_;			// isothermal expansion coefficient
	properties->cp = cp_; 		        // specific heat capacity cp
	properties->cv = cv_;		        // specific heat capacity cv
	properties->d = d_;				    // density
	properties->dd_dp_h = psi_;         // derivative of density by pressure at constant h
	properties->dd_dh_p = ksi_;         // derivative of density by enthalpy at constant p
	properties->h = h_;					// specific enthalpy
	properties->u = u_;					// specific internal energy
	properties->kappa = kappa_;			// compressibility
	properties->beta = theta_;			// thermal expansion coefficient
	properties->p = p;					// pressure
	properties->s = s_;     		    // specific entropy
	properties->T = T_;         		// temperature
    properties->a = c_;					// speed of sound

	properties->dT_dp_h = v_*(T_*theta_ - 1)/cp_;	// dT/dp|h
	properties->dT_dh_p = 1/cp_;					// dT/dh|p

	properties->ps = p;				// saturation pressure
	properties->Ts = T_sat_;		// saturation temperature

	properties->dl = d_liq_;	// bubble density
	properties->dv = d_vap_;	// dew density
	properties->hl = h_liq_;	// bubble specific enthalpy
	properties->hv = h_vap_;	// dew specific enthalpy

	properties->d_Ts_dp = dT_sat_dP_;  // derivative of Ts by pressure
	properties->d_dl_dp = dd_liq_dP_; // derivative of dls by pressure
	properties->d_dv_dp = dd_vap_dP_; // derivative of dvs by pressure
    properties->d_hl_dp = dh_liq_dP_; // derivative of hls by pressure
	properties->d_hv_dp = dh_vap_dP_; // derivative of hvs by pressure

	properties->eta = eta_;	    // dynamic viscosity
	properties->lambda = lambda_;	// thermal conductivity

	properties->du_dp = 1/beta_;  // derivative of u by pressure
	properties->du_dh = 1/beta_ * 1/psi_ * ksi_ ; // derivative of u by enthalpy
}

// Computes the properties of the state vector *and* the saturation properties at the medium pressure
// for later use by setState_p_state
// Note: the phase input is currently not supported
void FluidPropSolver::setState_dT(double &d, double &T, int &phase, TwoPhaseMediumProperties *const properties, const int nComp){
	string ErrorMsg;
	// FluidProp variables (in SI units)
    double P_, T_, v_, d_, h_, s_, u_, q_, x_[MAX_NO_COMPONENTS], y_[MAX_NO_COMPONENTS], 
		   cv_, cp_, c_, alpha_, beta_, chi_, fi_, ksi_,
		   psi_, zeta_ , theta_, kappa_, gamma_, eta_, lambda_,
		   d_liq_, d_vap_, h_liq_, h_vap_, T_sat_, dd_liq_dP_, dd_vap_dP_, dh_liq_dP_, 
		   dh_vap_dP_, dT_sat_dP_;

	// Compute all FluidProp variables
	if (T > _fluidConstants.Tc)
	  FluidProp.AllProps("Td", T , d, P_, T_, v_, d_, h_, s_, u_, q_, x_, y_, cv_, cp_, c_,
		                 alpha_, beta_, chi_, fi_, ksi_, psi_,
						 zeta_, theta_, kappa_, gamma_, eta_, lambda_, &ErrorMsg);
	else
	  FluidProp.AllPropsSat("Td", T , d, P_, T_, v_, d_, h_, s_, u_, q_, x_, y_, cv_, cp_, c_,
		                    alpha_, beta_, chi_, fi_, ksi_, psi_, zeta_, theta_, kappa_, gamma_, eta_, lambda_,  
	    			        d_liq_, d_vap_, h_liq_, h_vap_, T_sat_, dd_liq_dP_, dd_vap_dP_, dh_liq_dP_, 
						    dh_vap_dP_, dT_sat_dP_, &ErrorMsg);
	if (isError(ErrorMsg)) {  // An error occurred
		// Build error message and pass it to the Modelica environment
		char error[300];
		sprintf(error, "FluidProp error in FluidPropSolver::setState_dT(%f, %f)\n %s\n", d, T, ErrorMsg.c_str());
		errorMessage(error);
	}

    // set the phase output
	if (phase == 0) {
		properties->phase = (properties->d < properties->dl && 
			                 properties->d > properties->dv &&
							 properties->T < _fluidConstants.Tc)  ?  2 : 1;
	} else
		properties->phase = phase;

	// Fill in the TwoPhaseMedium variables (in SI units)
	
	//equilibrium concentrations
	for (int i = 0; i < nComp; i++) {
		properties->x[i] = x_[i];
		properties->y[i] = y_[i];
	}

	properties->beta = theta_;			// isothermal expansion coefficient
	properties->cp = cp_; 		        // specific heat capacity cp
	properties->cv = cv_;		        // specific heat capacity cv
	properties->d = d;				    // density
	properties->dd_dp_h = psi_;         // derivative of density by pressure at constant h
	properties->dd_dh_p = ksi_;         // derivative of density by enthalpy at constant p
	properties->h = h_;					// specific enthalpy
	properties->u = u_;					// specific internal energy
	properties->kappa = kappa_;			// compressibility
	properties->p = P_;					// pressure
	properties->s = s_;     		    // specific entropy
	properties->T = T;         			// temperature
    properties->a = c_;					// speed of sound

    if (properties->phase == 1)
	{
		properties->dT_dp_h = v_*(T_*theta_ - 1)/cp_;	// dT/dp|h
		properties->dT_dh_p = 1/cp_;					// dT/dh|p
	}
	else
	{
		properties->dT_dp_h = dT_sat_dP_;				// dT/dp|h
		properties->dT_dh_p = 0;						// dT/dh|p
	}

	properties->ps = P_;			// saturation pressure
	properties->Ts = T_sat_;		// saturation temperature

	properties->dl = d_liq_;	// bubble density
	properties->dv = d_vap_;	// dew density
	properties->hl = h_liq_;	// bubble specific enthalpy
	properties->hv = h_vap_;	// dew specific enthalpy

	properties->d_Ts_dp = dT_sat_dP_;  // derivative of Ts by pressure
	properties->d_dl_dp = dd_liq_dP_; // derivative of dls by pressure
	properties->d_dv_dp = dd_vap_dP_; // derivative of dvs by pressure
    properties->d_hl_dp = dh_liq_dP_; // derivative of hls by pressure
	properties->d_hv_dp = dh_vap_dP_; // derivative of hvs by pressure

	properties->eta = eta_;	    // dynamic viscosity
	properties->lambda = lambda_;	// thermal conductivity

	properties->du_dp = 1/beta_;  // derivative of u by pressure
	properties->du_dh = 1/beta_ * 1/psi_ * ksi_ ; // derivative of u by enthalpy
}

// Computes the properties of the state vector *and* the saturation properties at the medium pressure
// for later use by setState_p_state
// Note: the phase input is currently not supported
void FluidPropSolver::setState_ps(double &p, double &s, int &phase, TwoPhaseMediumProperties *const properties, const int nComp){
	string ErrorMsg;
	// FluidProp variables (in SI units)
    double P_, T_, v_, d_, h_, s_, u_, q_, x_[MAX_NO_COMPONENTS], y_[MAX_NO_COMPONENTS], 
		   cv_, cp_, c_, alpha_, beta_, chi_, fi_, ksi_,
		   psi_, zeta_ , theta_, kappa_, gamma_, eta_, lambda_,
		   d_liq_, d_vap_, h_liq_, h_vap_, T_sat_, dd_liq_dP_, dd_vap_dP_, dh_liq_dP_, 
		   dh_vap_dP_, dT_sat_dP_;

	// Compute all FluidProp variables
    if (p > _fluidConstants.pc)
	  FluidProp.AllProps("Ps", p , s, P_, T_, v_, d_, h_, s_, u_, q_, x_, y_, cv_, cp_, c_,
		                 alpha_, beta_, chi_, fi_, ksi_, psi_,
						 zeta_, theta_, kappa_, gamma_, eta_, lambda_, &ErrorMsg);
	else
	  FluidProp.AllPropsSat("Ps", p , s, P_, T_, v_, d_, h_, s_, u_, q_, x_, y_, cv_, cp_, c_,
		                    alpha_, beta_, chi_, fi_, ksi_, psi_, zeta_, theta_, kappa_, gamma_, eta_, lambda_,  
	    			        d_liq_, d_vap_, h_liq_, h_vap_, T_sat_, dd_liq_dP_, dd_vap_dP_, dh_liq_dP_, 
						    dh_vap_dP_, dT_sat_dP_, &ErrorMsg);
	if (isError(ErrorMsg)) {  
		// An error occurred
		// Build error message and pass it to the Modelica environment
		char error[300];
		sprintf(error, "FluidProp error in FluidPropSolver::setState_ps(%f, %f)\n %s\n", p, s, ErrorMsg.c_str());
		errorMessage(error);
	}

    // set the phase output
	if (phase == 0) {
		properties->phase = (properties->d < properties->dl && 
			                 properties->d > properties->dv &&
							 properties->T < _fluidConstants.Tc)  ?  2 : 1;
	} else
		properties->phase = phase;

	// Fill in the TwoPhaseMedium variables (in SI units)

	//equilibrium concentrations
	for (int i = 0; i < nComp; i++) {
		properties->x[i] = x_[i];
		properties->y[i] = y_[i];
	}

	properties->beta = theta_;			// isothermal expansion coefficient
	properties->cp = cp_; 		        // specific heat capacity cp
	properties->cv = cv_;		        // specific heat capacity cv
	properties->d = d_;				    // density
	properties->dd_dp_h = psi_;         // derivative of density by pressure at constant h
	properties->dd_dh_p = ksi_;         // derivative of density by enthalpy at constant p
	properties->h = h_;					// specific enthalpy
	properties->u = u_;					// specific internal energy
	properties->kappa = kappa_;			// compressibility
	properties->p = p;					// pressure
	properties->s = s;     			    // specific entropy
	properties->T = T_;         		// temperature
    properties->a = c_;					// speed of sound

    if (properties->phase == 1)
	{
		properties->dT_dp_h = v_*(T_*theta_ - 1)/cp_;	// dT/dp|h
		properties->dT_dh_p = 1/cp_;					// dT/dh|p
	}
	else
	{
		properties->dT_dp_h = dT_sat_dP_;				// dT/dp|h
		properties->dT_dh_p = 0;						// dT/dh|p
	}

	properties->ps = p;				// saturation pressure
	properties->Ts = T_sat_;		// saturation temperature

	properties->dl = d_liq_;	// bubble density
	properties->dv = d_vap_;	// dew density
	properties->hl = h_liq_;	// bubble specific enthalpy
	properties->hv = h_vap_;	// dew specific enthalpy

	properties->d_Ts_dp = dT_sat_dP_;  // derivative of Ts by pressure
	properties->d_dl_dp = dd_liq_dP_; // derivative of dls by pressure
	properties->d_dv_dp = dd_vap_dP_; // derivative of dvs by pressure
    properties->d_hl_dp = dh_liq_dP_; // derivative of hls by pressure
	properties->d_hv_dp = dh_vap_dP_; // derivative of hvs by pressure

	properties->eta = eta_;	    // dynamic viscosity
	properties->lambda = lambda_;	// thermal conductivity
	
	properties->du_dp = 1/beta_;  // derivative of u by pressure
	properties->du_dh = 1/beta_ * 1/psi_ * ksi_ ; // derivative of u by enthalpy
}

void FluidPropSolver::setBubbleState(int phase, TwoPhaseMediumProperties *const properties,
		                             TwoPhaseMediumProperties *const bubbleProperties){
	// Set the bubble state property record based on the original medium 
	// saturation state
    // Change hl a little to guarantee the correct phase, since the phase
    // input is currently not supported
    double hl;
	if (phase == 1)
		// liquid
		hl = properties->hl*(1-1e-6);
	else
		// two-phase
		hl = properties->hl*(1+1e-6);
    // Call setState function
	setState_ph(properties->ps, hl, phase, bubbleProperties);
}

void FluidPropSolver::setDewState(int phase, TwoPhaseMediumProperties *const properties,
		                          TwoPhaseMediumProperties *const dewProperties){
	// Set the dew state property record based on the original medium 
	// saturation state
    // Change hv a little to guarantee the correct phase, since the phase
    // input is currently not supported
    double hv;
	if (phase == 1)
		// vapour
		hv = properties->hv*(1+1e-6);
	else
		// two-phase
		hv = properties->hv*(1-1e-6);
    // Call setState function
	setState_ph(properties->ps, hv, phase, dewProperties);
}

//! Compute isentropic enthalpy
/*!
  This function returns the enthalpy at pressure p after an isentropic
  transformation from the state specified by the properties input

  @param p New pressure
  @param properties Two phase medium property record corresponding to current state
*/
double FluidPropSolver::isentropicEnthalpy(double &p, TwoPhaseMediumProperties *const properties){
	string ErrorMsg;
    double h;

	h = FluidProp.Enthalpy("Ps", p , properties->s, &ErrorMsg);
	if (isError(ErrorMsg)) {  // An error occurred
		// Build error message and pass it to the Modelica environment
		char error[300];
		sprintf(error, "FluidProp error in FluidPropSolver::isentropicEnthalpy(%f, %f)\n %s\n", p, properties->s, ErrorMsg.c_str());
		errorMessage(error);
	}
	return h;
}

bool FluidPropSolver::isError(string ErrorMsg)
{
  if(ErrorMsg == "No errors")
	  return false;
  else
      return true;
}


#endif // FLUIDPROP == 1
