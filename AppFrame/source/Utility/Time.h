#pragma once
class TimeClass
{
public:
	TimeClass();
	~TimeClass();
	void Process();
	int GetTime();
	int GetTimeSeconds();
	int GetTimeMinute();
	int WaitTimer(int waittime);
private:
	int fps;
	int frametime;
	int seconds;
	int minutes;
	int hour;
	int oldframe;
	bool trg;
};

