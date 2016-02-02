
#include "AV.h"

using std::cout;
using std::endl;

//========================================//
//////// CONSTRUCTOR AND DESTRUCTOR ////////
//========================================//

AV::AV( const unsigned int _n_var )

// CONSTANT FIELDS:
:	n_var( _n_var )

// NON-CONSTANT FIELDS:
{
	used_row = 0;
	non_used_row = 1;

	// Declare the two-row matrix.
	two_row_matrix = new unsigned int*[2];
	two_row_matrix[ used_row ] = new unsigned int[ n_var ];
	two_row_matrix[ non_used_row ] = new unsigned int[ n_var ];

	// Initialize the two_row_matrix.
	for( unsigned int j = 0; j < n_var; j++ )
	{
		two_row_matrix[ used_row ][j] = j;
		//two_row_matrix[ non_used_row ][j] = 'rubbish';, it doesn't matter.
	}

	counter = n_var;
}

//========================================//

AV::~AV()
{
	delete[] two_row_matrix[1];
	delete[] two_row_matrix[0];
	delete[] two_row_matrix;
}

//========================================//
///////////////// METHODS //////////////////
//========================================//

void AV::display() const
{
	cout << "active variable set (n_av = " << counter << ", used_row = " << used_row << "): " << endl;
	
	if( counter > 0 )
	{
		for( unsigned int j = 0; j < counter - 1; j++ )
		{
			cout << "x_" << get_ith(j) << ", ";
		}
		cout << "x_" << get_ith( counter - 1 ) << "." << endl; // last element in the array.
	}
}

//========================================//

void AV::clear()
{
	counter = n_var;

	used_row = 0;
	non_used_row = 1;

	for( unsigned int j = 0; j < n_var; j++ )
	{
		two_row_matrix[ used_row ][j] = j;
		//two_row_matrix[non_used_row][j] = 'rubbish', it doesn't matter.
	}
}

//========================================//

void AV::remove( const FixedVars& _fixed_vars )
{
	// Variables declaration.
	unsigned int n_fixed_vars = _fixed_vars.size(); // number of variables to be removed from 'two_row_matrix[used_row]'.
	unsigned int index_fv = 0; // index being used in the 'fixed_vars' set.
	unsigned int index_u = 0;  // index/column being used in the row 'two_row_matrix[used_row]'.
	unsigned int index_nu = 0; // index/column being used in the row 'two_row_matrix[non_used_row]'.

	// Check.
	if( counter < n_fixed_vars )
	{
		cout << "AV::remove: ERROR, number of active variables < number of indices to remove!" << endl;
		exit(1);
	}

	// Copy all the active variables in 'two_row_matrix[used_row]' that are not contained in 'fixed_vars' to 'two_row_matrix[non_used_row]'.
	while( index_fv < n_fixed_vars )
	{
		if( _fixed_vars.get_ith( index_fv ) == two_row_matrix[ used_row ][ index_u ] )
		{
			index_fv++;
		}

		else
		{
			two_row_matrix[ non_used_row ][ index_nu ] = two_row_matrix[ used_row ][ index_u ];
			index_nu++;
		}

		// Move to the next element in 'two_row_matrix[used_row]'.
		index_u++;
	}

	// Complete 'two_row_matrix[non_used_row]' with the remaining active variables contained in 'two_row_matrix[used_row]' (if applies).
	for( int i = index_u; i < counter; i++ )
	{
		two_row_matrix[ non_used_row ][ index_nu ] = two_row_matrix[ used_row ][i];
		index_nu++;
	}

	// Update the number of active variables as well as the used and non-used rows.
	counter = index_nu;
	std::swap( used_row, non_used_row );
}

// Example:
// let us suppose that the original ILP problem has 8 variables, and after some iterations of the Main algorithm we get:
//
// av = [0,2,3,5,7] and fixed_vars =[2,5,7].
//
// Internally the previous objects can be represented as follows:
// av = [0,2,3,5,7] --> two_row_matrix = [      0,      2,      3,      5,      7,rubbish,rubbish,rubbish] <-- used_row
//						                 [rubbish,rubbish,rubbish,rubbish,rubbish,rubbish,rubbish,rubbish] <-- non_used_row
//					-->	counter = 5
//				    --> n_var = 8
//
// fixed_vars = [2,5,7] --> array = [2,5,7,rubbish,rubbish,rubbish,rubbish,rubbish]
//                      --> counter = 3
//				        --> n_var = 8
//
// The method described above basically implements the 'av -= fixed_vars' operation:
//
//								      av = [0,2,3,5,7]
//                         -  fixed_vars = [2,5,7]
//							    -----------------------
//                                    av = [0,3]
//
// Internally, the result 'av = [0,3]' is represented as follows:
//
// av = [0,3] --> two_row_matrix = [      0,      2,      3,      5,      7,rubbish,rubbish,rubbish] <-- non_used_row
//								   [      0,      3,rubbish,rubbish,rubbish,rubbish,rubbish,rubbish] <-- used_row
//			  --> counter = 2
//			  --> n_var = 8

//========================================//
///////////////// GETTERS //////////////////
//========================================//

unsigned int AV::size() const
{
	return counter;
}

//========================================//

unsigned int AV::get_ith( const unsigned int _ith ) const
{
	// Check.
	if( _ith > counter - 1  )
	{
		cout << "AV::get_ith: ERROR, _ith > n_av - 1!" << endl;
		exit(1);
	}

	return two_row_matrix[ used_row ][ _ith ];
}

