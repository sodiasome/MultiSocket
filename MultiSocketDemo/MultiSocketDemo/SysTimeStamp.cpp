#include "pch.h"
#include "SysTimeStamp.h"


long SysTimeStamp::GetNowTime()
{
	m_tpNow = system_clock::now();
	long lNow = duration_cast<nanoseconds>(m_tpNow.time_since_epoch()).count();
	return lNow;
}

string SysTimeStamp::GetFormatNowTime()
{
	// if m_tpNow == nullptr
	time_t tmTmp = system_clock::to_time_t(m_tpNow);
	if (!tmTmp)
	{
		GetNowTime();
	}
	
	char str[64] = {0};
	struct tm tmInfo;
	localtime_s(&tmInfo, &tmTmp);
	sprintf_s(str, "%04d-%02d-%02d %02d:%02d:%02d", 
		tmInfo.tm_year+1900, 
		tmInfo.tm_mon+1, 
		tmInfo.tm_mday, 
		tmInfo.tm_hour, 
		tmInfo.tm_min, 
		tmInfo.tm_sec);
	
	//char str2[64] = { 0 };
	//ctime_s(str2, sizeof(str), &tmTmp);
	string strFormatTime(str);
	return strFormatTime;
}
