#include <stdexcept>

using namespace std;

//1.Clase PacManError
class PacManError : public logic_error
{
public:
	PacManError(const string& m) : logic_error(m) {};
};

//2.1.Clase SDLError
class SDLError : public PacManError
{
public:
	SDLError(const string& m) : PacManError("SDL Error. " + m) {};
};

//2.2.Clase FileFormatError
class FileFormatError : public PacManError
{
public:
	FileFormatError(const string& m) : PacManError("File format error. " + m) {};
};

//2.3.Clase FileNotFoundError
class FileNotFoundError : public PacManError
{
public:
	FileNotFoundError(const string& m) : PacManError("File not found. " + m) {};
};


