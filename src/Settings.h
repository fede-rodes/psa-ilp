/**
 * @file   Settings.h
 * @brief  solver's settings chosen by the user.
 * @author Federico Rodes
 */

#ifndef SETTINGS_H_
#define SETTINGS_H_

#include <string.h> // strcmp()
#include <string>
#include <cstdlib>  // exit()
#include <iostream> // cout, endl, cerr
#include <fstream>  // std::ifstream(), std::ofstream()
#include <sstream>  // std::stringstream()

#include "Constants.h"

using std::string;

class Settings
{
	/**
	 * Disable copy constructor given by default.
	 */
	Settings( const Settings& other ) = delete;

private:

	// FIELDS:
	string output;
	
	int lp_solver;

	double gap;

	bool cuts;
	bool heuristics;
	bool multisol;

public:

	// CONSTRUCTOR AND DESTRUCTOR:
	Settings();
	~Settings();

	// METHODS:
	void display(); // display settings.

	// SETTERS:
	void set_output( string _filename );
	void set_lpsol( string _strsol );
	void set_gap( string _strgap );
	void set_cuts( string _strcut );
	void set_heuristics( string _strheu );
	void set_multisol( string _strmsol );

	// GETTERS:
	string get_output() const;
	int    get_lpsol() const;
	double get_gap() const;
	bool   get_multisol() const;

};

#endif /* SETTINGS_H_ */
