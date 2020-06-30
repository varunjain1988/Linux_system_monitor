#include <unistd.h>
#include <cstddef>
#include <set>
#include <string>
#include <vector>

#include "process.h"
#include "processor.h"
#include "system.h"
#include "linux_parser.h"

using std::set;
using std::size_t;
using std::string;
using std::vector;

// TODO: Return the system's CPU
Processor& System::Cpu() 
{ 
    return cpu_; 
}

// TODO: Return a container composed of the system's processes
vector<Process>& System::Processes() 
{ 
    // First get the vector of processes ids, and store them in processIds
    std::vector<int> processIds(LinuxParser::Pids());

    // For each of the process receive the required id and set it as member variable.
    // The other functions can then be called directly.

    // Clear the vector and resize this vector to zero and then add all the processes
    processes_.clear();
    processes_.resize(0);
    for (size_t i =0 ; i< processIds.size(); i++)
    {
        Process currentProcess;
        currentProcess.Pid(processIds[i]); // set the process id for process
        currentProcess.Ram(currentProcess.Ram()); // set the ram for process as member variable(used for sort function)
        currentProcess.CpuUtilization(currentProcess.CpuUtilization()); // set the cpu for process as member variable(used for sort function)      
        processes_.push_back(currentProcess); // push all the process to vector
    }
    std::sort(processes_.begin(),processes_.end(),std::less<Process>()); // sort the vectors based on either ram/cpu
    return processes_; // return the processes
}

// TODO: Return the system's kernel identifier (string)
std::string System::Kernel() 
{ 
    return LinuxParser::Kernel();
}

// TODO: Return the system's memory utilization
float System::MemoryUtilization() 
{ 
    return LinuxParser::MemoryUtilization(); 
}

// TODO: Return the operating system name
std::string System::OperatingSystem() 
{ 
    return LinuxParser::OperatingSystem(); 
}

// TODO: Return the number of processes actively running on the system
int System::RunningProcesses() 
{ 
    return LinuxParser::RunningProcesses(); 
}

// TODO: Return the total number of processes on the system
int System::TotalProcesses() 
{ 
    return LinuxParser::TotalProcesses(); 
}

// TODO: Return the number of seconds since the system started running
long int System::UpTime() 
{ 
    return LinuxParser::UpTime(); 
}