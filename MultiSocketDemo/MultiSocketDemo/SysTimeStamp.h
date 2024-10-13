#pragma once
/*
	std::chrono 精确到纳秒时间戳
*/
#include <chrono> 
#include <string>

using namespace std;
using namespace std::chrono;

class SysTimeStamp
{
private:
	system_clock::time_point m_tpNow;//当前时间戳 微秒
public:
	long GetNowTime();
	string GetFormatNowTime();
};

