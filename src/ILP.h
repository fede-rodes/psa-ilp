/**
 * @file   ILP.h
 * @brief  this class keeps all the information about the ILP being solved.
 *
 *  We assume that the original ILP problem has the following structure:<br>
 *
 *  objective function:<br>
 *  max z = c[0] x[0] + ... + c[n_var] x[n_var] + 0<br>
 *
 *  constraints:<br>
 *  s.t. A[0][0] x[0] + ... + A[0][n_var] x[n_var] <= b[0]<br>
 *   				     .       .        .        .<br>
 *      				 .       .        .        .<br>
 *      				 .       .        .        .<br>
 *       A[n_const][0] x[0] + ... + A[n_const][n_var] x[n_var] <= b[n_const]<br>
 *
 *       x[i] >= 0 binary, c[i] > 0 integer, A[i][j] >= 0, b[j] >= 0  for all i,j.<br>
 *
 *
 * When the original ILP problem is reduced in size, we use the following notation:<br>
 *
 *  objective function:<br>
 *  max z = c[0] x[0] + ... + c[n_var] x[n_var] + h_red<br>
 *
 *  constraints:<br>
 *  s.t. A[0][0] x[0] + ... + A[0][n_var] x[n_var] <= b_red[0]<br>
 *  					    	 .       .        .        .<br>
 *  						     .       .        .        .<br>
 *  						     .       .        .        .<br>
 *       A[n_const][0] x[0] + ... + A[n_const][n_var] x[n_var] <= b_red[n_const]<br>
 *
 *       x[i] >= 0 binary, c[i] > 0 integer, A[i][j] >= 0, b[j] >= 0  for all i,j.<br>
 * @author Federico Rodes
 */

#ifndef ILP_H_
#define ILP_H_

#include <iostream> // std::cout, std::cerr, std::endl
#include <string>
#include <cstdlib>  // std::exit()

#include "Candidate.h"
#include "AV.h"
#include "FixedVars.h"
#include "Constants.h"

using std::string;

class ILP
{
	/**
	 * Disable copy constructor given by default.
	 */
	ILP( const ILP& other ) = delete;

private:

	// FIELDS:
	/**
	 * number of variables in the original ILP problem.
	 */
	const unsigned int n_var;

	/**
	 * number of constraints in the original ILP problem.
	 */
	const unsigned int n_const;

	/**
	 * coefficients of the objective function in the original ILP problem.
	 */
	const double* c; // const

	/**
	 * constant term ('shift') of the objective function.
	 */
	double h_red; // variable

	/**
	 * constraint matrix in the equation Ax <= b of the original ILP problem.
	 */
	double** A; // const

	/**
	 * right hand side value in the equation Ax <= b of the original ILP problem.
	 */
	double* b; // const

	/**
	 * right hand side value in the equation Ax <= b of the reduced ILP problem. ('b' may vary every time the problem is shrunk)
	 */
	double* b_red; // variable

public:

	// CONSTRUCTOR AND DESTRUCTOR:
	/**
	 * Sets all the fields of the ILP object from the information given by the 'parser'.
	 */
	//ILP( const string _path );
	ILP( const unsigned int _n_var, const unsigned int _n_const, const double* _c, double** _A, double* _b );
	~ILP();

	// METHODS:
	/**
	 * Restores the object fields 'h_red' and 'b_red' to their initial configuration (i.e., 'h_red = 0' and 'b_red = b').
	 */
	void clear();

	/**
	 * Displays all the information concerning the current ILP problem, such as: c, h_red , A, b_red, and active variables.
	 */
	void display( const AV& _av ) const;

	/**
	 * Calculates the feasibility value and the objective value of the given (final) candidate solution '_x'.
	 * In the case that the candidate solution is feasible, the method additionally checks whether the candidate improves
	 * the best LB known so far, or whether it is an optimal solution to the original ILP problem.
	 * @return PSA_INFEASIBLE, PSA_FEASIBLE, PSA_IMPROVE, PSA_OPTIMAL.
	 */
	unsigned int evaluate_candidate( const Candidate& _x, const double _level, double& _LB, Candidate& _x_best, const double _gap );

	/**
	 * Reduces/shrinks the current ILP problem by using the variables that were fixed in the last iteration of the Main algorithm.
	 */
	void reduce( const Candidate& _x, const FixedVars& _fixed_vars );

	// GETTERS:
	/**
	 * Returns the number of variables in the original ILP problem.
	 */
	unsigned int get_n_var() const;

	/**
	 * Returns the number of constraints in the original ILP problem.
	 */
	unsigned int get_n_const() const;

	/**
	 * Returns the objective value 'c[_index]' of the original ILP problem.
	 */
	double get_c( const int _index ) const;

	/**
	 * Returns the right-hand side value 'b_red[_row_index]' of the current ILP problem.
	 */
	double get_b_red( const int _row_index ) const;

	/**
	 * Returns 'h_red' for the current ILP problem.
	 */
	double get_h_red() const;

	/**
	 * Returns the coefficient A[_row,_column] of the constraint matrix of the original ILP problem.
	 */
	double get_A( const int _row, const int _column ) const;

	/**
	 * Returns 'j' such that c[j] is maximum among all the active variables.
	 */
	unsigned int get_c_max( const AV& _av ) const;

};

#endif /* ILP_H_ */
