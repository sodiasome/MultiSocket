#pragma once
class IniConfig
{
public:
	IniConfig();
	~IniConfig();

	CString m_strPath;
	CString GatExePath();

	void WriteIniString(CString strSection, CString strKey, CString strValue);
	CString ReadIniString(CString strSection,CString strKey,CString strDefaultValue);
};

