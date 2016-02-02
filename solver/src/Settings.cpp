
#include "Settings.h"

using std::cout;
using std::endl;
using std::string;

//========================================//
//////// CONSTRUCTOR AND DESTRUCTOR ////////
//========================================//

Settings::Settings()
{
	output = "";
	lp_solver = PSA_GLPK;
	gap = 0.0;
	cuts = false;
	heuristics = false;
	multisol = false;
}

//========================================//

Settings::~Settings()
{
	// TODO Auto-generated destructor stub
}

//========================================//
///////////////// METHODS //////////////////
//========================================//

void Settings::display()
{
	cout << " Parameters: " << endl;
	cout << " lp solver: " << lp_solver << endl;
	cout << " gap: " << gap << endl;
	cout << " cuts: " << cuts << endl;
	cout << " heuristics: " << heuristics << endl;
	cout << " output file: " << output << endl << endl << endl;
}

//========================================//
///////////////// SETTERS //////////////////
//========================================//

void Settings::set_output( string _filename )
{
	// Open the output file given by the user.
	std::ofstream output_file;
	output_file.open( _filename.c_str(), std::ios::app );

	// Check.
	if( !output_file )
	{
		cout << " output file cannot be opened." << endl;
		exit(1);
	}

	output_file.close();

	output = _filename;
}

//========================================//

void Settings::set_lpsol( string _strsol )
{
	// Variables declaration.
	char qsopt[] = "qsopt";
	char lp_solve[] = "lp_solve";
	char soplex[] = "soplex";
	char glpk[] = "glpk";

	if( strcmp( _strsol.c_str(), qsopt ) == 0 )
	{
		lp_solver = PSA_QSOPT;
	}
	else if( strcmp( _strsol.c_str(), lp_solve ) == 0 )
	{
		lp_solver = PSA_LPSOLVE;
	}
	else if( strcmp( _strsol.c_str(), soplex ) == 0 )
	{
		lp_solver = PSA_SOPLEX;
	}
	else if( strcmp( _strsol.c_str(), glpk ) == 0 )
	{
		lp_solver = PSA_GLPK ;
	}
	else
	{
		cout << endl << " wrong argument: lpsol = " << _strsol << ". It should be one of the following: glpk (default), qsopt, lp_solve (in progress), soplex (in progress). Try 'psailp.exe -help' for more information on commands." << endl;
		exit(0);
	}
}

//========================================//

void Settings::set_gap( string _strgap )
{
	// Variables declaration.
	double _gap;

	// string to double.
	std::stringstream( _strgap.c_str() ) >> _gap;

	// Check.
	if( _gap < 0.0 || _gap > 1.0 )
	{
		cout << endl << " wrong argument: gap = " << _strgap << ". It should be a double in [0.0,1.0]. Try 'psailp.exe -help' for more information on commands." << endl;
		exit(0);
	}

	gap = _gap;
}

//========================================//

void Settings::set_cuts( string _strcut )
{
	// Variables declaration.
	char on[] = "on";
	char off[] = "off";

	if( strcmp( _strcut.c_str(), on ) == 0 )
	{
		cuts = true;
	}
	else if( strcmp( _strcut.c_str(), off ) == 0 )
	{
		cuts = false;
	}
	else
	{
		cout << endl << " wrong argument: cuts = " << _strcut << ". It should be either on or off. Try 'psailp.exe -help' for more information on commands." << endl;
		exit(0);
	}
}

//========================================//

void Settings::set_heuristics( string _strheu )
{
	// Variables declaration.
	char on[] = "on";
	char off[] = "off";

	if( strcmp( _strheu.c_str(), on ) == 0 )
	{
		heuristics = true;
	}
	else if( strcmp( _strheu.c_str(), off ) == 0 )
	{
		heuristics = false;
	}
	else
	{
		cout << endl << " wrong argument: heu = " << _strheu << ". It should be either on or off. Try 'psailp.exe -help' for more information on commands." << endl;
		exit(0);
	}
}

//========================================//

void Settings::set_multisol( string _strmsol )
{
	// Variables declaration.
	char on[] = "on";
	char off[] = "off";

	if( strcmp( _strmsol.c_str(), on ) == 0 )
	{
		multisol = true;
	}
	else if( strcmp( _strmsol.c_str(), off ) == 0 )
	{
		multisol = false;
	}
	else
	{
		cout << endl << " wrong argument: multisol = " << _strmsol << ". It should be either on or off. Try 'psailp.exe -help' for more information on commands." << endl;
		exit(0);
	}
}

//========================================//
///////////////// GETTERS //////////////////
//========================================//

string Settings::get_output() const
{
	return output;
}

//========================================//

int Settings::get_lpsol() const
{
	return lp_solver;
}

//========================================//

double Settings::get_gap() const
{
	return gap;
}

//========================================//

bool Settings::get_multisol() const
{
	return multisol;
}