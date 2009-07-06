#ifndef TWOPHASEMIXTURE_H_
#define TWOPHASEMIXTURE_H_

#include "include.h"
#include "basetwophasemedium.h"

//! Two phase mixture
/*!
  This class is the object embedding the mixture property
  computations at a given point of the plant. 
  
  Andreas Joos, Jun 2009
  Hamburg University of Technology
*/
class TwoPhaseMixture : public BaseTwoPhaseMedium{
public:
	TwoPhaseMixture(const string &mediumName, const string &libraryName, 
				   const string &substanceName, BaseSolver *const solver,
				   const int &uniqueID);
	~TwoPhaseMixture();

	double *x() const;
	double *y() const;
};

#endif // TWOPHASEMIXTURE_H_
