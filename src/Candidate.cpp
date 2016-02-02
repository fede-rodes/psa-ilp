
#include "Candidate.h"

using std::cout;
using std::cerr;
using std::endl;

//========================================//
//////// CONSTRUCTOR AND DESTRUCTOR ////////
//========================================//

Candidate::Candidate( const unsigned int _n_var )

// CONSTANT FIELDS:
:	n_var( _n_var )

// NON-CONSTANT FIELDS:
{
	array = new int[ n_var ];
	for( int j = 0; j < n_var; j++ )
	{
		array[j] = -1; // '-1' means: no value has yet been assigned to 'x_j'.
	}
}

//========================================//

Candidate::~Candidate()
{
	delete[] array;
}

//======================================================//
//////// COPY CONSTRUCTOR AND ASSIGNMENT OPERATOR ////////
//======================================================//

Candidate::Candidate( const Candidate& other )

// CONSTANT FIELDS:
:	n_var( other.n_var )

// NON-CONSTANT FIELDS:
{
	array = new int[ n_var ];
	for( unsigned int j = 0; j < n_var; j++ )
	{
		array[j] = other.array[j]; 
	}
}
//=======================================================//

void Candidate::operator =( const Candidate& other )
{
	// Check.
	if( n_var != other.n_var )
	{
		cerr << "Candidate::assignment operator: n_var != rhs.n_var" << endl;
		exit(1);
	}

	for( unsigned int j = 0; j < n_var; j++ )
	{
		array[j] = other.array[j];
	}
}

//========================================//
///////////////// METHODS //////////////////
//========================================//

void Candidate::clear()
{
	for( unsigned int j = 0; j < n_var; j++ )
	{
		array[j] = -1;
	}
}

//========================================//

void Candidate::display() const
{
	//cout << "Candidate solution: " << endl;

	if( n_var > 0 )
	{
		cout << " x = [";
		for( unsigned int j = 0; j < n_var - 1; j++ )
		{
			cout << array[j] << ", ";
		}
		cout << array[ n_var - 1 ] << "]" << endl; // last element in the array.
	}
}

//========================================//
///////////////// SETTERS //////////////////
//========================================//

void Candidate::set_value( const unsigned int _index, const int _value )
{
	array[ _index ] = _value;
}

//========================================//
///////////////// GETTERS //////////////////
//========================================//

double Candidate::get_value( const unsigned int _index ) const
{
	return array[ _index ];
}

//========================================//


