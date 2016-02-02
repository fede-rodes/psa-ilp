/*
 * AuxFunctions.h
 *
 *  Created on: 26/02/2014
 *      Author: Federico Rodes
 */

#ifndef AUXFUNCTIONS_H_
#define AUXFUNCTIONS_H_

#include <cstdlib>
#include <string>
#include <math.h>
#include <iostream>
#include <fstream>
#include <iomanip>

using std::string;

// FUNCTIONS:
int  procesar1( string linea );
int  procesar2( string linea, int cont, double* fo );
int  calcularCoef( string linea, int ccoef, int fcoef );
int  diezalai( int i );
bool check_extension( const string& file_path, string& path_without_ext );

#endif /* AUXFUNCTIONS_H_ */
