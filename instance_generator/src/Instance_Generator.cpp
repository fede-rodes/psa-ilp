/**
* @file       : instance_generator.cpp
* @brief      : this program generates 0-1 multidimensional knapsack problem test-instances in .dat format.
*				 The instances are weakly correlated and all the coefficients are generated randomly.
*
*			     The program receives 4 inputs, namely:
*				 1) folder_path, path to the folder where the instances will be staored;
*                2) n_var, the number of variables that the instance will contain;
*				 3) n_const, the number of constraints that the instance will contain;
* 				 4) n_inst, number of instances to be generated;
*
* 				 .dat format files must respect the following structure:
*
*				 n_var variables
*                n_const constraints
*				 (space)
*                 OBJECTIVE FUNCTION -->
*				 (space)
*               f_1                 * coefficients of the objective function one after the other
*               f_2
*               .
*               .
*               .
*               f_(n_var)
*				 (space)
*               CONSTRAINTS -->
*				 (space)
*               a_1_1               * coefficients of the first row in the constraint Ax <= b
*               a_1_2
*               .
*               .
*               .
*               a_1_(n_var)
*     			 b_1
*				 (space)
*               a_2_1               * coefficients of the second row in the constraint Ax <= b
*               a_2_2
*               .
*               .
*               .
*               a_2_(n_var)
*     			 b_2
*				 (space)
*               .				     * coefficients of the i-th row in the constraint Ax <= b
*               .
*               .
*				 (space)
*               a_(n_const)_1       * coefficients of the last row in the constraint Ax <= b
*               a_(n_const)_2
*               .
*               .
*               .
*               a_(n_const)_(n_var)
*     			 b_(n_const)
* @usage      : instance_generator.exe [folder_path] [n_var] [n_const] [n_inst]
* @example    : instance_generator.exe ../../instances 50 3 10
* @author     : PSA-ilp
*/

#include <cstdlib>
#include <cmath>
#include <iostream>
#include <fstream>
#include <string>

using std::cout;
using std::endl;
using std::ofstream;
using std::string;

int main( int argc, char* argv[] )
{
	if( argc != 5 )
	{
      cout << endl << " Usage: instance_generator.exe [path_to_folder] [n_var] [n_const] [n_inst]" << endl;
      cout << " Example: instance_generator.exe ../../instances 50 3 10 " << endl;
	  return 0;
	}

   // Variables declaration.
   int i, j, ar;
   int n_var = atoi( argv[2] );
   int n_const = atoi( argv[3] );
   double A[ n_const ][ n_var ]; // constraint matrix in the problem: max fx s.t. Ax <= b
   double sum = 0;
   char file_name[55];
   string folder_path = argv[1];
   string full_path;

   for( ar = 1; ar <= atoi( argv[4] ); ar++ )
   {
	   // Fill the file_name.
	   sprintf( file_name, "Gen%d-%d-%d.dat", n_var, n_const, ar );

	   // Complete the full_path.
	   full_path = folder_path + "/" + file_name;
	   cout << "writing... " << full_path << endl;

	   // Open the file to start writing.
	   ofstream output_file( full_path.c_str() );

	   // Check.
	   if( !output_file )
	   {
		   cout << endl << " Error, the address '" << file_name << "' could not be found" << endl;
	       return 0;
	   }

	   output_file << n_var << " variables\n";
	   output_file << n_const << " constraints\n";

	   // First, fill the coefficient matrix 'A'.
	   for( j = 0; j < n_const; j++ )
	   {
		   for( i = 0; i < n_var; i++ )
		   {
			   A[j][i] = floor( (double)rand() / (double)( RAND_MAX + 1 ) * 1000 );
		   }
	   }

	   // Second, generate the coefficients in the objective function 'f' based on the information contained in A.
	   output_file << "\nOBJECTIVE FUNCTION -->\n\n";

	   for( i = 0; i < n_var; i++ )
	   {
		   for( j = 0; j < n_const; j++ )
		   {
			   sum = sum + A[j][i];
		   }

		   output_file << floor( ( sum / n_const ) + 10 ) << endl;
		   sum = 0;
	   }

	   // Lastly, generate the right hand side values 'b'.
	   output_file << "\nCONSTRAINTS -->\n\n";

	   for( j = 0; j < n_const; j++ )
	   {
		   for( i = 0; i < n_var; i++ )
		   {
			   output_file << A[j][i] << endl;
			   sum = sum + A[j][i];
		   }

		   output_file << floor( 0.5 * sum ) << endl << endl;
		   sum = 0;
	   }
   }

   /************ Close ************/

   cout << endl << " instance_generator --> done" << endl;

   return 0;
}
