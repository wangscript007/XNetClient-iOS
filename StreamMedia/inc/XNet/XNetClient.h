//XNetClient.h
#ifndef __XNetCLIENT_H__
#define __XNetCLIENT_H__

#include "XNetSession.h"
#include "XNetDefine.h"

//����Ӧ�÷���
class XNetAPI XNetClientCallback
{
public:
	XNetClientCallback(void){};
	virtual~XNetClientCallback(void){};
public:
	//�յ�����״̬
	virtual void OnXNetSessionConnectStatus(XNetSession::CONNECT_STATUS /*cs*/,int /*nErrorCode=0*/){};

	//����������Ӧ���յ������ݰ��ص�
	virtual void OnXNetClientReceivedFromServer(const char* /*pData*/,int /*nLen*/){};
};

class XNetAPI XNetClient
{
public:
	XNetClient(void){};
	virtual~XNetClient(void){};
public:
	//���ӷ�����
	virtual int Connect(unsigned short nServerType,const char*cszIP,unsigned short usPort,const char*cszInterface)=0;
	//�Ͽ�����
	virtual void Disconnect(void)=0;

	//�������ݸ�ָ��������Server
	virtual int SendDataToServer(const char*pData,int nLen)=0;

	virtual unsigned int GetClientID(void)=0;

	//ȡ���ص�ַ
	virtual const char*GetLocalIP(void)=0;

	//ȡ������ַ
	virtual const char*GetNATIP(void)=0;

	static XNetClient*Create(XNetClientCallback&rCallback);
};

#endif
