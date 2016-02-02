/**
 * @file     main.cpp
 * @brief    standalone application for solving ILP problems using projections.
 *           The algorithm scans the projection set associated to the given ILP problem and
 *           generates candidate solutions tailored to each of the possible optimal
 *           objective values (from now on 'levels'), say {z_1, ... , z_k}.
 *           The algorithm ends when a feasible candidate solution 'x' satisfying
 *           the condition z(x) = z_i is found when the algorithm is scanning level z_i.
 *
 *           Notice that the current version of PSA-ilp only accepts instances of the 0-1 multidimensional knapsack problem.
 *           This means, all variables of the problem are binary, the coefficients of the objective function
 *           are strictly positive integer values, and the constraint coefficients are non-negative integer values.
 *
 * @usage    psailp.exe path/to/dat/file
 * @example  psailp.exe ../instances/Gen20-2.1.dat
 * @author   Federico Rodes
 */

#include <string>
#include <iostream>  // std::cout, std::cerr, std::endl
#include <iomanip>   // std::setprecision()
#include <ctime>     // std::time(), std::localtime()
#include <algorithm> // std::max()
#include <string.h>  // strcmp()
#include <fstream>   // std::ifstream(), std::ofstream(), std::ios::app
#include <vector>

#include "ILP.h"
#include "Candidate.h"
#include "List.h"
#include "ProjSet.h"
#include "OrigProjSet.h"
#include "FixedVars.h"
#include "UserInterface.h" // display_help(), display_header(), display_footer()
#include "CPUTime.h" // TODO, change to 'timer class' (methods: timer.start(), timer.end()), see soplex timer class.
#include "MemUsage.h"
#include "Constants.h"
#include "Settings.h"
#include "Parser.h"

using std::cout;
using std::cerr;
using std::endl;
using std::string;
using std::vector;

int main( int argc, char* argv[] )
{

	/************* Check arguments, modify default settings and display help if necessary *************/

	Settings settings; // default parameters.

	// Parameters.
	char help[]       = "-help";
	char gap[]        = "-gap";
	char lpsolver[]   = "-lpsol";
	char cuts[]       = "-cuts";
	char heuristics[] = "-heu";
	char output[]     = "-out";
	char multisol[]   = "-msol";


	// Check arguments and modify settings if necessary.
	switch( argc )
	{
		case 2:
			if( strcmp( argv[1], help ) == 0 )
			{
				display_help();
				return 1;
			}
			break;

		default:
			if( argc % 2 == 1 ) // odd number of arguments.
			{
				cout << endl << " wrong number of arguments; try 'psailp.exe -help'" << endl;
				return 1;
			}
			else // even number of arguments.
			{
				for( int i = 0; i < ( argc - 2 )/2; i++ )
				{
					/* GAP */
					if( strcmp( argv[ 2*i + 1 ], gap ) == 0 )
					{
						settings.set_gap( argv[ 2*i + 2 ] );
					}

					/* LP SOLVER */
					else if( strcmp( argv[ 2*i + 1 ], lpsolver ) == 0 )
					{
						settings.set_lpsol( argv[ 2*i + 2 ] );
					}

					/* OUTPUT FILE */
					else if( strcmp( argv[ 2*i + 1 ], output ) == 0 )
					{
						settings.set_output( argv[ 2*i + 2 ] );
					}

					/* CUTS */
					else if( strcmp( argv[ 2*i + 1 ], cuts ) == 0 )
					{
						settings.set_cuts( argv[ 2*i + 2 ] );
					}

					/* HEURISTICS */
					else if( strcmp( argv[ 2*i + 1 ], heuristics ) == 0 )
					{
						settings.set_heuristics( argv[ 2*i + 2 ] );
					}
					
					/* MULTISOL */
					else if( strcmp( argv[ 2*i + 1 ], multisol ) == 0 )
					{
						settings.set_multisol( argv[ 2*i + 2 ] );
					}

					/* WRONG PARAMETER */
					else
					{
						cout << endl << " wrong parameter: " << argv[ 2*i + 1 ] << "; try 'psailp.exe -help'" << endl;
						return 1;
					}

				} // end for( int i = 0; i < ( argc - 2 )/2; i++ )
			}
			break;

	} // end switch( argc )


	/************* Load the ILP problem from the information contained in the '.dat' file *************/

	// TODO, clean all this!!
	//ILP* ilp; // empty ILP problem.

	//read_file( argv[ argc - 1 ], ilp ); // the last element in 'argv[]' should be the path to the '.dat' file.

	Parser parser( argv[ argc - 1 ] ); // the last element in the argv[] vector should be the path to the dat file.

	ILP ilp( parser.get_n_var(), parser.get_n_const(), parser.get_c(), parser.get_A(), parser.get_b() );


	/************* Variables declaration *************/

	unsigned int n_var = ilp.get_n_var();            // number of variables contained in the problem.
	unsigned int status;                             // INFEASIBLE, FEASIBLE, IMPROVE, OPTIMAL.
	unsigned int status_0;
	unsigned int status_1;
	double       LB = 0.0;                           // lower bound.
	unsigned int index;                              // index/coordinate to be fixed.
	unsigned int level_counter = 1;                  // keep track of the number of levels that were already scanned.
	unsigned int next_operation = PSA_INSPECT_LEVEL; // next operation to be performed in the 'while' loop below: INSPECT_LEVEL, ADD_CANDIDATES_TO_L, EXTRACT_CANDIDATE_FROM_L, REDUCE_PROBLEM, NEW_LEVEL, CLASSIFY_CANDIDATE.

	AV 		  av( n_var );		   // set of variables that have not yet been fixed (active variables).
	Candidate x_best( n_var );     // best feasible solution known so far.
	Candidate x( n_var );          // candidate solution being analyzed.
	FixedVars fixed_vars( n_var ); // set of variables that were fixed in the last iteration of the algorithm.
	List      L( n_var );          // list of partial candidate solutions to be scanned.
	
	vector < Candidate > solutions; // set of optimal solutions. 
	bool stop = false;


	/************* Calculate the initial projection set and the first level to be scanned *************/

	// Projection set associated to the original ILP problem.
	OrigProjSet orig_proj_set( ilp, av );

	// Initial level (UB) to be scanned.
	double level = orig_proj_set.get_UB();

	// Projection set associated to the current ILP problem.
	ProjSet proj_set = orig_proj_set;


	/************* Display info *************/

	display_header();
	settings.display();
	cout << " Legend: LB <= level, gap (( level - LB ) / LB)" << endl << endl;
	cout << std::setprecision(20) << " " << LB << " <= " << level << ", " << ( level - LB ) / LB << "%" << endl;
    cout << " CPU time: " << get_CPU_time() << " sec." << endl;
    cout << " memory consumption: " << (double)get_current_RSS()/1048576 << " MB" << endl << endl;


    /************* Start solving the ILP problem: scan the projection set restricted to each of the possible optimal objective values until reaching an optimal solution *************/

	while( !stop )
	{

		if( next_operation == PSA_ADD_CANDIDATES_TO_L )
		{
			// Since no variables were fixed during the PSA_INSPECT_LEVEL operation, we choose one
			// of the active variables by hand in order to reduce the current ILP problem.
			index = ilp.get_c_max( av );

			// For each value that can be assigned to the selected variable, we create a new partial candidate
			// solution. The new partial candidate solutions constructed in this manner are then added to 'L'
			// for future inspection.
			x.set_value( index, 0 );
			L.add( x );

			x.set_value( index, 1 );
			L.add( x );

			// Notice, both 'av' and 'fixed_vars' will be updated in the next operation.

			next_operation = PSA_EXTRACT_CANDIDATE_FROM_L;
		}


		if( next_operation == PSA_EXTRACT_CANDIDATE_FROM_L )
	   	{
			// Extract the last partial candidate added to 'L' to be studied.
	   		L.extract( x );

	   		// Set 'fixed_vars' as the set of fixed variables of x.
	   		fixed_vars.clear();
	   		for( unsigned int j = 0; j < n_var; j++ )
	   		{
	   			if( x.get_value(j) != -1 )
	   			{
	   				fixed_vars.add(j);
	   			}
	   		}

	   		// Set 'av' as the set of unfixed variables of x.
	   		av.clear();
	   		av.remove( fixed_vars );

	   		// Reconsider the original problem.
	   		ilp.clear();

	   		next_operation = PSA_REDUCE_PROBLEM;
	   	}


	   	if( next_operation == PSA_REDUCE_PROBLEM )
	    {
	   		// Reduce the current ILP problem by using the value of the variables that were fixed in the previous operation (PSA_EXTRACT_CANDIDATE_FROM_L or PSA_INSPECT_LEVEL).
			ilp.reduce( x, fixed_vars );

			// Re-calculate the projection set of the reduced ILP problem.
	    	//status = proj_set.calc_proj( ilp, av );
			status = proj_set.calc_proj( ilp, av, settings.get_lpsol() );

	    	if( status == PSA_INFEASIBLE )
	    	{
	    		next_operation = PSA_CLASSIFY_CANDIDATE;
	    	}

	    	else // ( status == FEASIBLE )
	    	{
	    		next_operation = PSA_INSPECT_LEVEL;
	    	}
		}


	    if( next_operation == PSA_NEW_LEVEL )
	    {
	    	// Go down a level.
	    	level--;
	    	level_counter++;

			// Display the new level, time and memory consumption.
			cout << std::setprecision(20) << " " << LB << " <= " << level << ", " << ( level - LB ) / LB << "%" << endl;
			cout << " CPU time: " << get_CPU_time() << " sec." << endl;
			cout << " memory consumption: " << (double)get_current_RSS()/1048576 << " MB" << endl << endl;
						
	        // Re-initialize all the variables.
	        proj_set = orig_proj_set;
	    	x.clear();
	    	ilp.clear();
	    	av.clear();

			// Check whether the initial problem can be reduced by analyzing the set of projections restricted to the interval [LB, level].
			//orig_proj_set.reduce( LB ); // no se preocupen por esta operacion!
						
	    	next_operation = PSA_INSPECT_LEVEL;

			// x_best is an optimal solution.
			if( ( level - LB ) / LB  <= settings.get_gap() )
			{
				if( settings.get_multisol() ) 
				{
					if( LB > level ) // stop after the optimal level is fully analysed.
					{
						stop = true;
						level_counter--;
					}
				}
			
				else // stop at the first optimal solution.
				{
					solutions.push_back( x_best );
					stop = true;
				}
			}			
		}


	    if( next_operation == PSA_INSPECT_LEVEL )
		{
	    	// Reset the fixed variables set before the inspection.
			fixed_vars.clear();

			// Inspect the projection set restricted to the current level. 'status == FEASIBLE'
			// means that all variables can take at least one integer value for the level being
			// scanned. In such a case, the variables that can take on a single value are stored
			// in the fixed_vars object, removed from the active variables set 'av', and fixed to
			// the partial candidate solution 'x'.
			status = proj_set.inspect( level, x, av, fixed_vars );

			next_operation = PSA_CLASSIFY_CANDIDATE;
        }


		if( next_operation == PSA_CLASSIFY_CANDIDATE )
		{
			//============================================================================//
			// Classify the partial candidate solution produced by the previous operation.
			//============================================================================//

			next_operation = PSA_NULL; // no operation has yet being fixed.

			if( status == PSA_FEASIBLE )
			{
				if( fixed_vars.size() == 0 ) // there were no changes in comparison with the previous iteration.
				{
					next_operation = PSA_ADD_CANDIDATES_TO_L;
				}

				else if( av.size() > 1 ) // there were some changes, but the candidate still has more than one unfixed coordinates.
				{
					next_operation = PSA_REDUCE_PROBLEM;
				}

				else // final candidate solution, or it contains at most one unfixed variable.
				{
	 				if( av.size() == 1 )
	 				{
	 					// Complete the partial candidate solution and determine whether it is feasible/infeasible, it outperforms the best candidate solution known so far, or it is an optimal solution.
	 					x.set_value( av.get_ith(0), 0 );
	 					status_0 = ilp.evaluate_candidate( x, level, LB, x_best, settings.get_gap() );

						if( status_0 == PSA_OPTIMAL )
						{
							solutions.push_back( x );
							if( !settings.get_multisol() )
							{
								stop = true;
							}
						}
						
	 					x.set_value( av.get_ith(0), 1 );
	 					status_1 = ilp.evaluate_candidate( x, level, LB, x_best, settings.get_gap() );
						
						if( status_1 == PSA_OPTIMAL )
						{
							solutions.push_back( x );
							if( !settings.get_multisol() )
							{
								stop = true;
							}
						}
						
	 					// Get the best status among status_0 and status_1.
	 					status = std::max( status_0, status_1 );
	 				}

	 				else // ( av.size() == 0 )
	 				{
	 					status = ilp.evaluate_candidate( x, level, LB, x_best, settings.get_gap() );
						
						if( status == PSA_OPTIMAL )
						{
							solutions.push_back( x );
							if( !settings.get_multisol() )
							{
								stop = true;
							}
						}
	 				}

	 	 			if( status == PSA_IMPROVE || status == PSA_OPTIMAL )
	 	  			{
	 					// Display info.
						cout << std::setprecision(20) << " " << LB << " <= " << level << ", " << ( level - LB ) / LB << "%" << endl;
						cout << " CPU time: " << get_CPU_time() << " sec." << endl;
						cout << " memory consumption: " << (double)get_current_RSS()/1048576 << " MB" << endl << endl;

	 					// Check whether the initial problem can be reduced by analyzing the set of projections restricted to the interval [LB, level].
						//orig_proj_set.reduce( LB ); // no se preocupen por esta operacion!
	 				}
			
	 			} // end else // final candidate solution, or it contains one unfixed variable.

	 		} // end if( status == FEASIBLE )
			
			if( next_operation == PSA_NULL )
			{
				// Determine the next operation to be performed. In case an optimal solution was found, the algorithm will stop at the next itaration.
				if( L.size() > 0 )
				{
					next_operation = PSA_EXTRACT_CANDIDATE_FROM_L;
				}

				else // ( L.size() == 0 )
				{
					next_operation = PSA_NEW_LEVEL;
				}
			}

	 	 } // end if( next_operation == CLASSIFY_CANDIDATE )

	} // end while( level > 0 && optimal == false && level > LB )


	/************* Display info *************/

	cout << endl<< endl << " FINAL RESULTS:" << endl;
    cout << endl << " optimal level: " << std::setprecision(20) << LB << endl;
    cout << " total CPU time: " << get_CPU_time() << " sec." << endl;
    cout << " peak memory consumption: " << (double)get_peak_RSS()/1048576 << " MB (" << (double)get_peak_RSS() << " bytes)" << endl;
    cout << " number of scanned levels: " << level_counter << endl;
	if( n_var < 500 )
	{
		cout << " optimal solution(s): " << endl;
		for( int s = 0; s < (int) solutions.size(); s++ )
		{
			solutions[s].display();
		}
	}
    display_footer();


    /************* Write down the solution to the output file given by the user (if applies) *************/

    if( settings.get_output() != "" )
    {
    	// Open the output file given by the user.
    	std::ofstream output_file;
    	output_file.open( settings.get_output().c_str() );

    	// Check.
    	if( !output_file )
    	{
    		cout << " output file cannot be opened." << endl;
    		return 1;
    	}

    	// Write down the solution info.
	    time_t t = time(0); // get time now
	    struct tm * now = localtime( & t );
	    output_file << "date: " <<  now -> tm_mday << "/" << (now -> tm_mon + 1) << "/" << (now -> tm_year + 1900) << endl << endl;
    	output_file << "input file: " << argv[ argc - 1 ] << endl << endl;
    	output_file << "optimal level: " << level << endl << endl;
    	output_file << "elapsed time: " << get_CPU_time() << " sec." << endl << endl;
    	output_file << "peak memory usage: " << std::setprecision(20) << (double)get_peak_RSS()/1048576 << " MB (" << (double)get_peak_RSS() << " bytes)" << endl << endl;
    	output_file << "number of scanned levels: " << level_counter << endl << endl;
		output_file << "nvars: " << n_var << endl << endl;
		output_file << "nsol: " << solutions.size() << endl << endl;
		for( int s = 0; s < (int) solutions.size(); s++ )
		{
			output_file << "sol: ";
			for( unsigned int j = 0; j < n_var - 1; j++ )
			{
				output_file << solutions[s].get_value(j) << ", ";
			}
			output_file << solutions[s].get_value( n_var - 1 ) << endl << endl; // last item.
		}
	    output_file.close();
    }

	return 0;
}


