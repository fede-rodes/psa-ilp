
#include "ProjSet.h"

using std::cout;
using std::cerr;
using std::endl;

//========================================//
//////// CONSTRUCTOR AND DESTRUCTOR ////////
//========================================//

ProjSet::ProjSet( const ILP& _ilp, const AV& _av )

// CONSTANT FIELDS:
: 	n_var( _ilp.get_n_var() )

// NON-CONSTANT FIELDS:
{
	// Variables declaration.
	unsigned int status;

	// Initialize.
	P_up_0  = new double[ n_var ];
	P_up_1  = new double[ n_var ];
	P_low_0 = new double[ n_var ];
	P_low_1 = new double[ n_var ];

	//status = calc_proj( _ilp, _av );
	status = calc_proj( _ilp, _av, PSA_GLPK ); // TODO, add lp_solver argument to constructor.

	// Check.
	if ( status == PSA_INFEASIBLE )
	{
		cout << "ProjSet::constructor: the original ILP problem is infeasible." << endl;
		exit(0);
	}
}

//========================================//

ProjSet::~ProjSet()
{
	delete[] P_up_0;
	delete[] P_up_1;
	delete[] P_low_0;
	delete[] P_low_1;
}

//=======================================================//
//////// COPY CONSTRUCTOR AND ASSIGNEMENT OPERATOR ////////
//=======================================================//

ProjSet::ProjSet( const ProjSet& other )

// CONSTANT FIELDS:
:	n_var( other.n_var )

// NON-CONSTANT FIELDS:
{
	P_up_0  = new double[ n_var ];
	P_up_1  = new double[ n_var ];
	P_low_0 = new double[ n_var ];
	P_low_1 = new double[ n_var ];

	for( int j = 0; j < n_var; j++ )
	{
		P_up_0[j]  = other.P_up_0[j];
		P_up_1[j]  = other.P_up_1[j];
		P_low_0[j] = other.P_low_0[j];
		P_low_1[j] = other.P_low_1[j];
	}
}

//=======================================================//
/*
Candidate& Candidate::operator=( const Candidate& other )
{
    if (this != &other) // protect against invalid self-assignment
    {
        // 1: allocate new memory and copy the elements
        int* new_array = new int[other.n_var];
        std::copy(other.array, other.array + other.n_var, new_array);

        // 2: deallocate old memory
        delete [] array;

        // 3: assign the new memory to the object
        array = new_array;
    }
    // by convention, always return *this
    return *this;
}
*/
//=======================================================//

void ProjSet::operator =( const ProjSet& other )
{
	// Check.
	if( n_var != other.n_var )
	{
		cerr << "ProjSet::assignment operator: n_var != other.n_var" << endl;
		exit(1);
	}

	for( int j = 0; j < n_var; j++ )
	{
		P_up_0[j]  = other.P_up_0[j];
		P_up_1[j]  = other.P_up_1[j];
		P_low_0[j] = other.P_low_0[j];
		P_low_1[j] = other.P_low_1[j];
	}
}

//========================================//
///////////////// METHODS //////////////////
//========================================//

void ProjSet::display( const AV& _av ) const
{
	// Variables declaration.
	int n_av = _av.size(); // number of active variables.
	int ith_av;            // i-th active variable.

	cout << "Projection set associated to the current ILP problem (P_up_0, P_up_1, P_low_0, P_low_1): " << endl;
	for( int j = 0; j < n_av; j++ )
	{
		// Consider one by one each of the active variables.
		ith_av = _av.get_ith(j);

		cout << P_up_0[ ith_av ] << ", " << P_up_1[ ith_av ] << ", " << P_low_0[ ith_av ] << ", " << P_low_1[ ith_av ] << endl;
	}

	_av.display();
}

//========================================//

unsigned int ProjSet::calc_proj( const ILP& _ilp, const AV& _av, const int _lp_solver )
{
	// Variables declaration.
	unsigned int status;

	switch( _lp_solver )
	{
	/*
		case PSA_QSOPT:

			status = calc_proj_qsopt( _ilp, _av );

		break;
	*/
		default:

			status = calc_proj_glpk( _ilp, _av );

		break;
	}

	return status;
}

//========================================//
/*
unsigned int ProjSet::calc_proj_qsopt( const ILP& _ilp, const AV& _av )
{
	//====================================================================================================================//
	// This method uses an external LP solver (QSopt) to calculate the projection set associated to the given ILP problem.   						  						     *
	//====================================================================================================================//

	// Variables declaration.
	unsigned int status_0 = PSA_FEASIBLE;
	unsigned int status_1 = PSA_FEASIBLE;
	unsigned int status   = PSA_FEASIBLE;
	int     n_av     = _av.size();					  // number of active variables.
	int     ith_av;                                   // i-th active variable.
	int  	err;                        		      // returned value. If no error occurs during the process, err = 0; otherwise err > 0.
	int  	n_const  = _ilp.get_n_const(); 		      // number of constraints.
	int* 	cmatcnt  = new int[ n_av ];  		      // an array of length ncols; the jth entry specifies the number of non-zeros in the jth column.
	int* 	cmatbeg  = new int[ n_av ];  		      // an array of length ncols; the jth entry specifies the location of the start of the entries for the jth column in the arrays cmatind and cmatval.
	int* 	cmatind  = new int[ n_av * n_const ];     // an array that contains the row indices of the non-zero entries in the constraint matrix. The indices of the jth column must be stored consecutively starting at entry number cmatbeg[j] (there are cmatcnt[j] entries for the jth column).
	double* cmatval  = new double[ n_av * n_const ];  // an array that contains the values of the non-zero entries in the constraint matrix. The non-zero values of the jth column must be stored consecutively starting at entry number cmatbeg[j] (there are cmatcnt[j] entries for the jth column).
	double* obj      = new double[ n_av ];			  // an array of length ncols; the jth entry is the coefficient of the jth variable in the objective function.
	double* rhs      = new double[ n_const ];		  // an array of length nrows; the ith entry is the right-hand-side value of the ith constraint.
	char*   sense    = new char[ n_const ];			  // an array of length nrows; the ith entry specifies the sense of the ith constraint; to specify an equation use 'E', to specify a <= constraint use 'L', and to specify a >= constraint use 'G'.
	double* lower    = new double[ n_av ];			  // an array of length ncols; the jth entry is the lower bound for the jth variable (use QS_MAXDOUBLE if the jth variable has no lower bound).
	double* upper    = new double[ n_av ];			  // an array of length ncols; the jth entry is the upper bound for the jth variable (use QS_MAXDOUBLE if the jth variable has no upper bound).
	double  obj_val;								  // optimal objective value to LP.
	double  temp_obj_val;							  // optimal objective value to the modified LP.
	double* x        =  new double[ n_av ];		      // optimal solution.
	double* rc 		 =  new double[ n_av ];		      // reduced costs.
	int 	stat;									  // solution status.
	//Value Constant           Meaning
	//  1   QS_LP_OPTIMAL      Optimal solution found.
	//  2   QS_LP_INFEASIBLE   LP problem has no feasible solution.
	//  3   QS_LP_UNBOUNDED    LP problem objective is unbounded.
	//  4   QS_LP_ITER_LIMIT   Iteration limit reached.
	//  5   QS_LP_TIME_LIMIT   Time limit reached.
	//  6   QS_LP_UNSOLVED     Could not solve the LP problem.

	//char*   cstat    =  new char[ n_av ];             // returns an array specifying the status of each column (variable) in the basis; this field can be NULL, if it is not NULL it should point to an array of length at least ncols, the number of columns in the problem.
	//char*   rstat    =  new char[ n_const ];          // returns an array specifying the status of each row (constraint) in the basis; this field can be NULL, if it is not NULL it should point to an array of length at least nrows, the number of rows in the problem.
	//double  minD, maxU;


	/************** Case: ILP problem contains only one variable **************

	if( n_av == 1 )
	{
		ith_av = _av.get_ith(0);

		// Determine whether P^low and P^up are feasible.
        for ( int i = 0; i < n_const && status_0 == PSA_FEASIBLE; i++ )
        {
        	if ( _ilp.get_b_red(i) < 0 )
        	{
        		status_1 = PSA_INFEASIBLE, status_0 = PSA_INFEASIBLE;
        	}

        	else if ( _ilp.get_b_red(i) < _ilp.get_A( i, ith_av ) )
        	{
        		status_1 = PSA_INFEASIBLE;
        	}
        }

        // Calculate P^low and P^up based on the previous answer.
        if ( status_1 == PSA_INFEASIBLE && status_0 == PSA_INFEASIBLE )
        {
        	status = PSA_INFEASIBLE;
        }

        else if ( status_1 == PSA_INFEASIBLE )
        {
		     P_up_0[ ith_av ]  = _ilp.get_h_red();
		     P_up_1[ ith_av ]  = -1;
	         P_low_0[ ith_av ] = _ilp.get_h_red();
		     P_low_1[ ith_av ] = -1;

		     status = PSA_FEASIBLE;
        }

        else
        {
        	 P_up_0[ ith_av ]  = _ilp.get_h_red();
        	 P_up_1[ ith_av ]  = _ilp.get_c( ith_av ) + _ilp.get_h_red();
        	 P_low_0[ ith_av ] = _ilp.get_h_red();
        	 P_low_1[ ith_av ] = _ilp.get_c( ith_av ) + _ilp.get_h_red();

        	 status = PSA_FEASIBLE;
        }
	}


	/************** Case: ILP problem contains at least two variables. **************

	else // if ( n_av > 1 )
	{
		//====================================================================================================================//
		// Calculate P^low. Given that all coefficients of the objective function are positive P^low is given by the following
		// formula: P^low(x_j) = c_j x_j + h_red
		//====================================================================================================================//

		for( int j = 0; j < n_av && status == PSA_FEASIBLE; j++ )
		{
			// Consider one by one each of the active variables of the problem.
			ith_av = _av.get_ith(j);

			// Check feasibility.
	        for ( int i = 0; i < n_const && status_0 == PSA_FEASIBLE; i++ )
	        {
	        	if ( _ilp.get_b_red(i) < 0 )
	        	{
	        		status_1 = PSA_INFEASIBLE, status_0 = PSA_INFEASIBLE;
	        	}

	        	else if ( _ilp.get_b_red(i) < _ilp.get_A( i, ith_av ) )
	        	{
	        		status_1 = PSA_INFEASIBLE;
	        	}
	        }

	        // Calculate P^low and P^up based on the previous answer.
	        if ( status_1 == PSA_INFEASIBLE && status_0 == PSA_INFEASIBLE )
	        {
	        	status = PSA_INFEASIBLE;
	        }

	        else if ( status_1 == PSA_INFEASIBLE )
	        {
		         P_low_0[ ith_av ] = _ilp.get_h_red();
			     P_low_1[ ith_av ] = -1;
	        }

	        else
	        {
	        	 P_low_0[ ith_av ] = _ilp.get_h_red();
	        	 P_low_1[ ith_av ] = _ilp.get_c( ith_av ) + _ilp.get_h_red();
	        }

	        // Reset variables for the next iteration.
	        status_0 = PSA_FEASIBLE;
	        status_1 = PSA_FEASIBLE;
		}


		//=====================================================================================//
		// Calculate P^up. In this case we need an LP solver to compute P^up(0) and P^up(1).
		//=====================================================================================//

		if( status == PSA_FEASIBLE )
		{
			QSprob lp; // empty LP problem.

			// Set the characteristics of the LP problem.
			for( int j = 0; j < n_av; j++ )
			{
				ith_av = _av.get_ith(j);

				cmatcnt[j] = n_const;
				cmatbeg[j] = j * n_const;
				obj[j]     = _ilp.get_c( ith_av );
				lower[j]   = 0;
				upper[j]   = 1;

				for( int i = 0; i < n_const ; i++ )
				{
					cmatind[ cmatbeg[j] + i ] = i;
					cmatval[ cmatbeg[j] + i ] = _ilp.get_A( i, ith_av );
				}
			}

			for( int i = 0; i < n_const; i++ )
			{
				rhs[i]   = _ilp.get_b_red(i);
				sense[i] = 'L';
			}

			// Load the LP problem by using the previous information.
			lp = QSload_prob( (const char*) NULL, n_av, n_const, cmatcnt, cmatbeg, cmatind, cmatval, QS_MAX, obj, rhs, sense, lower, upper, (const char**) NULL, (const char**) NULL );

			if( lp == (QSprob) NULL )
			{
				cerr << "Unable to load the LP problem" << endl;
				exit(1);
			}

			// Solve the LP problem using the Simplex-dual algorithm.
			err = QSopt_dual( lp, &stat );

			// Check.
			if( err )
			{
				cerr << "QSopt_dual failed with return code " << err << endl;
				exit(1);
			}

			else
			{
				switch( stat )
				{
					case QS_LP_OPTIMAL:
						//cout << "Found optimal solution to LP" << endl;
						break;

					case QS_LP_INFEASIBLE:
						//cout << "No feasible solution exists for the LP" << endl;
						//exit(0);
						break;

					case QS_LP_UNBOUNDED:
						cout << "The LP objective is unbounded" << endl;
						exit(0);
						break;

					default:
						cout << "LP could not be solved, status = " << stat << endl;
						exit(0);
						break;
				}
			}

			// Get the optimal objective value.
			err = QSget_objval( lp, &obj_val );

			// Check.
			if ( err )
			{
				cerr << "Could not get objective value, error code" << err << endl;
				exit(0);
			}

			obj_val += _ilp.get_h_red();

			// Get the optimal solution.
			err = QSget_x_array( lp, x );

			// Check.
			if( err )
			{
				cerr << "Could not get x-vector, error code " << err << endl;
				exit(0);
			}

			// Get the reduced costs.
			err = QSget_rc_array( lp, rc );

			// Check.
			if( err )
			{
				cerr << "Could not get reduced costs, error code " << err << endl;
				exit(0);
			}


			///// Get the reduced costs.
			///err = QSget_basis_array( lp, cstat, rstat );

			///// Check.
			///if( err )
			///{
			///	cerr << "Could not get basis, error code " << err << endl;
			///	exit(0);
			///}


			// Read the coordinates of the optimal solution and set the value for the upper projections.
			for( int j = 0; j < n_av; j++ )
			{
				ith_av = _av.get_ith(j);

	            P_up_0[ ith_av ]  = -1;
	            P_up_1[ ith_av ]  = -1;
	            ///P_low_0[ ith_av ] = -1;
	            ///P_low_1[ ith_av ] = -1;

				// Determine P_up_0:
				if( x[j] < 0.0000001 ) // x[ ith_av ] = 0 in the optimal solution of the LP-relaxation.
				{
					P_up_0[ ith_av ] = obj_val;
					P_up_1[ ith_av ] = obj_val - rc[j];


					///if( rc[j] < -0.000001 ) // negative reduced cost. Question: theoretically, if the variable takes the value zero at the optimal solution, then the reduced cost will be negative.
					///{
		        		//P_up_1[ ith_av ] = obj_val + rc[j];
		        		//cout << "calculate projections: x[j] = 0 && rc[j] < 0" << endl;
					///}
					///else if( rc[j] < 0.000001 )
					///{
						//cout << "calculate projections: x[j] = 0 && rc[j] = 0" << endl;
					///}
					///else
					///{
					///	P_up_1[ ith_av ] = obj_val - rc[j];
						//cout << "calculate projections: x[j] = 0 && rc[j] > 0" << endl;
					///}

				}

				// Determine P_up_1:
				else if( x[j] > 0.999999 ) // x[ ith_av ] = 1 in the optimal solution of the LP-relaxation.
				{
					P_up_1[ ith_av ] = obj_val;
					//P_up_0[j] = obj_val + rc[j];


					///if ( rc[j] > 0.000001 ) // positive reduced cost. Question: theoretically, if the variable takes a positive value at the optimal solution, then the reduced cost will be zero.
					///{
					///	P_up_0[j] = obj_val - rc[j];
					///	cout << "calculate projections: x[j] = 1 && rc[j] > 0" << endl;
					///}
					///else if ( rc[j] > -0.000001 ) // positive reduced cost. Question: theoretically, if the variable takes a positive value at the optimal solution, then the reduced cost will be zero.
					///{
					///	P_up_0[j] = obj_val - rc[j];
					///	cout << "calculate projections: x[j] = 1 && rc[j] == 0" << endl;
					///}
					///else // positive reduced cost. Question: theoretically, if the variable takes a positive value at the optimal solution, then the reduced cost will be zero.
					///{
					///	P_up_0[j] = obj_val + rc[j];
					///	cout << "calculate projections: x[j] = 1 && rc[j] < 0" << endl;
					///}

				}

				///else
				///{
					//cout << "i: " << i << endl;
				///CPXgetijrow( env, lp, -1, j, &numBase ); // get a pointer to an integer specifying the position in the basis header of the column of variable x_j.
				///CPXbinvarow( env, lp, numBase, row ); // compute the numBase-th row of the tableau.
				///CPXbinvrow( env, lp, numBase, &row[n_av] ); // compute the numBase-th row of the basis inverse.
				///minD = 9999999; maxU = -9999999;
				///for ( int i = 0; i < n_av; i++ )
				///{
						//cout << j << ": " << row[j] << " - " << dj[j] << " - " << cstat[j] << endl;
						// cstat == 0 --> CPX_AT_LOWER 0 variable at lower bound, cstat == 2 --> CPX_AT_UPPER 2	variable at upper bound.
				///		if ( cstat[i] == 0 && row[i] > 0 && minD > -rc[i] / row[i] ) minD = -rc[i] / row[i];
				///		if ( cstat[i] == 2 && row[i] < 0 && minD > -rc[i] / row[i] ) minD = -rc[i] / row[i];
				///		if ( cstat[i] == 0 && row[i] < 0 && maxU < -rc[i] / row[i] ) maxU = -rc[i] / row[i];
				///		if ( cstat[i] == 2 && row[i] > 0 && maxU < -rc[i] / row[i] ) maxU = -rc[i] / row[i];
				///	}
				///	for ( int i = 0; i < n_const; i++ )
				///	{
						 //cout << j << ": " << row[VarInicial+j] << " - " << dj[VarInicial+j] << " - " << rstat[j] << endl;
				///         if ( rstat[i] == 0 && row[n_av+i] > 0 && minD > rc[n_av+i] / row[n_av+i] )
				///         {
				///        	 minD = rc[n_av+i]/row[n_av+i];
				///         }
				///         if ( rstat[i] == 0 && row[n_av+i] < 0 && maxU < rc[n_av+i] / row[n_av+i] )
				///         {
				///        	 maxU = rc[n_av+i]/row[n_av+i];
				///         }
				///	}

				///	P_up_0[ ith_av ] = obj_val - minD * x[j];
				///	P_up_1[ ith_av ] = obj_val - maxU * ( x[j] - 1 );
				///}
			}


			/////////////////////
			for( int j = 0; j < n_av; j++ )
			{
				ith_av = _av.get_ith(j);

				if ( P_up_0[ ith_av ] == -1 )
				{
					// Set UB to 0 for the current variable, and compute an UB for P_up_0.
					err = QSchange_bound( lp, j, 'U', 0.0 );

					// Check.
					if ( err )
					{
						cerr << "Could not change the bound, error code " << err << endl;
						exit(1);
					}

					// Resolve the LP problem using the Simplex-dual algorithm, and the nex configuration.
					err = QSopt_dual( lp, &stat );

					// Check.
					if( err )
					{
						cerr << "QSopt_dual failed with return code " << err << endl;
						exit(1);
					}

					else
					{
						switch( stat )
						{
							case QS_LP_OPTIMAL:
								//cout << "Found optimal solution to LP" << endl;
								// Get the optimal objective value.
								err = QSget_objval( lp, &temp_obj_val );
								// Check.
								if ( err )
								{
									cerr << "Could not get objective value, error code" << err << endl;
									exit(1);
								}
								// Set the value for the upper projection.
								P_up_0[ ith_av ] = temp_obj_val + _ilp.get_h_red();
								break;

							case QS_LP_INFEASIBLE:
								//cout << "No feasible solution exists for the LP" << endl;
								P_up_0[ ith_av ] = -1;
								break;

							case QS_LP_UNBOUNDED:
								cout << "The LP objective is unbounded" << endl;
								break;

							default:
								cout << "LP could not be solved, status = " << stat << endl;
								break;
						}
					}

					// Undo the previous modification: set UB to 1 for the current variable.
					err = QSchange_bound( lp, j, 'U', 1.0 );

					// Check.
					if ( err )
					{
						cerr << "Could not change the bound, error code " << err << endl;
						exit(1);
					}
				}
			}
			////////////////////

			for( int j = 0; j < n_av; j++ )
			{
				ith_av = _av.get_ith(j);

				if ( P_up_1[ ith_av ] == -1 )
				{
					// Set LB to 1 for the current variable, and compute an UB for P_up_1.
					err = QSchange_bound( lp, j, 'L', 1.0 );

					// Check.
					if ( err )
					{
						cerr << "Could not change the bound, error code " << err << endl;
						exit(1);
					}

					// Resolve the LP problem using the Simplex-dual algorithm, and the nex configuration.
					err = QSopt_dual( lp, &stat );

					// Check.
					if( err )
					{
						cerr << "QSopt_dual failed with return code " << err << endl;
						exit(1);
					}

					else
					{
						switch( stat )
						{
							case QS_LP_OPTIMAL:
								//cout << "Found optimal solution to LP" << endl;
								// Get the optimal objective value.
								err = QSget_objval( lp, &temp_obj_val );
								// Check.
								if ( err )
								{
									cerr << "Could not get objective value, error code" << err << endl;
									exit(1);
								}
								// Set the value for the upper projection.
								P_up_1[ ith_av ] = temp_obj_val + _ilp.get_h_red();
								break;

							case QS_LP_INFEASIBLE:
								//cout << "No feasible solution exists for the LP" << endl;
								P_up_1[ ith_av ] = -1;
								break;

							case QS_LP_UNBOUNDED:
								cout << "The LP objective is unbounded" << endl;
								break;

							default:
								cout << "LP could not be solved, status = " << stat << endl;
								break;
						}
					}

					// Undo the previous modification: set LB to 0 for the current variable.
					err = QSchange_bound( lp, j, 'L', 0.0 );

					// Check.
					if ( err )
					{
						cerr << "Could not change the bound, error code " << err << endl;
						exit(1);
					}
				}
			}

			// Release memory.
			QSfree_prob( lp );
		}

		// Release memory.
		delete[] cmatcnt;
		delete[] cmatbeg;
		delete[] cmatind;
		delete[] cmatval;
		delete[] obj;
		delete[] rhs;
		delete[] sense;
		delete[] lower;
		delete[] upper;
		delete[] x;
	}

	// Fix the rounding errors produced by the Simplex method.
	round_errors( _av );

	return status;
}
*/
//========================================//

unsigned int ProjSet::calc_proj_glpk( const ILP& _ilp, const AV& _av )
{
	//====================================================================================================================//
	// This method uses an external LP solver (GLPK) to calculate the projection set associated to the given ILP problem.   						  						     *
	//====================================================================================================================//

	// Variables declaration.
	unsigned int status_0 = PSA_FEASIBLE;
	unsigned int status_1 = PSA_FEASIBLE;
	unsigned int status   = PSA_FEASIBLE;
	int     n_av     = _av.size();					  // number of active variables.
	int     ith_av;                                   // i-th active variable.
	int  	err;                        		      // returned value. If no error occurs during the process, err = 0; otherwise err > 0.
	int  	n_const  = _ilp.get_n_const(); 		      // number of constraints.
	double  obj_val;								  // optimal objective value to LP.
	double  temp_obj_val;							  // optimal objective value to the modified LP.
	double* x        =  new double[ n_av ];		      // optimal solution.
	double* rc 		 =  new double[ n_av ];		      // reduced costs.
	int 	stat;									  // solution status.
	//Value Constant           Meaning
	//  1   QS_LP_OPTIMAL      Optimal solution found.
	//  2   QS_LP_INFEASIBLE   LP problem has no feasible solution.
	//  3   QS_LP_UNBOUNDED    LP problem objective is unbounded.
	//  4   QS_LP_ITER_LIMIT   Iteration limit reached.
	//  5   QS_LP_TIME_LIMIT   Time limit reached.
	//  6   QS_LP_UNSOLVED     Could not solve the LP problem.

	//char*   cstat    =  new char[ n_av ];             // returns an array specifying the status of each column (variable) in the basis; this field can be NULL, if it is not NULL it should point to an array of length at least ncols, the number of columns in the problem.
	//char*   rstat    =  new char[ n_const ];          // returns an array specifying the status of each row (constraint) in the basis; this field can be NULL, if it is not NULL it should point to an array of length at least nrows, the number of rows in the problem.
	//double  minD, maxU;

	int size = 1 + n_av * n_const;
	int* ia = new int[ size ];
	int* ja = new int[ size ];
	double* ar = new double[ size ];


	/************** Case: ILP problem conatins only one variable. **************/

	if( n_av == 1 )
	{
		ith_av = _av.get_ith(0);

		// Determine whether P^low and P^up are feasible.
        for ( int i = 0; i < n_const && status_0 == PSA_FEASIBLE; i++ )
        {
        	if ( _ilp.get_b_red(i) < 0 )
        	{
        		status_1 = PSA_INFEASIBLE, status_0 = PSA_INFEASIBLE;
        	}

        	else if ( _ilp.get_b_red(i) < _ilp.get_A( i, ith_av ) )
        	{
        		status_1 = PSA_INFEASIBLE;
        	}
        }

        // Calculate P^low and P^up based on the previous answer.
        if ( status_1 == PSA_INFEASIBLE && status_0 == PSA_INFEASIBLE )
        {
        	status = PSA_INFEASIBLE;
        }

        else if ( status_1 == PSA_INFEASIBLE )
        {
		     P_up_0[ ith_av ]  = _ilp.get_h_red();
		     P_up_1[ ith_av ]  = -1;
	         P_low_0[ ith_av ] = _ilp.get_h_red();
		     P_low_1[ ith_av ] = -1;

		     status = PSA_FEASIBLE;
        }

        else
        {
        	 P_up_0[ ith_av ]  = _ilp.get_h_red();
        	 P_up_1[ ith_av ]  = _ilp.get_c( ith_av ) + _ilp.get_h_red();
        	 P_low_0[ ith_av ] = _ilp.get_h_red();
        	 P_low_1[ ith_av ] = _ilp.get_c( ith_av ) + _ilp.get_h_red();

        	 status = PSA_FEASIBLE;
        }
	}


	/************** Case: ILP problem contains at least two variables. **************/

	else // if ( n_av > 1 )
	{
		//====================================================================================================================//
		// Calculate P^low. Given that all coefficients of the objective function are positive P^low is given by the following
		// formula: P^low(x_j) = c_j x_j + h_red
		//====================================================================================================================//

		for( int j = 0; j < n_av && status == PSA_FEASIBLE; j++ )
		{
			// Consider one by one each of the active variables of the problem.
			ith_av = _av.get_ith(j);

			// Check feasibility.
	        for ( int i = 0; i < n_const && status_0 == PSA_FEASIBLE; i++ )
	        {
	        	if ( _ilp.get_b_red(i) < 0 )
	        	{
	        		status_1 = PSA_INFEASIBLE, status_0 = PSA_INFEASIBLE;
	        	}

	        	else if ( _ilp.get_b_red(i) < _ilp.get_A( i, ith_av ) )
	        	{
	        		status_1 = PSA_INFEASIBLE;
	        	}
	        }

	        // Calculate P^low and P^up based on the previous answer.
	        if ( status_1 == PSA_INFEASIBLE && status_0 == PSA_INFEASIBLE )
	        {
	        	status = PSA_INFEASIBLE;
	        }

	        else if ( status_1 == PSA_INFEASIBLE )
	        {
		         P_low_0[ ith_av ] = _ilp.get_h_red();
			     P_low_1[ ith_av ] = -1;
	        }

	        else
	        {
	        	 P_low_0[ ith_av ] = _ilp.get_h_red();
	        	 P_low_1[ ith_av ] = _ilp.get_c( ith_av ) + _ilp.get_h_red();
	        }

	        // Reset variables for the next iteration.
	        status_0 = PSA_FEASIBLE;
	        status_1 = PSA_FEASIBLE;
		}


		//==============================================================================================//
		// Calculate P^up. In this case, an external LP solver is needed to compute P^up(0) and P^up(1).
		//==============================================================================================//

		if( status == PSA_FEASIBLE )
		{
			// Create an empty LP problem and set parameters.
			glp_prob* lp = glp_create_prob();
			glp_smcp parm;
			glp_init_smcp( &parm );
			parm.meth = GLP_DUAL;
			parm.msg_lev = GLP_MSG_OFF;

			/******** Set the characteristics of the LP problem ********/

			// Maximize.
			glp_set_obj_dir( lp, GLP_MAX );

			// Set b_red[].
			glp_add_rows( lp, n_const );
			for( int i = 0; i < n_const ; i++ )
			{
				glp_set_row_bnds( lp, i+1, GLP_UP, 0.0, _ilp.get_b_red(i) );
			}

			// Set variables.
			glp_add_cols( lp, n_av );
			for( int j = 0; j < n_av ; j++ )
			{
				ith_av = _av.get_ith(j);
				glp_set_col_bnds( lp, j+1, GLP_DB, 0.0, 1.0 );
				glp_set_obj_coef( lp, j+1, _ilp.get_c(ith_av) );
			}

			// Set the constraint matrix
			int counter = 1;
			for( int i = 0; i < n_const ; i++ )
			{
				for( int j = 0; j < n_av; j++ )
				{
					ith_av = _av.get_ith(j);

					ia[ counter ] = i + 1;
					ja[ counter ] = j + 1;
					ar[ counter ] = _ilp.get_A( i, ith_av );
					counter++;
				}
			}
			glp_load_matrix( lp, n_av * n_const, ia, ja, ar );

			/******** Solve the LP problem and get all the information about the optimal solution ********/

			err = glp_simplex( lp, &parm );

			// Check.
			if( err )
			{
				cerr << "glp_simplex failed with return code " << err << endl;
				exit(1);
			}

			// Get the optimal objective value.
			obj_val = glp_get_obj_val( lp );
			obj_val += _ilp.get_h_red();

			// Get the optimal solution.
			for( int j = 0; j < n_av; j++ )
			{
				ith_av = _av.get_ith(j);
				x[j] = glp_get_col_prim( lp, j+1 );
			}

			// Get reduced costs.
			for( int j = 0; j < n_av; j++ )
			{
				rc[j] = glp_get_col_dual( lp, j+1 );
			}

			/******** Set P_up_0/1[ith_av] = obj_val if x[iht_av] = 0/1 in the optimal solution ********/

			// Read the coordinates of the optimal solution and set the value for the upper projections.
			for( int j = 0; j < n_av; j++ )
			{
				ith_av = _av.get_ith(j);

			    P_up_0[ ith_av ]  = -1;
			    P_up_1[ ith_av ]  = -1;
				//P_low_0[ ith_av ] = -1;
				//P_low_1[ ith_av ] = -1;

				// Determine P_up_0:
				if( x[j] < 0.0000001 ) // x[ ith_av ] = 0 in the optimal solution of the LP-relaxation.
				{
					P_up_0[ ith_av ] = obj_val;
					P_up_1[ ith_av ] = obj_val + rc[j]; // upper bound for P_up_1[ ith_av ].
				}

				// Determine P_up_1:
				else if( x[j] > 0.999999 ) // x[ ith_av ] = 1 in the optimal solution of the LP-relaxation.
				{
					P_up_1[ ith_av ] = obj_val;
				}
			}

			/******** Set 'x[ith_av] = 0' and compute an UB for P_up_0 for the active variables that could determined in the previous step ********/
			
			for( int j = 0; j < n_av; j++ )
			{
				ith_av = _av.get_ith(j);

				if ( P_up_0[ ith_av ] == -1 )
				{
					// Set 'x[ith_av] = 0' in the original LP problem, and resolve this new LP problem by means of the Simplex-dual algorithm.
					glp_set_col_bnds( lp, j+1, GLP_FX, 0.0, 0.0 );
					err = glp_simplex( lp, &parm );

					// Check.
					if( err )
					{
						cerr << "glp_simplex failed with return code " << err << endl;
						exit(1);
					}

					// Get solution status.
					stat = glp_get_status( lp );

					switch( stat )
					{
						case GLP_OPT: // Optimal solution found.
						
							// Get the optimal objective value and set the value for the upper projection.
							temp_obj_val = glp_get_obj_val( lp );
							P_up_0[ ith_av ] = temp_obj_val + _ilp.get_h_red();
							
						break;

						case GLP_NOFEAS: // LP problem has no feasible solution.
						
							P_up_0[ ith_av ] = -1;
							
						break;

						case GLP_UNBND:
						
							cout << "The LP objective is unbounded" << endl;
							
						break;

						default:
						
							cout << "LP could not be solved, status = " << stat << endl;
							
						break;
					}

					// Undo the previous changes, i.e., re-set x[ith_av] UB to 1 in the original LP problem.
					glp_set_col_bnds(lp, j+1, GLP_DB, 0.0, 1.0);

				}
			}

			/******** Set 'x[ith_av] = 1' and compute an UB for P_up_1 for the active variables that could determined in the previous step ********/
			
			for( int j = 0; j < n_av; j++ )
			{
				ith_av = _av.get_ith(j);

				if ( P_up_1[ ith_av ] == -1 )
				{
					// Set 'x[ith_av] = 1' in the original LP problem, and resolve this new LP problem by means of the Simplex-dual algorithm.
					glp_set_col_bnds( lp, j+1, GLP_FX, 1.0, 1.0 );
					err = glp_simplex( lp, &parm );

					// Check.
					if( err )
					{
						cerr << "glp_simplex failed with return code " << err << endl;
						exit(1);
					}

					// Get solution status.
					stat = glp_get_status( lp );


					switch( stat )
					{
						case GLP_OPT: // Optimal solution found.
						
							// Get the optimal objective value and set the value for the upper projection.
							temp_obj_val = glp_get_obj_val( lp );
							P_up_1[ ith_av ] = temp_obj_val + _ilp.get_h_red();
							
						break;

						case GLP_NOFEAS: // LP problem has no feasible solution.
						
							P_up_1[ ith_av ] = -1;
							
						break;

						case GLP_UNBND:
						
							cout << "The LP objective is unbounded" << endl;
							
						break;

						default:
						
							cout << "LP could not be solved, status = " << stat << endl;
							
						break;
					}
				
					// Undo the previous changes, i.e., re-set x[ith_av] LB to 0 in the original LP problem.
					glp_set_col_bnds( lp, j+1, GLP_DB, 0.0, 1.0 );
				}
			}

			// Release memory.
			glp_delete_prob( lp );
		}

		// Release memory.
		delete[] x;
		delete[] rc;
		delete[] ia;
		delete[] ja;
		delete[] ar;
	}

	// Fix the rounding errors produced by the Simplex method.
	round_errors( _av );

	return status;
}

//========================================//

void ProjSet::round_errors( const AV& _av ) // TODO, check this method!!
{
	//====================================================================================================================//
	// This method fixes the rounding errors that might be produced when applying the Simplex method to compute the 
	// projection set associated to the given ILP problem.
	//====================================================================================================================//

	// Variables declaration.
	int n_av = _av.size();
	int ith_av;

	// Fix the rounding errors that could be produced when applying the Simplex
	// algorithm to compute the projection set associated to the current ILP problem.
	// Stretch up the upper projections and stretch down the lower projections.
	for( int i = 0; i < n_av; i++ )
	{
		// Consider one by one each of the active variables of the problem.
		ith_av = _av.get_ith(i);

		if ( P_low_0[ ith_av ] - std::floor( P_low_0[ ith_av ] ) < 0.0001 )
		{
			P_low_0[ ith_av ] = std::floor( P_low_0[ ith_av ] ) - 0.0001;
		}

		if ( std::ceil( P_up_0[ ith_av ] ) - P_up_0[ ith_av ] < 0.0001 )
		{
			P_up_0[ ith_av ] = std::ceil( P_up_0[ ith_av ] ) + 0.0001;
		}

		if ( P_low_1[ ith_av ] - std::floor( P_low_1[ ith_av ] ) < 0.0001 )
		{
			P_low_1[ ith_av ] = std::floor( P_low_1[ ith_av ] ) - 0.0001;
		}

		if ( std::ceil( P_up_1[ ith_av ] ) - P_up_1[ ith_av ] < 0.0001 )
		{
			P_up_1[ ith_av ] = std::ceil( P_up_1[ ith_av ] ) + 0.0001;
		}
	}
}

//========================================//
/*
void ProjSet::round_errors( const AV& _av )
{

	//====================================================================================================================//
	// This method fixes the rounding errors that might be produced by LP solver when computing the projection set
	// associated to the given ILP problem.
	//====================================================================================================================//

	// Variables declaration.
	int n_av = _av.size();
	int ith_av;

	// Fix the rounding errors.
	for( int i = 0; i < n_av; i++ )
	{
		// Consider one by one each of the active variables.
		ith_av = _av.get_ith(i);

		if ( P_up_0[ ith_av ] - floor( P_up_0[ ith_av ] ) < 0.0001 ) // TODO, what for???
		{
			P_up_0[ ith_av ] = floor( P_up_0[ ith_av ] );
		}

		else if ( ceil( P_up_0[ ith_av ] ) - P_up_0[ ith_av ] < 0.0001 )
		{
			P_up_0[ ith_av ] = ceil( P_up_0[ ith_av ] );
		}

		if ( P_up_1[ ith_av ] - floor( P_up_1[ ith_av ] ) < 0.0001 ) // TODO, what for???
		{
			P_up_1[ ith_av ] = floor( P_up_1[ ith_av ] );
		}

		else if ( ceil( P_up_1[ ith_av ] ) - P_up_1[ ith_av ] < 0.0001 )
		{
			P_up_1[ ith_av ] = ceil( P_up_1[ ith_av ] );
		}
	}
}
*/
//========================================//

unsigned int ProjSet::inspect( const double _level, Candidate& _x, AV& _av, FixedVars& _fixed_vars )
{

	//====================================================================================================================//
	// This method scans the set of projections restricted to the given level. The variables that can take on a single value
	// are stored in the fixed_vars object, removed from the active variables set 'av', and fixed to the partial candidate 'x'.
	//====================================================================================================================//

	// Variables declaration.
	unsigned int status = PSA_FEASIBLE; // FEASIBLE if there is at least one integer value to be assigned each active variable; INFEASIBLE otherwise.
	unsigned int n_av = _av.size(); 	// number of active variables.
	unsigned int ith_av; 				// i-th active variable.
	double P_up_0_i;					// P^up(0) associated to the i-th active variable.
	double P_up_1_i;					// P^up(1) associated to the i-th active variable.
	double P_low_0_i;					// P^low(0) associated to the i-th active variable.
	double P_low_1_i;					// P^low(1) associated to the i-th active variable.

	// Scan the projection set restricted to the given level in order to detect all the variables that can take on a single integer-value.
	for ( int i = 0; i < n_av && status == PSA_FEASIBLE; i++ )
	{
		// Get the i-th active variable along with its associated projection.
		ith_av = _av.get_ith(i);

		P_up_0_i  = P_up_0[ ith_av ];
		P_up_1_i  = P_up_1[ ith_av ];
		P_low_0_i = P_low_0[ ith_av ];
		P_low_1_i = P_low_1[ ith_av ];

		// Consider all the possible projection-level configurations to detect those variables that can take on a single value.

    	// First case:
		if( ( P_up_0_i < _level && P_up_1_i >= _level && P_low_1_i <= _level ) || ( P_low_0_i > _level && P_up_1_i >= _level && P_low_1_i <= _level ) )
		{
			_x.set_value( ith_av, 1 );
			_fixed_vars.add( ith_av );
		}

		// Second case:
		else if( ( P_up_0_i >= _level && P_low_0_i <= _level && P_low_1_i > _level ) || ( P_up_0_i >= _level && P_low_0_i <= _level && P_up_1_i < _level ) )
		{
			_x.set_value( ith_av, 0 );
			_fixed_vars.add( ith_av );
    	}

		// Last case:
		else if( ( P_up_0_i < _level && P_up_1_i < _level ) || ( P_up_0_i < _level && P_low_1_i > _level ) || ( P_low_0_i > _level && P_up_1_i < _level ) || ( P_low_0_i > _level && P_low_1_i > _level ) )
		{
			status = PSA_INFEASIBLE;
		}

	} // end for ( int i = 0; i < n_av && feasible; i++ )

	// If the previous inspection returns a feasible solution, update '_av'.
	if( status == PSA_FEASIBLE && _fixed_vars.size() > 0 )
	{
		_av.remove( _fixed_vars );
	}

	return status;
}

//========================================//

