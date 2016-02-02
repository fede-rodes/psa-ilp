/*
 * Parser.h
 *
 *  Created on: 12/03/2014
 *      Author: Federico Rodes
 */

#ifndef PARSER_H_
#define PARSER_H_

#include <iostream> // std::cout, std::cerr, std::endl
#include <cstdlib>  // std::exit()
#include <fstream>  // std::ifstream(), std::ofstream()
#include <string>

using std::string;

class Parser
{
	/**
	 * Disable copy constructor given by default.
	 */
	Parser( const Parser& other ) = delete;

private:

	// FIELDS:
	/**
	 * number of variables in the original ILP problem.
	 */
	unsigned int n_var;

	/**
	 * number of constraints in the original ILP problem.
	 */
	unsigned int n_const;

	/**
	 * coefficients of the objective function of the original ILP problem.
	 */
	double* c;

	/**
	 * constraint matrix in the equation Ax <= b of the original ILP problem.
	 */
	double** A;

	/**
	 * right hand side value in the equation Ax <= b of the original ILP problem.
	 */
	double* b;

	// METHODS: (used to read the .dat file)
	int procesar1( string _line );
	int procesar2( string _line, int _counter, double* _obj_fun );
	int calcularCoef( string _line, int _ccoef, int _fcoef );
	int diezalai( int _i );

	/**
	 * Verifies whether the input file has '.dat' file extension.
	 * @param file_path
	 * @return 'true' if the file extension is '.dat', false otherwise.
	 */
	bool check_extension( const string _file_path );

public:

	// CONSTRUCTOR AND DESTRUCTOR:
	/**
	 * Reads, line by line, all the information contained in the '.dat' file given by the user, and sets the fields of the Parser object.
	 */
	Parser( const string _path );
	virtual ~Parser();

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
	 * Returns the objective function coefficients 'c' of the original ILP problem.
	 */
	double* get_c() const;

	/**
	 * Returns the constraints matrix 'A' of the original ILP problem.
	 */
	double** get_A() const;

	/**
	 * Returns the right-hand array 'b' of the original ILP problem.
	 */
	double* get_b() const;

};

#endif /* PARSER_H_ */
