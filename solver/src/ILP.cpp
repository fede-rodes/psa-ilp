
#include "ILP.h"

using std::cout;
using std::endl;
using std::string;

//========================================//
//////// CONSTRUCTOR AND DESTRUCTOR ////////
//========================================//

ILP::ILP( const unsigned int _n_var, const unsigned int _n_const, const double* _c, double** _A, double* _b )

// CONSTANT FIELDS:
: 	n_var( _n_var ), n_const( _n_const ), c( _c )

// NON-CONSTANT FIELDS:
{
	//c = _c;
	A = _A;
	b = _b;

	/*
	// Variables declaration.
	unsigned int i;
	unsigned int j;

	// Objective function.
	c = new double[ n_var ];

	for( j = 0; j < n_var; j++ )
	{
		c[j] = *( _c + j );
	}

	// Constraint matrix.
	A = new double*[ n_const ];

	for( i = 0; i < n_const; i++ )
	{
		A[i] = new double[ n_var ];
	}

	for( i = 0; i < n_const; i++ )
	{
		for( j = 0; j < n_var; j++ )
		{
			A[i][j] = _A[i][j];
		}
	}

	// Right hand side value.
	b = new double[ n_const ];

	for( i = 0; i < n_const; i++ )
	{
		b[i] = _b[i];
	}

	*/
	// Right hand side value current/reduced ILP problem.
	b_red = new double[ n_const ];

	for( int i = 0; i < n_const; i++ )
	{
		b_red[i] = b[i];
	}

	// Height current/reduced ILP problem.
	h_red = 0;
}

//========================================//
/*
ILP::ILP( const string _path )
{
	// Variables declaration.
	n_var   = 0;
	n_const = 0;
	string aux;

	// Check extension and open the .dat file

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


	// Read one by one the lines of the .dat file and set the object fields

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
	b_red = new double[ n_const ];

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

	for( int i = 0; i < n_const; i++ )
	{
		b_red[i] = b[i];
	}

	h_red = 0;
}
*/
//========================================//

ILP::~ILP()
{
	/*
	delete[] c;
	delete[] A;
	delete[] b;
	*/
	delete[] b_red;
}

//========================================//
///////////////// METHODS //////////////////
//========================================//

void ILP::clear()
{
	for( int i = 0; i < n_const; i++ )
	{
		b_red[i] = b[i];
	}

	h_red = 0;
}

//========================================//

void ILP::display( const AV& _av ) const
{
	// Variables declaration.
	int ith_av;

	cout << "z = ";
	for( int j = 0; j < _av.size(); j++ )
	{
		ith_av = _av.get_ith(j);
		cout << c[ ith_av ] << "x_" << ith_av << " + ";
	}
	cout << h_red << endl;

	cout << "s.t." << endl;
	for( int i = 0; i < n_const; i++ )
	{
		if( _av.size() > 0 )
		{
			for( int j = 0; j < _av.size() - 1; j++ )
			{
				ith_av = _av.get_ith(j);
				cout << A[i][ ith_av ] << "x_" << ith_av << " + ";
			}
			cout << A[i][ _av.get_ith( _av.size() - 1 ) ] << "x_" << _av.get_ith( _av.size() - 1 );
		}
		cout << " <= " << b_red[i] << endl;
	}
}

//========================================//

unsigned int ILP::evaluate_candidate( const Candidate& _x, const double _level, double& _LB, Candidate& _x_best, const double _gap )
{

	//====================================================================================================================//
	// This method checks whether the given candidate solution outperforms the current LB or it is an optimal solution.
	//====================================================================================================================//

	// Variables declaration.
	unsigned int status = PSA_FEASIBLE;
	double obj_val = 0.0;
	double A_ix; // A_i multiply by x.

	// Check feasibility for the final candidate solution _x.
	for( unsigned int i = 0; i < n_const && status == PSA_FEASIBLE; i++ )
	{
		A_ix = 0.0;

		for( unsigned int j = 0; j < n_var; j++ )
		{
			A_ix += A[i][j] * _x.get_value(j);
		}

        if ( A_ix > b[i] )
        {
           	status = PSA_INFEASIBLE;
        }
	}

	// Check optimality and LB improvement.
	if( status == PSA_FEASIBLE )
	{
		// Calculate the objective value of the candidate solution _x.
		for( unsigned int j = 0; j < n_var; j++ )
		{
			obj_val += c[j] * _x.get_value(j);
		}

		//if( obj_val < _level )
		//{
		if( obj_val > _LB )
		{
			_LB = obj_val;
			_x_best = _x;
			status = PSA_IMPROVE;
		}
		
		if( ( _level - obj_val ) / obj_val <= _gap )
		{
			status = PSA_OPTIMAL;
		}
		//}

		//else
		//{
		//	status = PSA_OPTIMAL;
		//}
	}

	return status;
}

//========================================//

void ILP::reduce( const Candidate& _x, const FixedVars& _fixed_vars )
{

	//====================================================================================================================//
	// This method uses the variables that were fixed during the last iteration of the Main algorithm (_fixed_vars)
	// in order to shrink the size of the current ILP problem.
	//====================================================================================================================//

	// Variables declaration.
	int n_fixed_vars = _fixed_vars.size();
	int index;

	for( int j = 0; j < n_fixed_vars; j++ )
	{
		index = _fixed_vars.get_ith( j );
	    h_red += c[ index ] * _x.get_value( index );

	    for( int i = 0; i < n_const; i++ )
	    {
	    	b_red[i] -= A[i][ index ] * _x.get_value( index );
	    }
	}
}

//========================================//
///////////////// GETTERS //////////////////
//========================================//

unsigned int ILP::get_c_max( const AV& _av ) const
{

	//====================================================================================================================//
	// This method is used in the ADD_CANDIDATES_TO_L operation to select the variable that is going to be used to split
	// the current candidate solution.
	//====================================================================================================================//

	// Variables definition.
	unsigned int index;
	unsigned int max_index;
	double max_obj_value = PSA_SMALL_VALUE;


	// Select the active variable with the highest objective value.
	for( int j = 0; j < _av.size(); j++ )
	{
		index = _av.get_ith(j);

		if( max_obj_value < c[ index ] )
		{
			max_obj_value = c[ index ];
			max_index = index;
		}
	}

	return max_index;
}

//========================================//

unsigned int ILP::get_n_var() const
{
	return n_var;
}

//========================================//

unsigned int ILP::get_n_const() const
{
	return n_const;
}

//========================================//

double ILP::get_c( const int _index ) const
{
	return c[ _index ];
}

//========================================//

double ILP::get_b_red( const int _row_index ) const
{
	return b_red[ _row_index ];
}

//========================================//

double ILP::get_h_red() const
{
	return h_red;
}

//========================================//

double ILP::get_A( const int _row, const int _column ) const
{
	return A[ _row ][ _column ];
}
