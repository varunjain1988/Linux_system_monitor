#ifndef PROCESS_H
#define PROCESS_H

#include <string>
/*
Basic class for Process representation
It contains relevant attributes as shown below
*/
class Process {
 public:
  int Pid();                               // TODO: See src/process.cpp
  void Pid(int);                           // set function to set the process ID member variable
  std::string User();                      // TODO: See src/process.cpp
  std::string Command();                   // TODO: See src/process.cpp
  float CpuUtilization();                  // TODO: See src/process.cpp
  void CpuUtilization(float);              // set function to set the cpuValue member variable
  std::string Ram();                       // TODO: See src/process.cpp
  void Ram(std::string);                   // set function to set the ramValue member variable
  long int UpTime();                       // TODO: See src/process.cpp
  bool operator<(Process const& a) const;  // TODO: See src/process.cpp

  // TODO: Declare any necessary private members
 private:
    int processId;  // define to sort as per processId
    int ramValue;   // define to sort as per ramValue
    float cpuValue; // define to sort as per ramValue
};

#endif