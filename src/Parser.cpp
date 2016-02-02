/*
 * Parser.cpp
 *
 *  Created on: 12/03/2014
 *      Author: Federico Rodes
 */

#include "Parser.h"

using std::cout;
using std::endl;
using std::string;

//========================================//
//////// CONSTRUCTOR AND DESTRUCTOR ////////
//========================================//

Parser::Parser( const string _path )
{
	// Variables declaration.
	n_var   = 0;
	n_const = 0;
	string aux;

	/************* Check extension and open the .dat file *************/

	if( !check_extension( _path ) )
	{
        cout << " Invalid input: must end with '.dat'" << endl;
        exit(0);
	}

	std::ifstream dat( _path.c_str() );

	// Check.
	if( !dat )
	{
		cout << " ILP parser: ERROR, file '" << _path << "' cannot be opened" << endl;
	    exit(0);
	}


	/************* Read one by one the lines of the .dat file and set the object fields *************/

	while( !dat.eof() && n_var == 0 )
	{
		getline( dat, aux );

		if ( !aux.empty() )
	    {
	      	n_var = procesar1( aux );
	    }
	}

	while( !dat.eof() && n_const == 0 )
	{
	   	getline( dat, aux );

	   	if ( !aux.empty() )
	    {
	       	n_const = procesar1( aux );
	    }
	}

	c = new double[ n_var ];
	A = new double*[ n_const ];
	for( int i = 0; i < n_const; i++ )
	{
		A[i] = new double[ n_var ];
	}
	b = new double[ n_const ];

	getline( dat, aux );
	getline( dat, aux );
	getline( dat, aux );

	for ( int cont = 0; !dat.eof() && cont < n_var; cont++ )
	{
		getline( dat, aux );
		c[ cont ] = atof( aux.c_str() );
	}

	getline( dat, aux );
	getline( dat, aux );

	for ( int i = 0; i < n_const; i++ )
	{
		getline( dat, aux );

		for ( int cont = 0; !dat.eof() && cont < n_var; cont++ )
		{
			getline( dat, aux );
			A[i][cont] = atof( aux.c_str() );
		}

		getline( dat, aux );
	   	b[i] = atof( aux.c_str() );
	}
}

//========================================//

Parser::~Parser()
{
	delete[] c;
	for( int i = 0; i < n_const; i++ )
	{
		delete[] A[i];
	}
	delete[] A;
	delete[] b;
}

//========================================//
///////////////// METHODS //////////////////
//========================================//

int Parser::procesar1( string linea )
{
   int i = 0, ccoef, fcoef;

   while( i < (int) linea.size() && ( linea[i] < '0' || linea[i] > '9' ) ) i++;

   ccoef = i;
   while( i < (int) linea.size() && linea[i] >= '0' && linea[i] <= '9' ) i++;
   fcoef = i;
   if( fcoef != ccoef ) return calcularCoef( linea, ccoef, fcoef );
   return 0;
}

//========================================//

int Parser::procesar2( string linea, int cont, double* fo )
{
   int i = 0, ccoef, fcoef;

   while( i < (int) linea.size() )
   {
      while( i < (int) linea.size() && ( linea[i] < '0' || linea[i] > '9' ) ) i++;

      ccoef = i;
      while( i < (int) linea.size() && linea[i] >= '0' && linea[i] <= '9' ) i++;
      fcoef = i;
      if( fcoef != ccoef )
      {
         fo[ cont ] = calcularCoef( linea, ccoef, fcoef );
         cont++;
      }
   }

   return cont;
}

//========================================//

int Parser::calcularCoef( string linea, int ccoef, int fcoef )
{
   if( fcoef == ccoef ) return 1;
   int i,resu = 0;
   for( i = 0; i < fcoef - ccoef; i++ ) resu = resu+( (int) linea[ fcoef - i - 1 ] - '0' ) * diezalai(i);
   return resu;
}

//========================================//

int Parser::diezalai( int i )
{
   int resu = 1, j;
   for( j = 1; j <= i; j++ ) resu = resu * 10;
   return resu;
}

//========================================//

bool Parser::check_extension( const string file_path )
{
	// Look for the last '.' in the file_path.
    size_t pos = file_path.rfind( '.' );

    // If there is no '.' in the string, pos = NULL.
    if( pos == string::npos )
    {
        return false;
    }

    // Consider the string that follows the last '.'.
    string ext = file_path.substr( pos + 1 );

    // Check extension.
    if ( ext == "dat" )
    {
        return true;
    }

    return false;
}

//========================================//
///////////////// GETTERS //////////////////
//========================================//

unsigned int Parser::get_n_var() const
{
	return n_var;
}

//========================================//

unsigned int Parser::get_n_const() const
{
	return n_const;
}

//========================================//

double* Parser::get_c() const
{
	return c;
}

//========================================//

double** Parser::get_A() const
{
	return A;
}

//========================================//

double* Parser::get_b() const
{
	return b;
}
