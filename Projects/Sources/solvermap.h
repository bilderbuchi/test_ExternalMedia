#ifndef SOLVERMAP_H_
#define SOLVERMAP_H_

#include "include.h"

class BaseSolver;

//! Solver map
/*!
  This class manages the map of all solvers. A solver is a class that inherits
  from BaseSolver and that interfaces the external fluid property computation
  code. Only one instance is created for each external library.

  Francesco Casella, Christoph Richter, Sep 2006
  Copyright Politecnico di Milano and TU Braunschweig
  modified by Andreas Joos (Hamburg University of Technology), Jun 2009
*/
class SolverMap{
public:
	static BaseSolver *getSolver(const string &mediumName, const string &libraryName, const string &substanceName, const int nComp = 1, double *Conc = 0);
	static string solverKey(const string &libraryName, const string &substanceName, const int nComp = 1, double *Conc = 0);

protected:
	//! Map for all solver instances identified by the SolverKey
	static map<string, BaseSolver*> _solvers;
};

#endif // SOLVERMAP_H_
