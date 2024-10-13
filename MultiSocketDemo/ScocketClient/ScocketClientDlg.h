
// ScocketClientDlg.h: 头文件
//

#pragma once
#include "../MultiSocketDemo/SocketCtrl.h"


// CScocketClientDlg 对话框
class CScocketClientDlg : public CDialogEx
{
// 构造
public:
	CScocketClientDlg(CWnd* pParent = nullptr);	// 标准构造函数

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_SCOCKETCLIENT_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	virtual void OnCancel();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedBtnConnected();
	afx_msg void OnBnClickedBtnSend();
public:
	CString m_strServerIp;
	CString m_strServerPort;
	BOOL m_bConnect;
	SocketCtrl m_clientCtrl;
};
