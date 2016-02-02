
#include "FixedVars.h"

using std::cout;
using std::endl;

//========================================//
//////// CONSTRUCTOR AND DESTRUCTOR ////////
//========================================//

FixedVars::FixedVars( const unsigned int _n_var ):

// CONSTANT FIELDS:
	n_var( _n_var )

// NON-CONSTANT FIELDS:
{
	array = new unsigned int[ n_var ]; // put rubbish in it, it doesn't matter.

	counter = 0; // number of fixed variables in the array.
}

//========================================//

FixedVars::~FixedVars()
{
	delete[] array;
}

//========================================//
///////////////// METHODS //////////////////
//========================================//

void FixedVars::display() const
{	
	cout << " fixed variables (" << counter << "):" << endl;

	if( counter > 0 )
	{
		for( unsigned int j = 0; j < counter - 1; j++ )
		{
			cout << "x_" << array[j] << ", ";
		}
		cout << "x_" << array[ counter - 1 ] << "." << endl; // last element in the array.
	}
}

//========================================//

void FixedVars::clear()
{
	counter = 0;
}

//========================================//

void FixedVars::add( const unsigned int _index )
{
	array[ counter++ ] = _index;
}

//========================================//

unsigned int FixedVars::size() const
{
	return counter;
}

//========================================//

unsigned int FixedVars::get_ith( const unsigned int _ith ) const
{

	// Check.
	if( _ith > counter - 1  )
	{
		cout << "FixedVars::get_ith: ERROR, _ith > n_fixed_vars - 1!" << endl;
		exit(1);
	}

	return array[ _ith ];
}

//========================================//
