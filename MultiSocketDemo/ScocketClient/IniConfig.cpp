#include "pch.h"
#include "IniConfig.h"

IniConfig::IniConfig()
{
	CString strExeForder = GatExePath();
	m_strPath = strExeForder+_T("config.ini");
}
IniConfig::~IniConfig()
{

}

CString IniConfig::GatExePath()
{
	TCHAR buff[MAX_PATH] = { 0 };
	::GetModuleFileName(NULL, buff, MAX_PATH);
	CString strPath = buff;
	int pos = strPath.ReverseFind('\\');
	strPath = strPath.Left(pos + 1);
	return strPath;
}

void IniConfig::WriteIniString(CString strSection, CString strKey, CString strValue)
{
	::WritePrivateProfileString(strSection, strKey, strValue, m_strPath);
}

CString IniConfig::ReadIniString(CString strSection, CString strKey, CString strDefaultValue)
{
	TCHAR szbuff[MAX_PATH];
	memset(szbuff, 0, MAX_PATH);
	DWORD dwRet = ::GetPrivateProfileString(strSection, strKey, strDefaultValue, szbuff, MAX_PATH, m_strPath);
	if (0 < dwRet)
	{
		CString strValue;
		strValue.Format(_T("%s"), szbuff);
		return strValue;
	}
	return strDefaultValue;
}
