#ifndef __TIMER_H__
#define __TIMER_H__

typedef unsigned int uint;
typedef unsigned char uchar;
typedef unsigned __int32 uint32;
typedef unsigned __int64 uint64;

class Timer
{
public:

	// Constructor
	Timer();

	void Start();
	uint32 ReadSec() const;
	float ReadMSec() const;

	int something();

private:
	uint32 startTime;
};

#endif //__TIMER_H__