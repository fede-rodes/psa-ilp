
#include "OrigProjSet.h"

using std::cout;
using std::endl;

//========================================//
//////// CONSTRUCTOR AND DESTRUCTOR ////////
//========================================//

OrigProjSet::OrigProjSet( const ILP& _ilp, const AV& _av ): ProjSet( _ilp, _av)
{
	// Inherited constructor.
}

//========================================//
/*
OrigProjSet::~OrigProjSet()
{
	// Inherited destructor.
}
*/
//========================================//
///////////////// METHODS //////////////////
//========================================//
// no se preocupen por este metodo!
// only used for the original_proj_set!!
// Check whether the initial problem can be reduced by analyzing the set of projections restricted to the interval [LB, level].
int OrigProjSet::reduce( const double _LB )
{

	//====================================================================================================================//
	// This method is not being used within the routine. It is just used to get some information that may be useful in future versions
	//====================================================================================================================//

	// Variables declaration.
	int    counter = 0;
	int*   fixed_vars = new int[ n_var ]; // set of variables that can be fixed in the initial problem.
	double P_up_0_i;
	double P_up_1_i;
	double P_low_0_i;
	double P_low_1_i;

	//
	for( unsigned int j = 0; j < n_var; j++ )
	{
		P_up_0_i  = P_up_0[j];
		P_up_1_i  = P_up_1[j];
		P_low_0_i = P_low_0[j];
		P_low_1_i = P_low_1[j];

		// First case:
		if( P_up_0_i < _LB && P_up_1_i >= _LB && P_low_1_i <= _LB )
		{
			fixed_vars[ counter ] = j;
			counter++;
		}

		// Second case:
		else if( P_up_0_i >= _LB && P_low_0_i <= _LB && P_low_1_i > _LB )
		{
			fixed_vars[ counter ] = j;
			counter++;
    	}

    	// Third case:
    	else if( P_up_0_i >= _LB && P_low_0_i <= _LB && P_up_1_i < _LB )
    	{
			fixed_vars[ counter ] = j;
			counter++;
		}

		// Fourth case:
		else if( P_low_0_i > _LB && P_up_1_i >= _LB && P_low_1_i <= _LB )
		{
			fixed_vars[ counter ] = j;
			counter++;
		}
	}

	// Display.
	if( counter > 0 )
	{
		cout << " The initial problem can be reduced " << counter << " variable(s)!" << endl;
		cout << " detail: ";
		for( int j = 0; j < counter - 1; j++ )
		{
			cout << "x_" << fixed_vars[j] << " , ";
		}

		cout << "x_" << fixed_vars[ counter - 1 ] << endl; // last item.
	}

	// Release memory.
	delete[] fixed_vars;

	return counter;
}

//========================================//
///////////////// GETTERS //////////////////
//========================================//

double OrigProjSet::get_UB() const
{
	//====================================================================================================================//
	// Based on the information given by the projections associated to the original ILP problem, this method
	// computes an upper (integer) bound for the optimal objective value of the original ILP problem.
	//====================================================================================================================//

	// Variables declaration.
	double UB = PSA_BIG_VALUE;
	double max_obj_val_j; // maximum objective value that can be reached by x_j based on the information given by the projections.

	// Get the maximum objective value that can be reached by all integer variables simultaneously.
	for( unsigned int j = 0; j < n_var; j++ )
	{
		max_obj_val_j = std::max( P_up_0[j], P_up_1[j] );

		if( max_obj_val_j < UB )
		{
			UB = max_obj_val_j;
		}
	}

	return std::floor( UB );
}
