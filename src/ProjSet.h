/**
 * @file   ProjSet.h
 * @brief  projection set associated to the current ILP problem.
 * @author Federico Rodes
 */

#ifndef PROJSET_H_
#define PROJSET_H_

#include <iostream> // std::cout, std::cerr, std::endl
#include <cstdlib>  // std::exit()
#include <cmath>    // std::floor(), std::ceil()
//#include "../qsopt/qsopt.h"
#include "../include/glpk.h"

#include "AV.h"
#include "Candidate.h"
#include "ILP.h"
#include "FixedVars.h"
#include "Constants.h"

class ProjSet
{

protected: // so that OrigProjSet can access to these fields.

	// FIELDS:
	/**
	 * number of variables in the original ILP problem.
	 */
	const unsigned int n_var;

	/**
	 * upper projection in zero.
	 */
	double* P_up_0;

	/**
	 * upper projection in one.
	 */
	double* P_up_1;

	/**
	 * lower projection in zero.
	 */
	double* P_low_0;

	/**
	 * lower projection in one.
	 */
	double* P_low_1;

	// METHODS:
	/**
	 * Fixes the rounding errors that might be produced when applying the Simplex method to compute the projection set
	 * associated to the given ILP problem.
	 * @param _av // set of active variables.
	 */
	void round_errors( const AV& _av );

	//unsigned int calc_proj_qsopt( const ILP& _ilp, const AV& _av );
	unsigned int calc_proj_glpk( const ILP& _ilp, const AV& _av );

public:

	// CONSTRUCTOR AND DESTRUCTOR:
	/**
	 * Creates a projection (P_up_0, P_up_1, P_low_0, P_low_1) for each of the active variables in the given ILP problem '_ilp'.
	 * @param _ilp // current ILP problem.
	 * @param _av // active variables set.
	 */
	ProjSet( const ILP& _ilp, const AV& _av );
	~ProjSet();

	// COPY CONSTRUCTOR AND ASSIGNMENT OPERATOR:
	/**
	 * Copies the information contained in the 'other' object to the current one.
	 * @param other // projection set.
	 */
	ProjSet( const ProjSet& other );
	void operator =( const ProjSet& other );

	// METHODS:
	/**
	 * Scans the projection set restricted to the given level. The variables that can take on a single value
	 * are stored in the '_fixed_vars' object, removed from the active variables set '_av', and fixed in
	 * the partial candidate '_x' to the value provided by the projections.
	 * @param _level // level being scanned.
	 * @param _x // current partial candidate solution.
	 * @param _av // set of active variables.
	 * @param _fixed_vars // set of variables that were fixed in the last iteration of the Main algorithm.
	 * @return PSA_FEASIBLE or PSA_INFEASIBLE.
	 */
	unsigned int inspect( const double _level, Candidate& _x, AV& _av, FixedVars& _fixed_vars );

	/**
	 * Utilizes an external LP solver (QSopt) to calculate the projection set associated to the active variables '_av' for the given ILP problem '_ilp'.
	 * @param _ilp // current ILP problem.
	 * @param _av // set of active variables.
	 * @return PSA_FEASIBLE or PSA_INFEASIBLE.
	 */
	unsigned int calc_proj( const ILP& _ilp, const AV& _av, const int _lp_solver );

	/**
	 * Displays the projection set associated to the active variables of the current ILP problem.
	 */
	void display( const AV& _av ) const;

};

#endif /* PROJSET_H_ */
