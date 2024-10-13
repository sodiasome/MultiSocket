
// ScocketClientDlg.cpp: 实现文件
//

#include "pch.h"
#include "framework.h"
#include "ScocketClient.h"
#include "ScocketClientDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CScocketClientDlg 对话框



CScocketClientDlg::CScocketClientDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_SCOCKETCLIENT_DIALOG, pParent)
	, m_strServerIp(_T(""))
	, m_strServerPort(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CScocketClientDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_ED_IP, m_strServerIp);
	DDX_Text(pDX, IDC_ED_PORT, m_strServerPort);
}

BEGIN_MESSAGE_MAP(CScocketClientDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BTN_CONNECTED, &CScocketClientDlg::OnBnClickedBtnConnected)
	ON_BN_CLICKED(IDC_BTN_SEND, &CScocketClientDlg::OnBnClickedBtnSend)
END_MESSAGE_MAP()


// CScocketClientDlg 消息处理程序

BOOL CScocketClientDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	m_strServerIp = g_iniCfg.ReadIniString(_T("Tcp"),_T("ServerIp"),_T("127.0.0.1"));
	m_strServerPort = g_iniCfg.ReadIniString(_T("Tcp"), _T("ServerPort"), _T("9001"));

	UpdateData(FALSE);

	return TRUE;  
}

void CScocketClientDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}


void CScocketClientDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CScocketClientDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CScocketClientDlg::OnBnClickedBtnConnected()
{
	m_clientCtrl.m_nType = 0;
	m_clientCtrl.InitClient(m_strServerIp,m_strServerPort);
}


void CScocketClientDlg::OnBnClickedBtnSend()
{
	if (!m_clientCtrl.m_bConnectServ)
	{
		AfxMessageBox(_T("服务器未连接"));
		return;
	}
	CString strSend;
	GetDlgItemText(IDC_ED_SENDMSG, strSend);
	if (!strSend.IsEmpty())
	{
		m_clientCtrl.SendMsg(strSend);
	}
	
}



void CScocketClientDlg::OnCancel()
{
	g_iniCfg.WriteIniString(_T("Tcp"), _T("ServerIp"), m_strServerIp);
	g_iniCfg.WriteIniString(_T("Tcp"), _T("ServerPort"), m_strServerPort);


	CDialogEx::OnCancel();
}
