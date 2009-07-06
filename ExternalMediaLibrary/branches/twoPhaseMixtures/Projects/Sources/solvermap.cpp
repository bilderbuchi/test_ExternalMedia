#include <sstream>
#include "solvermap.h"
#include "basesolver.h"
#include "mediummap.h"
#include "testsolver.h"

#if (FLUIDPROP == 1)
#include "fluidpropsolver.h"
#endif // FLUIDPROP == 1

//! Get a specific solver
/*!
  This function returns the solver for the specified library name, substance name
  and possibly medium name. It creates a new solver if the solver does not already
  exist. When implementing new solvers, one has to add the newly created solvers to
  this function. An error message is generated if the specific library is not supported
  by the interface library.
  @param mediumName Medium name
  @param libraryName Library name
  @param substanceName Substance name
  @param nComp Number of Components in the Mixtures
  @param Conc Vector of the Concentrations of each Component
*/
BaseSolver *SolverMap::getSolver(const string &mediumName, const string &libraryName, const string &substanceName, const int nComp, double *Conc){
	// Get solver key from library and substance name
	string solverKeyString(solverKey(libraryName, substanceName, nComp, Conc));
	// Check whether solver already exists
	if (_solvers.find(solverKeyString) != _solvers.end())
		return _solvers[solverKeyString];
	// Create new solver if it doesn't exist
	// CompilerTest solver
	if (libraryName.compare("TestMedium") == 0)
	  _solvers[solverKeyString] = new TestSolver(mediumName, libraryName, substanceName);
#if (FLUIDPROP == 1)
	else if (libraryName.find("FluidProp") == 0)
	{
		if (nComp == 1)
			_solvers[solverKeyString] = new FluidPropSolver(mediumName, libraryName, substanceName);
		else 
			_solvers[solverKeyString] = new FluidPropSolver(mediumName, libraryName, substanceName, nComp, Conc);
	}
#endif // FLUIDPROP == 1
	else {
	  // Generate error message
	  char error[100];
	  sprintf(error, "Error: libraryName = %s is not supported by any external solver\n", libraryName.c_str());
	  errorMessage(error);
	}
	// Create new medium object for function calls without specified unique ID
	MediumMap::addSolverMedium(solverKeyString, _solvers[solverKeyString]);
	// Return pointer to solver
	return _solvers[solverKeyString];  
};

//! Generate a unique solver key
/*!
  This function generates a unique solver key based on the library name and 
  substance name.
*/
string SolverMap::solverKey(const string &libraryName, const string &substanceName, const int nComp, double *Conc){
	// This function returns the solver key and may be changed by advanced users
	if (nComp !=1 && libraryName.find("FluidProp") != string::npos)
		{
		
		int loc, nb = 0;
		string substanceName_cpy = substanceName;
		string substances[MAX_NO_COMPONENTS];
		string name;

		while ((loc = substanceName_cpy.find("-")) != string::npos)
		{
			if (loc == 0) 
			{
				substanceName_cpy = substanceName_cpy.erase(0, loc+1);
				continue;
			}
			substances[nb] = substanceName_cpy.substr(0, loc);
			substanceName_cpy = substanceName_cpy.erase(0, loc+1);
			nb++;
		}
		substances[nb++] = substanceName_cpy;

		for (int i = 0; i < nb; i++) {
			std::ostringstream os;
			os.precision(12); //Sum of Conc[i] has to be 1, if precicision is to small, FluidProp may get an error!
			os << Conc[i];
			name += substances[i];
			name += "_";
			name += os.str();
			if (i != nb - 1)
				name += "/";
		}
		return libraryName + "." + name;
		}
	else
		return libraryName + "." + substanceName;
}

map<string, BaseSolver*> SolverMap::_solvers;
