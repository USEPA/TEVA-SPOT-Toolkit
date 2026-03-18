
#include <string>

#define TEVA_SPOT_TOOLKIT_VERSION "2.1" 

inline std::string create_version(const char* command, const char* date, const char* time)
{
std::string version;
version += command;
version += " (TEVA-SPOT Toolkit) ";
version += TEVA_SPOT_TOOLKIT_VERSION;
version += "\n";
version += "  Compiled ";
version += date;
version += " ";
version += time;
version += "\n\n\
Copyright (2008) Sandia Corporation. Under the terms of Contract DE-AC04-       94AL85000 with Sandia Corporation, the U.S. Government retains certain rights   in this software.";
return version;
}
