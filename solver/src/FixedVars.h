/**
 * @file   FixedVars.h
 * @brief  FixedVars keeps track of the set of variables that were fixed during the last iteration of the Main algorithm.
 * @author Federico Rodes
 */

#ifndef FIXEDVARS_H_
#define FIXEDVARS_H_

#include <iostream> // std::cout, std::cerr, std::endl
#include <cstdlib>  // std::exit()

class FixedVars
{
	/**
	 * Disable copy constructor given by default.
	 */
	FixedVars( const FixedVars& other ) = delete;

private:

	// FIELDS:
	/**
	 * number of variables in the original ILP problem.
	 */
	const unsigned int n_var;

	/**
	 * array made up with the variables that were fixed during the last iteration of the Main algorithm.
	 */
	unsigned int* array;

	/**
	 * number of variables that make up the 'array'.
	 */
	unsigned int counter;

public:

	// CONSTRUCTOR AND DESTRUCTOR:
	/**
	 * Sets 'counter = 0', and initializes the 'array' with rubbish.
	 */
	FixedVars( const unsigned int _n_var );
	~FixedVars();

	// METHODS:
	/**
	 * Adds the variable indexed '_index' to the 'array'.
	 */
	void add( const unsigned int _index );

	/**
	 * Displays the set of variables that were fixed during the last iteration of the Main algorithm.
	 */
	void display() const;

	/**
	 * Restores the object fields to their initial configuration (i.e., 'counter = 0').
	 */
	void clear();

	/**
	 * Returns the number of fixed variables in the 'array'.
	 */
	unsigned int size() const;

	/**
	 * Returns the i-th variable that was fixed during the last iteration of the Main algorithm.
	 */
	unsigned int get_ith( const unsigned int _ith ) const;

};

#endif /* FIXEDVARS_H_ */
