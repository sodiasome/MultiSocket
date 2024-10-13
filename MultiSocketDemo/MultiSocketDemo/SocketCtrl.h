#pragma once
/*
	socket 管理类
	描述：
		提供socket的并发连接管理
		没有文件操作，全部数据在内存处理
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
	int m_nType;	//TCP类型
	char m_szRecvBuff[256];//接收缓存
	void ClearRecvBuff();

	SysTimeStamp m_sysTimeStamp;//时间戳
	
	SOCKET m_sockServer;
	SOCKET m_sockListen;	//连接的客户端
	CString m_strServerIp;
	int		m_nServerPort;

	SOCKET m_sockClient;//客户端
	BOOL m_bConnectServ;//是否连接服务器

};

