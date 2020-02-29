#ifndef __LIBXMEDIAFILECAPTURE_H__
#define __LIBXMEDIAFILECAPTURE_H__

#include <AVCONEC/libAVCONEC.h>

class IXMediaFileCaptureNotify
	: public IHPAVCaptureNotify
{
public:
	IXMediaFileCaptureNotify(void){};
	virtual~IXMediaFileCaptureNotify(void){};
public:
	virtual void OnIXMediaFileCaptureNotifyFilePositionChanged(unsigned int ulSeconds)=0;
	virtual void OnIXMediaFileCaptureNotifyFilePause(void)=0;
	virtual void OnIXMediaFileCaptureNotifyFilePlay(void)=0;
	virtual void OnIXMediaFileCaptureNotifyFileEnd(void)=0;
};

class IXMediaFileCapture
	: public IHPVideoWindow
{
public:
	IXMediaFileCapture(void){};
	virtual~IXMediaFileCapture(void){};
public:
	virtual void ReleaseConnections(void)=0;
	virtual unsigned int GetDuration(void)=0;
	virtual unsigned int GetPosition(void)=0;
	virtual void Seek(unsigned int ulSeconds)=0;
	virtual void Pause(void)=0;
	virtual void Play(void)=0;
	virtual bool IsPlaying(void)=0;
	virtual void SetVolume(int nVolume)=0;
	virtual int GetVolume(void)=0;
	virtual void SetMute(bool bMute=true)=0;
	virtual bool GetMute(void)=0;
	virtual void SetBalance(int nBalance)=0;
	virtual int GetBalance(void)=0;
	virtual int GetAudioLevel(void)=0;//����ÿ��200�������һ��

	virtual bool HasAudio(void)=0;
	virtual bool HasVideo(void)=0;
	virtual int GetAudioBitrate(void)=0;
	virtual void CaptureVideoKeyFrame(bool bSub)=0;
	virtual int GetVideoBitrate(void)=0;

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

	//nNetworkType,�μ�VIDEC.h�е�VIDEC_BITRATE_CONTROL_VBR�ȶ���
	static IXMediaFileCapture*Create(IXMediaFileCaptureNotify&rNotify,HWND hWnd,const char*cszMediaFile,int nFrameRate,int nQuality,VIDEC_CODEC_TYPE codecType,bool bLoop,int nNetworkType);
};


#endif