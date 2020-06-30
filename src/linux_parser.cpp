#include <dirent.h>
#include <unistd.h>
#include <string>
#include <vector>
#include <numeric>

#include "linux_parser.h"

using std::stof;
using std::string;
using std::to_string;
using std::vector;

// DONE: An example of how to read data from the filesystem
string LinuxParser::OperatingSystem() {
  string line;
  string key;
  string value;
  std::ifstream filestream(kOSPath);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ' ', '_');
      std::replace(line.begin(), line.end(), '=', ' ');
      std::replace(line.begin(), line.end(), '"', ' ');
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "PRETTY_NAME") {
          std::replace(value.begin(), value.end(), '_', ' ');
          return value;
        }
      }
    }
  }
  return value;
}

// DONE: An example of how to read data from the filesystem
string LinuxParser::Kernel() {
  string os, version, kernel;
  string line;
  std::ifstream stream(kProcDirectory + kVersionFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> os >> version >> kernel;
  }
  return kernel;
}

// BONUS: Update this to use std::filesystem
vector<int> LinuxParser::Pids() {
  vector<int> pids;
  DIR* directory = opendir(kProcDirectory.c_str());
  struct dirent* file;
  while ((file = readdir(directory)) != nullptr) {
    // Is this a directory?
    if (file->d_type == DT_DIR) {
      // Is every character of the name a digit?
      string filename(file->d_name);
      if (std::all_of(filename.begin(), filename.end(), isdigit)) {
        int pid = stoi(filename);
        pids.push_back(pid);
      }
    }
  }
  closedir(directory);
  return pids;
}

// TODO: Read and return the system memory utilization
float LinuxParser::MemoryUtilization() 
{ 
  std::string currentLine, memKey;
  float memValue, totalMem, freeMem;
  bool totalMemFound = false;
  bool freeMemFound = false;
  std::ifstream filestream(kProcDirectory + kMeminfoFilename); 
  if (filestream.is_open())
  {
    while(std::getline(filestream,currentLine))
    {
      std::replace(currentLine.begin(), currentLine.end(),':',' ');
      std::istringstream stream(currentLine);
      while(stream >> memKey >> memValue)
      {
        if(memKey == "MemTotal")
        {
            totalMem = memValue;
            totalMemFound = true;
        }
        if(memKey == "MemFree")
        {
            freeMem = memValue;
            freeMemFound = true;
        } 
        if (totalMemFound && freeMemFound)
        {
          return (totalMem-freeMem)/totalMem;
        }
      }
    }
  }
  return 0.0; // If the values are not found, send default of 0.0
}

// TODO: Read and return the system uptime
long LinuxParser::UpTime() 
{
  std::string currentLine;
  long upTimeValue;
  std::ifstream filestream(kProcDirectory + kUptimeFilename); 
  if (filestream.is_open())
  {
    std::getline(filestream,currentLine);
    std::istringstream stream(currentLine);
    stream >> upTimeValue;
    return upTimeValue;
  }
  return 0;
} 


// TODO: Read and return the number of jiffies for the system
long LinuxParser::Jiffies() 
{ 
  long totalJiffies = LinuxParser::ActiveJiffies() + LinuxParser::IdleJiffies();
  return totalJiffies;
}

// TODO: Read and return the number of active jiffies for a PID
long LinuxParser::ActiveJiffies(int pid) 
{
  // Use of function overloading to obtain the cpu utilization for process
    std::vector<std::string> cpuUtilData = LinuxParser::CpuUtilization(pid); // This returns all the relevant data for calculation

    // Change the string to ints again
    std::vector<float> cpuUtil{};
    for (std::size_t i = 0; i< cpuUtilData.size();i++)
    {
      cpuUtil.push_back(std::stof(cpuUtilData[i]));
    }
    // Calculate sum of all jiffies to get active jiffies
    long activeJiffies = std::accumulate(cpuUtil.begin(), cpuUtil.end(),0); 
    return activeJiffies;
}

// TODO: Read and return the number of active jiffies for the system
long LinuxParser::ActiveJiffies() 
{
  std::vector<std::string> cpuUtil = LinuxParser::CpuUtilization();
  long activeJiffies = std::stof(cpuUtil[CPUStates::kUser_]) + std::stof(cpuUtil[CPUStates::kNice_]) + std::stof(cpuUtil[CPUStates::kSystem_]) + std::stof(cpuUtil[CPUStates::kIRQ_]) + std::stof(cpuUtil[CPUStates::kSoftIRQ_]) + std::stof(cpuUtil[CPUStates::kSteal_]); // Total Nonidle = user + nice + system + irq + softirq + steal
  return activeJiffies; 
}

// TODO: Read and return the number of idle jiffies for the system
long LinuxParser::IdleJiffies() 
{ 
  std::vector<std::string> cpuUtil = LinuxParser::CpuUtilization();
  long idleJiffies = std::stof(cpuUtil[CPUStates::kIdle_]) + std::stof(cpuUtil[CPUStates::kIOwait_]); // Total Nonidle = user + nice + system + irq + softirq + steal
  return idleJiffies; 
}

// TODO: Read and return CPU utilization
vector<string> LinuxParser::CpuUtilization() 
{ 
  string currentLine, cpuKey;
  int cpuValue;
  std::vector<string> cpuUtilVector{};
  std::ifstream filestream(kProcDirectory+kStatFilename);
  if (filestream.is_open())
  {
    while(std::getline(filestream,currentLine))
    {
      std::istringstream stream(currentLine);
      if (stream >> cpuKey && cpuKey =="cpu")
      {
        while (stream>>cpuValue)
        {
          cpuUtilVector.push_back(std::to_string(cpuValue));
        }
        return cpuUtilVector;
      }
    }
  } 
  return cpuUtilVector;
}

vector<string> LinuxParser::CpuUtilization(int pid) 
{ 
  std::vector<string> cpuUtil{"0","0","0","0"}, allData{}; // Initialize the util vector to zero cpu times and update based on data read
  string currentLine, currentString;

  std::ifstream filestream(kProcDirectory + std::to_string(pid)+ kStatFilename); 
  if (std::getline(filestream,currentLine))
  {
    std::istringstream stream(currentLine);
    while(stream >> currentString)
    {
      allData.push_back(currentString);
    }
    cpuUtil = {allData[13],allData[14],allData[15],allData[16]};
  }
  return cpuUtil;
}

// TODO: Read and return the total number of processes
int LinuxParser::TotalProcesses() 
{ 
  std::string currentLine, processKey;
  int processValue;
  std::ifstream filestream(kProcDirectory + kStatFilename); 
  if (filestream.is_open())
  {
    while(std::getline(filestream,currentLine))
    {
      std::istringstream stream(currentLine);
      if(stream >> processKey >> processValue) // check if the stream works
      {
        if(processKey == "processes")
        {
            return processValue;
        }
      }
    }
  }
  return 0;
}

// TODO: Read and return the number of running processes
int LinuxParser::RunningProcesses() 
{ 
  std::string currentLine, processRunningKey;
  int processRunningValue;
  std::ifstream filestream(kProcDirectory + kStatFilename); 
  if (filestream.is_open())
  {
    while(std::getline(filestream,currentLine))
    {
      std::istringstream stream(currentLine);
      if (stream >> processRunningKey >> processRunningValue) // check if the stream works
      {
        if(processRunningKey == "procs_running")
        {
            return processRunningValue;
        }
      }
    }
  }
  return 0;
}

// TODO: Read and return the command associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Command(int pid) 
{
  std::string currentLine;
  std::ifstream filestream(kProcDirectory + std::to_string(pid)+ kCmdlineFilename); 
  if (std::getline(filestream,currentLine))
  {
    return currentLine;
  }
  return string(); 
}

// TODO: Read and return the memory used by a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Ram(int pid) 
{ 
  std::string currentLine, ramKey;
  int ramValue;
  std::ifstream filestream(kProcDirectory + std::to_string(pid)+ kStatusFilename); 
  while (std::getline(filestream,currentLine))
  {
    std::replace(currentLine.begin(),currentLine.end(),':',' ');
    std::istringstream stream(currentLine);
    if(stream >> ramKey && ramKey == "VmSize")
    {
      if(stream >> ramValue)
      {
        return std::to_string(ramValue/1000); // convert to MB
      }
    }
  }
  return std::to_string(0); // return 0 as string in case nothing is found, so that the process can be sought based on ram size
}

// TODO: Read and return the user ID associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Uid(int pid) 
{
  std::string currentLine, uidKey;
  int uidValue;
  std::ifstream filestream(kProcDirectory + std::to_string(pid)+ kStatusFilename); 
  while (std::getline(filestream,currentLine))
  {
    std::replace(currentLine.begin(),currentLine.end(),':',' ');
    std::istringstream stream(currentLine);
    if(stream >> uidKey && uidKey == "Uid")
    {
      if(stream >> uidValue)
      {
        return std::to_string(uidValue);
      }
    }
  }
  return string(); 
}

// TODO: Read and return the user associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::User(int pid) 
{ 
  std::string currentLine,usernameKey,uidValue;
  char xCharInFile;
  int usernameValue;
  // Read the username from the file that matches the uidValue
  std::ifstream filestream(kPasswordPath); 
  while (std::getline(filestream,currentLine))
  {
    std::replace(currentLine.begin(),currentLine.end(),':',' ');
    std::istringstream stream(currentLine);
    if(stream >> usernameKey >> xCharInFile >> usernameValue && std::to_string(usernameValue) == LinuxParser::Uid(pid))
    {
      return usernameKey;
    }
  }
  return string(); 
}

// TODO: Read and return the uptime of a process
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::UpTime(int pid) 
{ 
  // Read everything as string, later convert to type int
  std::string currentLine, startTimeString;
  int startTimeValueCounter = 0;
  long startTimeValue;
  std::ifstream filestream(kProcDirectory + std::to_string(pid)+ kStatFilename); 
  if (std::getline(filestream,currentLine))
  {
    std::istringstream stream(currentLine);
    while(stream >> startTimeString)
    {
      if (startTimeValueCounter==21)
      {
        std::istringstream streamStartTime(startTimeString);
        streamStartTime >> startTimeValue;
        // Once the starttime is available, calculate the uptime by subtracting it from system uptime
        return LinuxParser::UpTime() - startTimeValue/sysconf(_SC_CLK_TCK);
      }
      startTimeValueCounter ++;
    }
  }
  return 0; 
}