#pragma once
/*
	std::chrono ��ȷ������ʱ���
*/
#include <chrono> 
#include <string>

using namespace std;
using namespace std::chrono;

class SysTimeStamp
{
private:
	system_clock::time_point m_tpNow;//��ǰʱ��� ΢��
public:
	long GetNowTime();
	string GetFormatNowTime();
};

