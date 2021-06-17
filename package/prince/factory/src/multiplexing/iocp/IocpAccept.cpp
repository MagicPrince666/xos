#include "stdafx.h"
#include "IocpAccept.h"
#include <assert.h>
//作者QQ13712486 擅长c++、c#开发

IocpAccept::IocpAccept()
{
	m_callback = NULL;
}


IocpAccept::~IocpAccept()
{
}

void IocpAccept::SetCallback(IAcceptCallback* callback)
{
	m_callback = callback;
}

void IocpAccept::AddListenPort(UINT16 port)
{
	std::vector<UINT16>::iterator begin = m_listListenPort.begin();
	for (;begin != m_listListenPort.end();++begin)
	{
		if (*begin == port)
			return;
	}

	m_listListenPort.push_back(port);
}

int IocpAccept::CreateListenInfo()
{
	//m_listListenPort存储要监听的端口；总个数不超过64个
	std::vector<UINT16>::iterator pos = m_listListenPort.begin();
	for (;pos != m_listListenPort.end();++pos)
	{
		//生成socket
		UINT16 listenPort = *pos;
		LISTEN_SOCKET_INFO socketInfo;
		socketInfo.listenSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
		socketInfo.listenPort = listenPort;

		//绑定端口
		sockaddr_in InetAddr;
		InetAddr.sin_family = AF_INET;
		InetAddr.sin_addr.s_addr = htonl(INADDR_ANY);
		InetAddr.sin_port = htons(listenPort);

		int ret = bind(socketInfo.listenSocket, (SOCKADDR *)&InetAddr, sizeof(InetAddr));
		if (SOCKET_ERROR == ret)
		{
			::closesocket(socketInfo.listenSocket);
			//绑定失败
			continue;
		}

		//生成事件
		socketInfo.netEvent = WSACreateEvent();

		//将socket句柄与事件关联起来。只监视socket的accept和close消息
		ret = WSAEventSelect(socketInfo.listenSocket, socketInfo.netEvent, FD_ACCEPT | FD_CLOSE);
		if (SOCKET_ERROR == ret)
		{
			::closesocket(socketInfo.listenSocket);
			continue;
		}

		// 启动监听
		ret = listen(socketInfo.listenSocket, 1000);
		if (SOCKET_ERROR == ret)
		{
			::closesocket(socketInfo.listenSocket);
			continue;
		}

		m_listListenInfo.push_back(socketInfo);
	}
	return 0;
}

BOOL IocpAccept::Start()
{
	m_bServerStart = TRUE;
	CreateListenInfo();
	if (m_listListenInfo.size() != m_listListenPort.size())
	{
		for (int i = 0; i < m_listListenInfo.size();i++)
		{
			LISTEN_SOCKET_INFO item =	m_listListenInfo[i];
			closesocket(item.listenSocket);
		}
		m_listListenInfo.clear();
		return FALSE;
	}

	//启动监听线程
	CreateThread(NULL, 0, ListenRoutine, (PVOID)this, 0, &m_dwThreadID);
	return TRUE;
}

DWORD IocpAccept::ListenRoutine(LPVOID lp_param)
{
	IocpAccept *pNetServer = (IocpAccept*)lp_param;
	pNetServer->Deal_ListenRoutine();
	return 0;
}

WSAEVENT* IocpAccept::CreateNetEventArray(int *nEventTotal)
{
	*nEventTotal = m_listListenInfo.size();
	WSAEVENT *pResult = new WSAEVENT[*nEventTotal];
	for (int i = 0; i < m_listListenInfo.size(); i++)
	{
		pResult[i] = m_listListenInfo[i].netEvent;
	}
	return pResult;
}

DWORD IocpAccept::Deal_ListenRoutine()
{
	//生成事件地址指针
	int nEventTotal;
	WSAEVENT* pEventArray = CreateNetEventArray(&nEventTotal);
	if (nEventTotal == 0)
		return 0;
	assert(nEventTotal <= WSA_MAXIMUM_WAIT_EVENTS);

	MSG msg;
	while (m_bServerStart)
	{
		// 同时对多个事件做监听
		DWORD index = WSAWaitForMultipleEvents(nEventTotal,
			pEventArray,
			FALSE,
			10000,
			FALSE);
		if (!m_bServerStart)
			return 0;

		//查看是哪个事件触发函数返回
		index = index - WSA_WAIT_EVENT_0;
		//客户端连接事件
		if ((index != WSA_WAIT_FAILED) && (index != WSA_WAIT_TIMEOUT))
		{
			//pEventArray排序与m_listListenInfo一样，所以可以根据index找到对应的socket。
			//就是该socket导致函数返回
			LISTEN_SOCKET_INFO socketInfo = m_listListenInfo[index];

			//查看具体是什么事件导致函数返回
			WSANETWORKEVENTS NetworkEvents;
			WSAEnumNetworkEvents(socketInfo.listenSocket, pEventArray[index], &NetworkEvents);

			//如果是accept事件，说明有客户端连接此端口
			if (NetworkEvents.lNetworkEvents == FD_ACCEPT
				&& NetworkEvents.iErrorCode[FD_ACCEPT_BIT] == 0)
			{
				//这时调用accept函数，会立即返回
				AcceptListenPort(socketInfo.listenSocket, socketInfo.listenPort);
			}
			if (NetworkEvents.lNetworkEvents == FD_CLOSE
				&& NetworkEvents.iErrorCode[FD_CLOSE_BIT] == 0)
			{
				assert(false);
			}

		}
		else
		{
			//因为超时等其他原因引起函数返回
		}
	}
	return 0;
}

UINT IocpAccept::AcceptListenPort(SOCKET hListenSocket, UINT16 nListenPort)
{
	SOCKET hClient = 0;
	SOCKADDR_IN localAddr;
	int iaddrSize = sizeof(SOCKADDR_IN);

	hClient = accept(hListenSocket, (struct sockaddr *)&localAddr, &iaddrSize);
	if (INVALID_SOCKET == hClient)
	{
		int nAccepetError = WSAGetLastError();
		if (nAccepetError == WSAECONNRESET)
		{
			return 1;
		}
		else
		{
			return 0;
		}
	}
	else
	{
		//获取了一个客户端连接
		OnAcceptClient(hClient, nListenPort);
	}
	return 0;
}


void IocpAccept::OnAcceptClient(SOCKET hSocketClient, UINT16 nListenPort)
{
	m_callback->OnAcceptClient(hSocketClient, nListenPort);
}


void IocpAccept::Stop()
{
	if (!m_bServerStart)
		return;
	m_bServerStart = FALSE;

	for (int i = 0; i < m_listListenInfo.size(); i++)
	{
		closesocket(m_listListenInfo[i].listenPort);
	}
}


