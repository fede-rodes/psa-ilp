
#include "List.h"

using std::cout;
using std::endl;

//========================================//
//////// CONSTRUCTOR AND DESTRUCTOR ////////
//========================================//

List::List( unsigned int _n_var ):

// CONSTANT FIELDS:
	n_var( _n_var )

// NON-CONSTANT FIELDS:
{
	// matrix declaration.
	matrix = new double*[ 1000 ];
	for( int i = 0; i < 1000; i++ )
	{
		matrix[i] = new double[ n_var ];
	}
	// matrix[i][j] = rubbish; it doesn't matter.

	counter = 0;
}

//========================================//

List::~List()
{
	for( int i = 0; i < 1000; i++ )
	{
		delete[] matrix[i];
	}
	delete[] matrix;
}

//========================================//
///////////////// METHODS //////////////////
//========================================//

void List::display() const
{	
	cout << "List of partial candidate solutions (" << counter  << " elements): " << endl;

	if( n_var > 0 )
	{
		for ( unsigned int i = 0; i < counter; i++ )
		{
			cout << "[";
			for( unsigned int j = 0; j < n_var - 1; j++ )
			{
				cout << matrix[i][j] << ", ";
			}
			cout << matrix[i][ n_var - 1 ] << "]" << endl; // last coordinate.
		}
	}
}

//========================================//

void List::add( const Candidate& _x )
{	
	// Add the new partial candidate solution to the 'matrix' at the row labeled 'counter'.
	for ( unsigned int j = 0; j < n_var; j++ )
	{
		matrix[ counter ][j] = _x.get_value(j);
	}

	counter++;
}

//========================================//

void List::extract( Candidate& _x )
{	
	counter--;

	// Extract the last partial candidate solution added to the list.
	for ( unsigned int j = 0; j < n_var; j++ )
	{
		_x.set_value( j, matrix[ counter ][j] );
	}
}

//========================================//

unsigned int List::size() const
{
	return counter;
}

