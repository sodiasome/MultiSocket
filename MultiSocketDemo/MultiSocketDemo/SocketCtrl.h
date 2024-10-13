#pragma once
/*
	socket ������
	������
		�ṩsocket�Ĳ������ӹ���
		û���ļ�������ȫ���������ڴ洦��
*/

#include <WinSock2.h>
#include <WS2tcpip.h>
#pragma comment(lib, "Ws2_32.lib")
#include "SysTimeStamp.h"


#define WM_RECVSOCKDATA (WM_USER+100)  

class SocketCtrl
{
public:
	SocketCtrl();
	~SocketCtrl();
public:
	enum {
		TCP_CLIENT=0, 
		TCP_SERVER
	};
	void InitServer(HWND hParentWnd, CString strIp, int nPort);
	void ReceiveAccept();
	void SendMsg(CString strMsg);
	void InitClient(CString strServerIp, CString strPort);

	static UINT LoopReadBuff(LPVOID pParam);

	//void SendData();
public:
	int m_nType;	//TCP����
	char m_szRecvBuff[256];//���ջ���
	void ClearRecvBuff();

	SysTimeStamp m_sysTimeStamp;//ʱ���
	
	SOCKET m_sockServer;
	SOCKET m_sockListen;	//���ӵĿͻ���
	CString m_strServerIp;
	int		m_nServerPort;

	SOCKET m_sockClient;//�ͻ���
	BOOL m_bConnectServ;//�Ƿ����ӷ�����

};

