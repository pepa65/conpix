// Logger.h
// Copyright: rofl0r  License: GNU GPLv3

#ifndef LOGGER_H_
#define LOGGER_H_

#include <string>
#include <fstream>

#include "Interfaces.h"


DeclareInterface(ILogger)
	virtual void log(std::string s);
	virtual void logln(std::string s);
EndInterface

class SimpleConsoleLogger : implements ILogger {
	virtual void log(std::string s);
	virtual void logln(std::string s);
};

class SimpleFileLogger : implements ILogger {
private:
	std::string outfile;
	std::ofstream myfile;
public:
	SimpleFileLogger(std::string out_file);

	virtual ~SimpleFileLogger();
	virtual void log(std::string s);
	virtual void logln(std::string s);
	virtual void clear();
};

#endif // LOGGER_H_
