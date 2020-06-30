#include "processor.h"
#include "linux_parser.h"

// TODO: Return the aggregate CPU utilization
float Processor::Utilization() 
{   
    // Calculate the value in %
    long idleJiffies =  LinuxParser::IdleJiffies() ; // Return idle jiffies
    long totalJiffies = LinuxParser::Jiffies(); // Return total jiffies
    
    // Calculate the value difference from the last or previous value
    float TotalDiff = totalJiffies - m_PrevTotal;
    float IdleDiff = idleJiffies - m_PrevIdle;

    // Set the previous values to current value, to be used in next cycle
    // Values are 1 sec old, since sleep of 1 sec
    m_PrevTotal = totalJiffies;
    m_PrevIdle = idleJiffies;

    // Calculate the % value. Only a factor, so a conversion of hz is not required
    return (TotalDiff- IdleDiff)/TotalDiff;
}