// ConsoleAppCPP.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <algorithm>
#include <functional>

#include <qstd10.h>

//////////////////////////////////////////////////////////////////////////////////////////////////////
// some model to store application config. Using 'CQLoggerUserAppCfg' as base class for logger 
// configuration 

class COptions : public qstd::CQLoggerUserAppCfg
{
public:
	COptions(const qstd::qstr& sAppName) : CQLoggerUserAppCfg(sAppName), _nSomeValue(0) {

	}
	// Input File property getter and setter
	const qstd::qstr& InputFile() const { return _sInputFile; }
	qstd::qstr& InputFile() { return _sInputFile; }
	
	// Some Value property getter and setter
	const int& SomeValue() const { return _nSomeValue; }
	int& SomeValue() { return _nSomeValue; }

private:
	// some more parameter
	qstd::qstr _sInputFile;	
	int _nSomeValue;		
};

//////////////////////////////////////////////////////////////////////////////////////////////////////
// usage: dump usage string to the screen

static void usage()
{
	_tprintf(_qc("usage  : myapp [options] <input file> [Parameter 2]\n")
		_qc("options: \n")
		_qc("-value <number>    some value\n")
		_qc("-l [outfile]       logfile\n")
		_qc("-ll                log level\n")
	);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////
// dump_version: read copyright notice and programm version from exe-file resource and print it
// to the screen

static void dump_version(void)
{
	qstd::CModuleVersion app;
	qchar szFilePath[_MAX_PATH];

	::GetModuleFileName(NULL, szFilePath, sizeof(szFilePath));
	if (app.Open(szFilePath)) {
		_tprintf(_qc("%s V%s - %s\n"), app.GetInternalName().c_str(), app.GetFileVersion().c_str(), app.GetFileDescription().c_str());
		_tprintf(_qc("(c) %s. All rights reserved\n"), app.GetLegalCopyright().c_str());
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////////
// Parameter error handles the case to missmíng values or other command line error
//
// 		  	sErrorMsg	Message describing the error.
// [in,out]	prc		 	store return code here.
// 		  	rcCode   	(Optional) return code default
//
// returns	false always to signal application should terminate
//

static bool paramError(const qstd::qstr& sErrorMsg, int* prc, int rcCode = 1)
{
	*prc = rcCode;
	qstd::dprintf(qstd::DbgType::XDBG_TYPE_ERROR, _qc("ERROR: %s\n"), sErrorMsg.c_str());
	return false;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////
// parseArguments: fetch the given arguments from _TCHAR* argv[] to our application config model
//
// COptions *pOpt		put the arguments here (out parameter)
// int      argc, 		count of strings in 'argv'
// _TCHAR   *argv[]		array of pointer of char (array of strings; in parameter)
// int      *prc		put the application exit code here (out parameter)
//
// return 	true = continue programm execution; false = exit with code given in *prc

static bool parseArguments(COptions* pOpt, int argc, const _TCHAR* const argv[], int* prc)
{
	*prc = 0; // be optimistic (0=no error)
	qstd::CArgvParserEx cmd(argc, (const qchar**)argv);	// setup command line parser

	if (!cmd.CheckCount(1)) {	// check count of arguments (min==1) if failed ... 
		dump_version();			// dump copyright notice
		usage();				// dump usage text
		return false;			// and exit application
	}
	// loop through all arguments 
	qstd::CArgv arg;
	for (; cmd.GetNext(&arg);) {
		if (arg.IsOptionHelp()) { // -h
			dump_version();
			usage();
			return false;
		}
		else if (arg.IsOption(_qc("v"))) {	// -v
			dump_version();
			return false;
		}
		else if (arg.IsOption(_qc("value"))) {	// -value <number>
			if (!cmd.GetOptionParameter(&pOpt->SomeValue())) return paramError(_qc("some value missing"), prc);
		}
		else if (arg.IsOption(_qc("l"))) {	// -l [filename]
			qstd::qstr s = _qc("myapp.log");
			cmd.GetOptionParameter(&(s));
			pOpt->Logfile(s); 				// take it to the model			
		}
		else if (arg.IsOption(_qc("ll"))) { // -ll <level>
			qstd::qstr level;
			if (!cmd.GetOptionParameter(&level)) return paramError(_qc("log level missing"), prc);
			pOpt->Level(qstd::MakeLevel(level));	// take it to the model			
		}
		else if (arg.ParameterNo() == 1) {			// not an option than the found the first parameter
			pOpt->InputFile() = arg.Get();			// take it to the model
		}
		else {
			return paramError(_qc("unknown argument"),prc,2);
		}
	}
	return true;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////
// main function with command line arguments

int main(int argc, _TCHAR* argv[])
{
	qstd::DbgConsoleAppOutputMode();		// initialize dprintf for additional console output

	COptions cfg(_qc("ConsoleAppCPP"));		// instance of our config model
	cfg.ConsoleOutput(true);				// tell the logger we are a console app

	// fetch arguments from the command line to our model
	int rc = 0;
	if (!parseArguments(&cfg, argc, argv, &rc)) return rc;

	qstd::SetupAppLogger(&cfg);		// config the logger module

	CoInitialize(0);				// initialize COM (if used)

	/* ... run your code here ...
	*/

	CoUninitialize();				// release COM
	return rc;
}
