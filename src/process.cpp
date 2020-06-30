#include <unistd.h>
#include <cctype>
#include <sstream>
#include <string>
#include <vector>

#include "process.h"
#include "linux_parser.h"

using std::string;
using std::to_string;
using std::vector;

// TODO: Return this process's ID
int Process::Pid() 
{ 
    return processId;
}

// sets the processId member variable
void Process::Pid(int idx)
{
    processId = idx;
}

// TODO: Return this process's CPU utilization
float Process::CpuUtilization() 
{
    // Once all the values are obtained, calculate cpu in %
    float totalTime = LinuxParser::ActiveJiffies(Pid());
    float cpuVal;
    // To avoid div. by zero in case of zero uptime of new prcocesses 
    if (UpTime()>0)
    {
        cpuVal = totalTime/sysconf(_SC_CLK_TCK)/UpTime() ;   
    }
    else
    {
        cpuVal = 0;
    }
    return cpuVal;
}

// set the cpu member variable
void Process::CpuUtilization(float cpuVal) 
{
    cpuValue = cpuVal;
}

// TODO: Return the command that generated this process
string Process::Command() 
{ 
    return LinuxParser::Command(Pid()); 
}


// TODO: Return this process's memory utilization
string Process::Ram() 
{ 
    return LinuxParser::Ram(Pid()); ; 
}

// set the ram value member variable in ints
void Process::Ram(std::string ram)
{
    std::istringstream stream(ram);
    if (stream)
    {
        stream >> ramValue;
    }   
}

// TODO: Return the user (name) that generated this process
string Process::User() 
{ 
    return LinuxParser::User(Pid()); 
}

// TODO: Return the age of this process (in seconds)
long int Process::UpTime() 
{ 
    return LinuxParser::UpTime(Pid());  
}

// TODO: Overload the "less than" comparison operator for Process objects
// Sort the process based on ram/cpu consumption in decreasing order
bool Process::operator<(Process const& a) const 
{ 
    return (cpuValue > a.cpuValue) ? true : false;
}