
#include "UserInterface.h"

using std::cout;
using std::endl;
//using std::string;

//========================================//
//////////////// FUCNTIONS /////////////////
//========================================//

void display_header()
{
	const char* header =
	" \n\n"
	" ******************************************************** \n\n"
	" Welcome to PSA-ilp(R) Optimizer version 0.1 \n\n"
	" Type 'psailp.exe -help' for more information on commands \n\n"
	" ******************************************************** \n\n\n"
	;

	cout << header;
}

//========================================//

void display_help()
{
	const char* help =
	" \n\n"
	" ******************************************************** \n\n"
	" Welcome to PSA-ilp(R) Optimizer version 0.1 \n\n"
	" ******************************************************** \n\n\n"
	" USAGE: psailp.exe [option1 option2 ...] path/to/dat/file \n\n"
	"           +-----------+-------------------+ \n"
    "           | OPTIONS:  | VALUES:           | \n"
    "           +-----------+-------------------+ \n"
    "           | -gap      | double in [0,1]   | TODO \n"
    "           +-----------+-------------------+ \n"
    "           | -heu      | on|off            | TODO \n"
    "           +-----------+-------------------+ \n"
    "           | -cuts     | on|off            | TODO \n"
    "           +-----------+-------------------+ \n"
    "           | -out      | path/to/txt/file  | \n"
    "           +-----------+-------------------+ \n"
    "           | -lpsol    | glpk|qsopt|soplex | \n"
	"           +-----------+-------------------+ \n"
    "           | -msol     | on|off            | \n"
    "           +-----------+-------------------+ \n\n"
    " DEFAULT SETTINGS: -gap 0.0  -heu off  -cuts off  -msol off -lpsol glpk \n\n\n"
    " ******************************************************** \n\n"
    " Thank you for using PSA-ilp(R) \n\n"
    " Visit <www.psa-ilp.com> for further information \n\n"
    ;

    cout << help;
}

//========================================//

void display_footer()
{
	const char* footer =
	" \n\n"
	" ******************************************************** \n\n"
	" Thank you for using PSA-ilp(R) \n\n"
	" Visit <www.psa-ilp.com> for further information \n\n"
	;

	cout << footer;
}

