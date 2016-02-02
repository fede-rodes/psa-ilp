/**
 * @file   Candidate.h
 * @brief  Partial (final) candidate solution being analyzed.
 * @author Federico Rodes
 */

#ifndef CANDIDATE_H_
#define CANDIDATE_H_

#include <iostream> // std::cout, std::cerr, std::endl
#include <cstdlib>  // std::exit()

class Candidate
{

private:

	// FIELDS:
	/**
	 * number of variables in the original ILP problem.
	 */
	const unsigned int n_var;

	/**
	 * array of integer values. Where 'array[j] = -1' means the variable x_j has not yet
	 * been fixed to a specific value; 'array[j] = a, with a >= 0' means x_j is set to 'a'.
	 */
	int* array;

public:

	// CONSTRUCTOR AND DESTRUCTOR:
	/**
	 * Creates an 'empty' candidate solution where 'array[j] = -1' for all x_j in the original
	 * ILP problem. In other words, no variables has yet been fixed.
	 */
	Candidate( const unsigned int _n_var );
	~Candidate();

	// ASSIGNMENT OPERATOR:
	Candidate( const Candidate& other );
	void operator =( const Candidate& other );


	// METHODS:
	/**
	 * Restores the 'array' to its initial configuration (i.e., 'array[j] = -1'
	 * for all variable x_j in the original ILP problem).
	 */
	void clear();

	/**
	 * Displays the content of the 'array'.
	 */
	void display() const;

	// SETTERS:
	/**
	 * Sets the variable indexed '_index' to '_value'.
	 */
	void set_value( const unsigned int _index, const int _value );

	// GETTERS:
	/**
	 * Returns the value assigned to the variable indexed '_index'.
	 */
	double get_value( const unsigned int _index ) const;

};

#endif /* CANDIDATE_H_ */
