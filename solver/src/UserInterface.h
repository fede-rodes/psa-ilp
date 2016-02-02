/**
 * @file   UserInterface.h
 * @brief  Bunch of functions used to display information to the user.
 * @author Federico Rodes
 */

#ifndef USERINTERFACE_H_
#define USERINTERFACE_H_

#include <iostream> // std::cout, std::cerr, std::endl
//#include <string>   // std::to_string

//FUNCTIONS:
/**
 * Displays on screen the header information when running psa-ilp.
 */
void display_header();

/**
 * Displays on screen the help information when typing 'psailp.exe --help' on command line.
 */
void display_help();

/**
 * Displays on screen the footer information once psa-ilp has finished.
 */
void display_footer();

#endif /* USERINTERFACE_H_ */
