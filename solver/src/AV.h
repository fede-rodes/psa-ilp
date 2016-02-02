/**
 * @file   AV.h
 * @brief  Set of 'active variables' (av). That is, the set of variables that has not yet been fixed to a specific value.
 * @author Federico Rodes
 */

#ifndef AV_H_
#define AV_H_

#include <iostream>  // std::cout, std::cerr, std::endl
#include <cstdlib>   // std::exit()
#include <algorithm> // std::swap()

#include "FixedVars.h"

class AV
{
	/**
	 * Disable copy constructor given by default.
	 */
	AV( const AV& other ) = delete;

private:

	// FIELDS:
	/**
	 * number of variables in the original ILP problem.
	 */
	const unsigned int n_var;

	/**
	 * two-row matrix used to keep track of the set of active variables in the current ILP problem.
	 */
	unsigned int** two_row_matrix;

	/**
	 * row being used in 'two_row_matrix' (used_row = 0 or 1).
	 */
	int used_row;

	/**
	 * non-used row in 'two_row_matrix' (non_used_row = 1 or 0).
	 */
	int non_used_row;

	/**
	 * keep track of the number of active variables in 'two_row_matrix[used_row]'.
	 */
	unsigned int counter;

public:

	// CONSTRUCTOR AND DESTRUCTOR:
	/**
	 * Sets 'n_var = number of variables in the original ILP problem', 'counter = n_var' (all variables in the original ILP are active when the active variables set is created),
	 * 'used_row = 0', 'non_used_row = 1', 'two_row_matrix[used_row][j] = j for all variables x_j in the original ILP problem',
	 * and 'two_row_matrix[non_used_row][j] = rubbish' for all variables x_j in the original ILP problem.
	 */
	AV( const unsigned int _n_var );
	~AV();

	// METHODS:
	/**
	 * Displays the set of active variables (variables in the original ILP problem that has not yet been fixed).
	 */
	void display() const;

	/**
	 * Restores all the object fields to their initial configuration. More precisely, the method sets 'counter = n_var', 'used_row = 0', 'non_used_row = 1',
	 * 'two_row_matrix[used_row][j] = j' for all variable x_j in the original ILP problem', and
	 * 'two_row_matrix[non_used_row][j] = rubbish' for all variable x_j in the original ILP problem'.
	 */
	void clear();

	/**
	 * Removes from the active variables set the set of variables that were fixed to a specific value during the last iteration of the Main algorithm.
	 * Basically, this method implements the 'av -= _fixed_vars' operation.
	 */
	void remove( const FixedVars& _fixed_vars );

	// GETTERS:
	/**
	 * Returns the number of active variables.
	 */
	unsigned int size() const;

	/**
	 * Returns the index of the i-th active variable.
	 */
	unsigned int get_ith( const unsigned int _ith ) const;

};

#endif /* AV_H_ */
