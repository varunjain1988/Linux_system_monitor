#include <string>

#include "format.h"

using std::string;

// TODO: Complete this helper function
// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
// REMOVE: [[maybe_unused]] once you define the function
string Format::ElapsedTime(long timeInSeconds) 
{ 
    std::string formattedTime, hours, minutes,seconds ;
    int hoursInSeconds = timeInSeconds/3600;
    int minutesInSeconds = (timeInSeconds - hoursInSeconds*3600)/60;
    int secondsLeft = (timeInSeconds - hoursInSeconds*3600- minutesInSeconds*60);
    if (hoursInSeconds <= 9)
    {
        hours = "0"+std::to_string(hoursInSeconds);
    }
    else
    {
        hours = std::to_string(hoursInSeconds);
    }
    

    if (minutesInSeconds <= 9)
    {
        minutes = "0"+std::to_string(minutesInSeconds);
    }
    else
    {
        minutes = std::to_string(minutesInSeconds);
    }

    if (secondsLeft <= 9)
    {
        seconds = "0"+std::to_string(secondsLeft);
    }
    else
    {
        seconds = std::to_string(secondsLeft);
    }
    
    return hours + ":" + minutes + ":" + seconds;

}