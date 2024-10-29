#include"Time.h"
TimeClass::TimeClass()
{
    frametime = 0;
    oldframe = 0;
    fps = 60;
    trg = false;
}

TimeClass::~TimeClass()
{
    //empty
}
 void TimeClass::Process()
 {
     frametime++;
     seconds = frametime / fps;
     minutes = frametime / fps*60;
 }
 	 
 int TimeClass::GetTime()
 {
     return frametime;
 }
 	
 int TimeClass::GetTimeSeconds()
 {
     return seconds;
 }

 int TimeClass::GetTimeMinute()
 {
     return minutes;
 }
 
 int TimeClass::WaitTimer(int waitsecond)
 {
     return 0;
 }

