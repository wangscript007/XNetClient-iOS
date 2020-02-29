//XNetMediaReceiver.h
#ifndef __XNetMEDIARECEIVER_H__
#define __XNetMEDIARECEIVER_H__

#include <XNet/XNetDefine.h>

#define MEDIA_RECEIVER_NOT_STARTED			0x00000001
#define MEDIA_RECEIVER_CONNECTING			0x00000002
#define MEDIA_RECEIVER_CONNECTED			0x00000004
#define MEDIA_RECEIVER_DISCONNECTED			0x00000008

//ý�������
class XNetMediaReceiverCallback
{
public:
	XNetMediaReceiverCallback(void){};
	virtual~XNetMediaReceiverCallback(void){};
public:
	/******************************************************************************
	* OnXNetMediaReceiverCallbackAudioPacket
	*���������յ���Ƶ���ݰ��Ļص�
	*���룺	pData			-���ݰ�ָ��
			nLen			-���ݰ�����
	*����� ��
	*����ֵ����
	*****************************************************************************/
	virtual void OnXNetMediaReceiverCallbackAudioPacket(unsigned char*pData,int nLen)=0;

	/******************************************************************************
	* OnXNetMediaReceiverCallbackVideoPacket
	*���������յ���Ƶ���ݰ��Ļص�
	*���룺	pData			-���ݰ�ָ��
			nLen			-���ݰ�����
	*����� ��
	*����ֵ����
	*****************************************************************************/
	virtual void OnXNetMediaReceiverCallbackVideoPacket(unsigned char*pData,int nLen)=0;
};

class XNetAPI XNetMediaReceiver
{
public:
	XNetMediaReceiver(void){};
	virtual~XNetMediaReceiver(void){};
public:
	//add by wanggj
	virtual void SetLocalMCUID(const char* cszLocalMCUID)=0;
	virtual void SetLocalMCIIP(const char* cszLocalMCUIP)=0;
	virtual void SetLocalMCUPort(unsigned short usLocalMCUPort)=0;
	//end.
	/******************************************************************************
	* Open
	*��������
	*���룺	cszPeerNodeID	-�Զ˽ڵ��ʶ
			cszPeerNATIP	-�Զ�����IP
			cszPeerLocalIP	-�Զ˱���IP
			nPeerLocalPort	-�Զ˱����˿�
			cszPeerMCUID	-�Զ�MCU��ʶ
			cszPeerMCUIP	-�Զ�MCU��ַ
			usPeerMCUPort	-�Զ�MCU�˿�
	*����� ��
	*����ֵ���ɹ�����0��ʧ�ܷ���-1
	*****************************************************************************/
	virtual int Open(const char*cszPeerNodeID,const char*cszPeerNATIP,const char*cszPeerLocalIP,unsigned short nPeerLocalPort,const char*cszPeerMCUID,const char*cszPeerMCUIP,unsigned short nPeerMCUPort)=0;

	/******************************************************************************
	* Close
	*�������ر�
	*���룺	��
	*����� ��
	*����ֵ����
	*****************************************************************************/
	virtual void Close(void)=0;

	/******************************************************************************
	* SetAudioID
	*������������Ƶ��ʶ
	*���룺	ulAudioID	-��Ƶ��ʶ
	*����� ��
	*����ֵ���ɹ�����0��ʧ�ܷ���-1
	*****************************************************************************/
	virtual int SetAudioID(unsigned int ulAudioID)=0;

	/******************************************************************************
	* StartAudio
	*������������Ƶ
	*���룺	��
	*����� ��
	*����ֵ���ɹ�����0��ʧ�ܷ���-1
	*****************************************************************************/
	virtual int StartAudio(void)=0;

	/******************************************************************************
	* StopAudio
	*������ֹͣ��Ƶ
	*���룺	��
	*����� ��
	*����ֵ����
	*****************************************************************************/
	virtual void StopAudio(void)=0;

	/******************************************************************************
	* IsAudioStarted
	*��������ȡ�Ƿ���������Ƶ
	*���룺	��
	*����� ��
	*����ֵ���ɹ�����>=0��0:δ�����������Ѿ������ˡ���ʧ�ܷ���-1
	*****************************************************************************/
	virtual int IsAudioStarted(void)=0;

	/******************************************************************************
	* EnableAudio
	*�����������Ƿ�ʼ������Ƶ
	*���룺	nEnable			-�Ƿ�ʼ������Ƶ��0��ֹͣ������Ƶ������ʼ������Ƶ��
	*����� ��
	*����ֵ����
	*****************************************************************************/
	virtual int EnableAudio(int nEnable)=0;

	/******************************************************************************
	* IsAudioEnable
	*��������ȡ�Ƿ�ʼ����������Ƶ
	*���룺	��
	*����� ��
	*����ֵ���ɹ�����>=0��0:�Ѿ���ʼ���գ��Ѿ�ֹͣ���ա���ʧ�ܷ���-1
	*****************************************************************************/
	virtual int IsAudioEnable(void)=0;

	/******************************************************************************
	* GetAudioPacketStatistics
	*��������ȡ��Ƶ�İ���ͳ������
	*���룺	��
	*����� ulTotalPackets		-�ܹ������˶��ٸ���
			ulLostPackets		-�ܹ������˶��ٸ���
			AvgPacketLossRate	-ƽ��������
			CurPacketLossRate	-��ǰ������
	*����ֵ���ɹ�����0��ʧ�ܷ���-1
	*****************************************************************************/
	virtual int GetAudioPacketStatistics(unsigned int&ulTotalPackets,unsigned int&ulLostPackets,double&AvgPacketLossRate,double&CurPacketLossRate)=0;

	/******************************************************************************
	* GetAudioNetworkStatus
	*��������ȡ��Ƶ������״̬
	*���룺	��
	*����� nNetworkStatus	-������[0-100]
	*����ֵ���ɹ�����0��ʧ�ܷ���-1
	*****************************************************************************/
	virtual int GetAudioNetworkStatus(unsigned int&nNetworkStatus)=0;


	/******************************************************************************
	* SetVideoID
	*����������������Ƶ��ʶ
	*���룺	ulVideoID	-������Ƶ��ʶ
	*����� ��
	*����ֵ���ɹ�����0��ʧ�ܷ���-1
	*****************************************************************************/
	virtual int SetVideoID(unsigned int ulVideoID)=0;

	/******************************************************************************
	* StartVideo
	*����������������Ƶ
	*���룺	��
	*����� ��
	*����ֵ���ɹ�����0��ʧ�ܷ���-1
	*****************************************************************************/
	virtual int StartVideo(void)=0;

	/******************************************************************************
	* StopVideo
	*������ֹͣ������Ƶ
	*���룺	��
	*����� ��
	*����ֵ����
	*****************************************************************************/
	virtual void StopVideo(void)=0;

	/******************************************************************************
	* IsVideoStarted
	*��������ȡ�Ƿ�������������Ƶ
	*���룺	��
	*����� ��
	*����ֵ���ɹ�����>=0��0:δ�����������Ѿ������ˡ���ʧ�ܷ���-1
	*****************************************************************************/
	virtual int IsVideoStarted(void)=0;

	/******************************************************************************
	* EnableVideo
	*�����������Ƿ�ʼ������Ƶ
	*���룺	nEnable			-�Ƿ�ʼ������Ƶ��0��ֹͣ������Ƶ������ʼ������Ƶ��
	*����� ��
	*����ֵ����
	*****************************************************************************/
	virtual int EnableVideo(int nEnable)=0;

	/******************************************************************************
	* IsVideoEnable
	*��������ȡ�Ƿ�ʼ����������Ƶ
	*���룺	��
	*����� ��
	*����ֵ���ɹ�����>=0��0:�Ѿ���ʼ���գ��Ѿ�ֹͣ���ա���ʧ�ܷ���-1
	*****************************************************************************/
	virtual int IsVideoEnable(void)=0;

	virtual int SendCtrl(void*pData,int nLen)=0;
	virtual int SendPTZ(void*pData,int nLen)=0;

	/******************************************************************************
	* GetVideoPacketStatistics
	*��������ȡ��Ƶ�İ���ͳ������
	*���룺	��
	*����� ulTotalPackets		-�ܹ������˶��ٸ���
			ulLostPackets		-�ܹ������˶��ٸ���
			AvgPacketLossRate	-ƽ��������
			CurPacketLossRate	-��ǰ������
	*����ֵ���ɹ�����0��ʧ�ܷ���-1
	*****************************************************************************/
	virtual int GetVideoPacketStatistics(unsigned int&ulTotalPackets,unsigned int&ulLostPackets,double&AvgPacketLossRate,double&CurPacketLossRate)=0;

	/******************************************************************************
	* GetVideoNetworkStatus
	*��������ȡ��Ƶ������״̬
	*���룺	��
	*����� nNetworkStatus	-������[0-100]
	*����ֵ���ɹ�����0��ʧ�ܷ���-1
	*****************************************************************************/
	virtual int GetVideoNetworkStatus(unsigned int&nNetworkStatus)=0;

	/******************************************************************************
	* RequestKeyFrame
	*����������ؼ�֡
	*���룺	��
	*����� ��
	*����ֵ���ɹ�����0��ʧ�ܷ���-1
	*****************************************************************************/
	virtual int RequestKeyFrame(void)=0;

	/******************************************************************************
	* SetFrameRateControlMode
	*����������֡�ʿ���ģʽ
	*���룺	nMode			-֡�ʿ���ģʽ
	*����� ��
	*����ֵ���ɹ�����0��ʧ�ܷ���-1
	*****************************************************************************/
	virtual int SetFrameRateControlMode(FRAME_RATE_CONTROL_MODE nMode)=0;

	/******************************************************************************
	* SetFrameTypeControlMode
	*����������֡���Ϳ���ģʽ
	*���룺	nMode			-֡���Ϳ���ģʽ
	*����� ��
	*����ֵ���ɹ�����0��ʧ�ܷ���-1
	*****************************************************************************/
	virtual int SetFrameTypeControlMode(FRAME_TYPE_CONTROL_MODE nMode)=0;


	static XNetMediaReceiver*Create(XNetMediaReceiverCallback&rCallback);
};

#endif
