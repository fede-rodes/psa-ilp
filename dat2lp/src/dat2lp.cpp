/**
 * @file     dat2lp.cpp
 * @brief    'dat_to_lp' transforms .dat format files into .lp format files.
 * 				 .lp format is widely used in most commercial and non-commercial LP and ILP solvers.
 * 				 For a detailed description about .lp format files, see the 'Input format' section in the
 * 			     website folder at bitbucket.
 * 			     .dat is a much more simpler format file and is used in PSA-ilp to avoid implementing
 * 				 a reader/parser program reading .lp format files.
 * 				 Commercial and non-commercial ILP solvers are not able to read .dat format files.
 *
 * 				 When do you need to use dat_to_lp?: dat_to_lp is used when you try to compare the
 * 				 performance of PSA-ilp against other ILP solver. In order to do that, you must:
 * 				 1) either generate a new instance by means of the 'instance_generator' program,
 * 				 or use one of the instances already stored in the database folder, say Gen100-2-1.dat
 * 				 2) run PSA-ilp.exe to solve Gen100-2-1.dat
 * 				 3) use dat_to_lp to translate Gen100-2-1.dat to Gen100-2-1.lp
 * 				 4) solve Gen100-2-1.lp by means of the ILP (non-)commercial solver.
 * 				 5) compare time and memory usage by each of the solvers.
 * @usage    dat2lp.exe [path_to_dat_file]
 * @example  dat2lp.exe ../../instances/Gen20-2-1.dat
 * @author   PSA-ilp
 */




#include <cstdlib>
#include <string>
#include <math.h>
#include <iostream>
#include <fstream>
#include <iomanip>

#include "AuxFunctions.h"

using std::cout;
using std::endl;
using std::string;
using std::ifstream;
using std::ofstream;


int main( int argc, char* argv[] )
{
	/************* Check arguments and extension **************/

	// Check input.
    if( argc != 2 )
    {
    	cout << endl << " Usage: dat2lp.exe [path_to_dat_file]" << endl;
    	cout << " Example: 'dat2lp.exe ../../instances/Gen20-2-1.dat' translates ../../instances/Gen20-2-1.dat to ../../instances/Gen20-2-1.lp" << endl;
        return 0;
    }

	// Variables declaration.
    string dat_path = argv[1];
    string path_without_extension;

    // Check extension.
    if( !check_extension( dat_path, path_without_extension ) )
    {
        cout << " Invalid input: must end with '.dat'" << endl;
        return 0;
    }

    /************* Open the input .dat file **************/

    ifstream input_file( dat_path.c_str() );

	// Check.
	if( !input_file )
	{
		cout << endl << " Error, the file '" << dat_path << "' cannot be opened" << endl;
        return 0;
    }

    /************ Read one by one each of the lines of the .dat file **************/

	// Variables declaration.
    int n_var = 0;
    int n_const = 0;
	string line;

    while( !input_file.eof() && n_var == 0 )
    {
    	getline( input_file, line );
        if ( !line.empty() )
        {
        	n_var = procesar1( line );
        }
    }

    while( !input_file.eof() && n_const == 0 )
    {
    	getline( input_file, line );
        if ( !line.empty() )
        {
        	n_const = procesar1( line );
        }
    }

    // Variables declaration.
    int i, j, counter = 0;
    double* f = new double[ n_var ];
    double** A = new double*[ n_const ];
    double* b = new double[ n_const ];
    for( i = 0; i < n_const; i++ )
    {
    	A[i] = new double[ n_var ];
    }

    getline( input_file, line );
    getline( input_file, line );
    getline( input_file, line );

    for( counter = 0;!input_file.eof() && counter < n_var; counter++ )
    {
    	getline( input_file, line );
        f[ counter ] = atof( line.c_str() );
    }

    getline( input_file, line );
    getline( input_file, line );

    for( i = 0; i < n_const; i++ )
    {
    	getline( input_file, line );
        for( counter = 0; !input_file.eof() && counter < n_var; counter++ )
        {
        	getline( input_file, line );
            A[i][ counter ] = atof( line.c_str() );
        }
        getline( input_file, line );
        b[i] = atof( line.c_str() );
    }


    /************* Translate the .dat format file to .lp format *******************/

    // Variables declaration.
    string lp_path = path_without_extension + ".lp";
    ofstream output_file( lp_path.c_str() );

	// Check.
	if( !output_file )
	{
		cout << endl << " Error, the address '" << lp_path << "' could not be found" << endl;
        return 0;
    }

    output_file << "Maximize" << endl;
    output_file << "obj: ";
    for( j = 0; j < n_var - 1; j++ )
    {
    	output_file << f[j] << " x" << j << " + ";
    }
    output_file << f[ n_var - 1 ] << " x" << n_var - 1 << endl;

    output_file << "Subject To" << endl;
    for( i = 0; i < n_const; i++ )
    {
    	for( j = 0; j < n_var - 1; j++ )
    	{
    		output_file << A[i][j] << " x" << j << " + ";
    	}
    	output_file << A[i][ n_var - 1 ] << " x" << n_var - 1 << " <= " << b[i] << endl;
    }
    output_file << endl;

    output_file << "Binary" << endl;
    for( j = 0; j < n_var; j++ )
    {
    	output_file << " x" << j << endl;
    }

    output_file << "End" << endl;


    /*************** Close ***************/

    cout << endl << " dat2lp --> done" << endl;

    // Release memory.
    delete[] f;
    delete[] A;
    delete[] b;

    return 0;
}


