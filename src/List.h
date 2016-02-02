/**
 * @file   List.h
 * @brief  list of partial candidate solutions to be analyzed.
 * @author Federico Rodes
 */

#ifndef LIST_H_
#define LIST_H_

#include <iostream> // std::cout, std::cerr, std::endl
#include <cstdlib>  // std::exit()

#include "Candidate.h"

class List
{
	/**
	 * Disable copy constructor given by default.
	 */
	List( const List& other ) = delete;

private:

	// FIELDS:
	/**
	 * number of variables in the original ILP problem.
	 */
	const unsigned int n_var;

	/**
	 * 1000 x n_var matrix. Each row of the matrix could contain a partial candidate solution to be analyzed.
	 */
	double** matrix;

	/**
	 * keep track of the number of partial candidate solutions contained in the list. It is easy to see that the 
	 * last partial candidate solution added to the list is stored in 'matrix' at the row labeled 'counter-1'.
	 */
	unsigned int counter;

public:

	// CONSTRUCTOR AND DESTRUCTOR:
	/**
	 * Generates an empty 'matrix' with 1000 rows and 'n_var' columns to stored the set of partial candidate solutions to be analyzed.
	 * Additionally, the constructor sets 'counter = 0'.
	 */
	List( const unsigned int _n_var );
	~List();

	// METHODS:
	/**
	 * Displays the set of partial candidate solutions contained in the list.
	 */
	void display() const;

	/**
	 * Adds a new partial candidate solution to the list of partial candidate solutions to be analyzed.
	 * More precisely, the new partial candidate is added to the 'matrix' at the row labeled 'counter'.
	 */
	void add( const Candidate& _x );

	/**
	 * Returns the last partial candidate solution added to the list/'matrix'.
	 */
	void extract( Candidate& _x );

	/**
	 * Returns the number of partial candidate solutions contained in the list/'matrix'.
	 */
	unsigned int size() const;

};

#endif /* LIST_H_ */
