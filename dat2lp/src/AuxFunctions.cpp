/**
 * AuxFunctions.cpp
 *
 *  Created on: 26/02/2014
 *      Author: Federico Rodes
 */

#include "AuxFunctions.h"

using namespace std;

//========================================//
//////////////// FUCNTIONS /////////////////
//========================================//

int procesar1( string linea )
{
	// Variables declaration.
   int i = 0, ccoef, fcoef;

   while( i < (int) linea.size() && ( linea[i] < '0' || linea[i]>'9' ) )
   {
	   i++;
   }

   ccoef = i;
   while( i < (int) linea.size() && linea[i] >= '0' && linea[i] <= '9' )
   {
	   i++;
   }

   fcoef = i;
   if( fcoef != ccoef )
   {
	   return calcularCoef( linea, ccoef, fcoef );
   }

   return 0;
}

//========================================//

int procesar2( string linea, int cont, double* fo )
{
	// Variables declaration.
   int i = 0, ccoef, fcoef;

   while( i < (int) linea.size() )
   {
      while( i < (int) linea.size() && ( linea[i] < '0' || linea[i] > '9' ) )
      {
    	  i++;
      }

      ccoef = i;
      while( i < (int) linea.size() && linea[i] >= '0' && linea[i] <= '9' )
      {
    	  i++;
      }

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

int calcularCoef( string linea, int ccoef, int fcoef )
{
	// Variables declaration.
	int i;
	int resu = 0;

	if( fcoef == ccoef )
	{
		return 1;
	}

	for( i = 0; i < fcoef - ccoef; i++ )
	{
		resu = resu + ( (int) linea[ fcoef - i - 1 ] - '0' ) * diezalai(i);
	}

	return resu;
}

//========================================//

int diezalai( int i )
{
	// Variables declaration.
	int resu = 1;
	int j;

	for( j = 1; j <= i; j++ )
	{
		resu = resu * 10;
	}

	return resu;
}

//========================================//

bool check_extension( const string& file_path, string& path_without_ext )
{
    size_t pos = file_path.rfind( '.' );

    if( pos == string::npos )
    {
        return false;
    }

    string ext = file_path.substr( pos + 1 );

    if ( ext == "dat" )
    {
    	path_without_ext = file_path.substr( 0, pos );
        return true;
    }

    return false;
}

//========================================//

