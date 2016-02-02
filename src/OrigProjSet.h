/*
 * OrigProjSet.h
 *
 *  Created on: 11/03/2014
 *      Author: Federico Rodes
 */

#ifndef ORIGPROJSET_H_
#define ORIGPROJSET_H_

#include <algorithm> // std::max()

#include "ProjSet.h"

class OrigProjSet: public ProjSet
{

public:

	// CONSTRUCTOR AND DESTRUCTOR:
	/**
	 * Creates a projection (P_up_0, P_up_1, P_low_0, P_low_1) for each of the active variables in the given ILP problem '_ilp'.
	 * @param _ilp // current ILP problem.
	 * @param _av // active variables set.
	 */
	OrigProjSet( const ILP& _ilp, const AV& _av );
	//~OrigProjSet();

	// METHODS:
	/**
	 * Don't worry about this method!
	 * @param _LB // lower bound.
	 * @return number of variable that can be removed form the original ILP problem.
	 */
	int reduce( const double _LB );

	// GETTERS:
	/**
	 * Based on the information given by the projections, this method computes an (integer) upper bound
	 * for the optimal objective value of the original ILP problem.
	 * @return largest level to be scanned.
	 */
	double get_UB() const;

};

#endif /* ORIGPROJSET_H_ */
