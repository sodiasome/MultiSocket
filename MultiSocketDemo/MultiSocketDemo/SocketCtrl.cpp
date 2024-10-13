#include "pch.h"
#include "SocketCtrl.h"
#include <afxwin.h>
#include "SysTimeStamp.h"
#include <vcruntime_string.h>




HWND m_hParentWnd;

SocketCtrl::SocketCtrl()
{
}

SocketCtrl::~SocketCtrl()
{
	WSACleanup();
}

void SocketCtrl::InitServer(HWND hParentWnd,CString strIp,int nPort)
{
	ClearRecvBuff();

	//server socket 初始化

	//绑定回调函数

	m_hParentWnd = hParentWnd;
	m_strServerIp = strIp;
	m_nServerPort = nPort;
	//初始化Winsock
	WSADATA wsaData;
	WSAStartup(MAKEWORD(2, 2), &wsaData);

	//创建socket
	m_sockServer = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
	
	//绑定socket
	sockaddr_in sockAddr;
	memset(&sockAddr, 0, sizeof(sockAddr));						//每个字节都用0填充
	sockAddr.sin_family = PF_INET;												//使用IPv4地址
	//sockAddr.sin_addr.s_addr = inet_pton((LPSTR)(LPCTSTR)m_strServerIp);		//具体的IP地址
	InetPton(PF_INET,m_strServerIp,&sockAddr.sin_addr);
	sockAddr.sin_port = htons(m_nServerPort);							//端口
	bind(m_sockServer, (SOCKADDR*)&sockAddr, sizeof(SOCKADDR));

	//监听客户端socket
	listen(m_sockServer, 20);

	//接受客户端连接请求
	AfxBeginThread(LoopReadBuff, this);
	
}

void SocketCtrl::ReceiveAccept()
{
	SOCKADDR clntAddr;
	int nSize = sizeof(SOCKADDR);
	m_sockListen = accept(m_sockServer, (SOCKADDR*)&clntAddr, &nSize);

	
	CString strDefaultMsg = _T("server:Welcome to connect !");
	int resSend = send(m_sockListen, (LPSTR)(LPCTSTR)strDefaultMsg, strDefaultMsg.GetLength(), 0);

	while (true)
	{
		if (resSend != strDefaultMsg.GetLength())	//发送的长度与需要发送的长度不等
		{
			break;
		}

		int recvLen = recv(m_sockListen, m_szRecvBuff, 256, 0);
		if (recvLen < 0 || recvLen == 0)
			continue;

		
		CString strTimeStamp;
		strTimeStamp.Format(_T("时间戳(纳秒):%lld"), m_sysTimeStamp.GetNowTime());
		//AfxMessageBox(strTimeStamp);


		//发送消息
		PostMessage(m_hParentWnd, WM_RECVSOCKDATA, TRUE, (LPARAM)&m_szRecvBuff);
	}
}

void SocketCtrl::SendMsg(CString strMsg)
{
	if (m_nType == TCP_CLIENT)
	{
		USES_CONVERSION;
		char* pFileName = T2A(strMsg);
		int nRet = send(m_sockClient, pFileName, (int)strlen(pFileName), 0);
		if (nRet == SOCKET_ERROR) {
			AfxMessageBox(_T("数据发送失败"));
			return;
		}
	}
}

void SocketCtrl::InitClient(CString strServerIp,CString strPort)
{
	m_bConnectServ = FALSE;
	WSADATA wsadata;
	if (0 != WSAStartup(MAKEWORD(2, 2), &wsadata))
	{
		AfxMessageBox(_T("嵌套字打开失败"));
		return;
	}
	m_sockClient = socket(AF_INET, SOCK_STREAM, 0);
	if (m_sockClient < 0)
	{
		AfxMessageBox(_T("套接字创建失败"));
		return;
	}

	SOCKADDR_IN client_in;
	//client_in.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");//将网络地址字符串转换成二进制形式
	InetPton(AF_INET, strServerIp, &client_in.sin_addr);
	client_in.sin_family = AF_INET;
	client_in.sin_port = htons(_ttoi(strPort));

	if (connect(m_sockClient, (SOCKADDR*)&client_in, sizeof(SOCKADDR)) == SOCKET_ERROR)
	{
		AfxMessageBox(_T("套接字连接失败"));
		return;
	}
	m_bConnectServ = TRUE;
}

UINT SocketCtrl::LoopReadBuff(LPVOID pParam)
{
	SocketCtrl* pDlg = (SocketCtrl*)pParam;
	pDlg->ReceiveAccept();
	return 1;
}

void SocketCtrl::ClearRecvBuff()
{
	memset(m_szRecvBuff, 0, 256);
}
