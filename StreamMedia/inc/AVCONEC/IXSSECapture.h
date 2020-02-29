#ifndef __IXSSECAPTURE_H__
#define __IXSSECAPTURE_H__

#include <AVCONEC/AVCONECC.h>

class IXSSECaptureNotify
{
public:
	IXSSECaptureNotify(void){};
	virtual~IXSSECaptureNotify(void){};
public:
	virtual void OnIXSSECaptureNotifyOutputAudioData(unsigned char*pPacketData,int nPacketLen)=0;
	virtual void OnIXSSECaptureNotifyOutputVideoData(unsigned char*pPacketData,int nPacketLen)=0;
	virtual bool OnIXSSECaptureNotifyVideoCapture(bool bSub)=0;//return bool,if true,encoder current frame,else skip it
};

class IXSSECapture
{
public:
	IXSSECapture(void){};
	virtual ~IXSSECapture(void){};
public:
	virtual void ReleaseConnections(void)=0;

	virtual void OpenAudioCapture(HP_AUDIO_CODEC_TYPE HPAudioCodec)=0;
	virtual bool HasAudioCapture(void)=0;
	virtual void ResetAudioParam(HP_AUDIO_CODEC_TYPE HPAudioCodec)=0;
	virtual void CloseAudioCapture(void)=0;
	virtual int GetAudioBitrate(void)=0;//����ÿ��2000�������һ��
	virtual int GetAudioLevel(void)=0;//����ÿ��200�������һ��

	//nBitrateControlType,�μ�VIDEC.h�е�VIDEC_BITRATE_CONTROL_VBR�ȶ���
	virtual void OpenVideoCapture(int x,int y,int cx,int cy,int nBitCount/*16,24,32*/,int nFrameRate,int nQuality,VIDEC_CODEC_TYPE codecType,int nNetworkType)=0;
	virtual bool HasVideoCapture(void)=0;
	virtual void ResetVideoCaptureParam(int x,int y,int cx,int cy,int nBitCount/*16,24,32*/,int nFrameRate,int nQuality,VIDEC_CODEC_TYPE codecType,int nNetworkType)=0;
	virtual void CloseVideoCapture(void)=0;
	virtual void RequestKeyFrame(void)=0;
	virtual int GetVideoBitrate(void)=0;//����ÿ��2000�������һ��


	/******************************************************************************
	* EnableOSD
	*�����������Ƿ���ʾOSD
	*���룺	bEnable			-�Ƿ���ʾOSD,Ϊtrueʱ��ʾ��������ʾ
	*�������
	*����ֵ����
	*****************************************************************************/
	virtual void EnableOSD(bool bEnable)=0;

	/******************************************************************************
	* IsOSDEnable
	*��������ȡ�Ƿ���ʾOSD
	*���룺��
	*�������
	*����ֵ��Ϊtrueʱ��ʾ��������ʾ
	*****************************************************************************/
	virtual bool IsOSDEnable(void)=0;

	/******************************************************************************
	* SetOSDText
	*����������OSD���ı�
	*���룺	cszText			-Ҫ��ʾ���ı�
			nAlign			-���뷽ʽ���ο�VIDEC.h�е�VIDEC_OSD_ALIGN_TOP_LEFT�Ķ���
	*�������
	*����ֵ����
	*****************************************************************************/
	virtual void SetOSDText(const TCHAR*cszText,int nAlign=0)=0;

	/******************************************************************************
	* SetOSDFont
	*����������OSD������
	*���룺	logfont			-OSD������
			crText			-OSD���ı�����ɫ
			crBk			-OSD���ı��ı�����ɫ
	*�������
	*����ֵ����
	*****************************************************************************/
	virtual void SetOSDFont(LOGFONT logfont,COLORREF crText=RGB(0,0,255),COLORREF crBk=RGB(255,255,255))=0;

	virtual void GetScreenRect(int& x,int& y,int& cx,int& cy)=0;

	/******************************************************************************
	* StartRecordToAVI
	*��������ʼ¼��
	*���룺	cszPathName		-¼���ļ�����
			nStandardFormat	-�Ƿ�¼�Ƴɱ�׼�ļ���ʽ
	*�������
	*����ֵ���ɹ�����0��ʧ�ܷ���-1
	*****************************************************************************/
	virtual int StartRecordToAVI(const char*cszPathName,int nStandardFormat=1)=0;
	
	/******************************************************************************
	* StopRecordToAVI
	*������ֹͣ¼��
	*���룺��
	*�������
	*����ֵ����
	*****************************************************************************/
	virtual void StopRecordToAVI(void)=0;


	/******************************************************************************
	* SetScreenWnd
	*����������������Ļ����λ��
	*���룺	nScreenIndex	-��Ļ��
			nWndIndex		-���ں�
			nFullScreen		-�Ƿ�ȫ����ʾ��0����ͨ��ʾ����0��ȫ����ʾ��
	*�������
	*����ֵ����
	*****************************************************************************/
	virtual void SetScreenWnd(int nScreenIndex,int nWndIndex,int nFullScreen)=0;
};

AFX_EXT_CLASS IXSSECapture*CreateIXSSECapture(IXSSECaptureNotify&rIXSSECaptureNotify);

#endif
