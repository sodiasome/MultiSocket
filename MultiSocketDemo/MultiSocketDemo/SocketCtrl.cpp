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

	//server socket ��ʼ��

	//�󶨻ص�����

	m_hParentWnd = hParentWnd;
	m_strServerIp = strIp;
	m_nServerPort = nPort;
	//��ʼ��Winsock
	WSADATA wsaData;
	WSAStartup(MAKEWORD(2, 2), &wsaData);

	//����socket
	m_sockServer = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
	
	//��socket
	sockaddr_in sockAddr;
	memset(&sockAddr, 0, sizeof(sockAddr));						//ÿ���ֽڶ���0���
	sockAddr.sin_family = PF_INET;												//ʹ��IPv4��ַ
	//sockAddr.sin_addr.s_addr = inet_pton((LPSTR)(LPCTSTR)m_strServerIp);		//�����IP��ַ
	InetPton(PF_INET,m_strServerIp,&sockAddr.sin_addr);
	sockAddr.sin_port = htons(m_nServerPort);							//�˿�
	bind(m_sockServer, (SOCKADDR*)&sockAddr, sizeof(SOCKADDR));

	//�����ͻ���socket
	listen(m_sockServer, 20);

	//���ܿͻ�����������
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
		if (resSend != strDefaultMsg.GetLength())	//���͵ĳ�������Ҫ���͵ĳ��Ȳ���
		{
			break;
		}

		int recvLen = recv(m_sockListen, m_szRecvBuff, 256, 0);
		if (recvLen < 0 || recvLen == 0)
			continue;

		
		CString strTimeStamp;
		strTimeStamp.Format(_T("ʱ���(����):%lld"), m_sysTimeStamp.GetNowTime());
		//AfxMessageBox(strTimeStamp);


		//������Ϣ
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
			AfxMessageBox(_T("���ݷ���ʧ��"));
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
		AfxMessageBox(_T("Ƕ���ִ�ʧ��"));
		return;
	}
	m_sockClient = socket(AF_INET, SOCK_STREAM, 0);
	if (m_sockClient < 0)
	{
		AfxMessageBox(_T("�׽��ִ���ʧ��"));
		return;
	}

	SOCKADDR_IN client_in;
	//client_in.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");//�������ַ�ַ���ת���ɶ�������ʽ
	InetPton(AF_INET, strServerIp, &client_in.sin_addr);
	client_in.sin_family = AF_INET;
	client_in.sin_port = htons(_ttoi(strPort));

	if (connect(m_sockClient, (SOCKADDR*)&client_in, sizeof(SOCKADDR)) == SOCKET_ERROR)
	{
		AfxMessageBox(_T("�׽�������ʧ��"));
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
