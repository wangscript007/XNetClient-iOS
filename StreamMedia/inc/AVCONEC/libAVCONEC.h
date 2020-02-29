//libAVCONEC.h
#ifndef __LIBAVCONEC_H__
#define __LIBAVCONEC_H__

#include <AVCONEC/AVCONECC.h>
#include <VIDEC/VIDEC.h>


AFX_EXT_CLASS bool IsAudioHardware(int nChannelIndex);
AFX_EXT_CLASS void SetAudioRecordDevice(unsigned int nDevice);
AFX_EXT_CLASS unsigned int GetAudioRecordDevice(void);
AFX_EXT_CLASS void SetAudioPlayDevice(unsigned int nDevice);

//�ж��Ǹ����豸
AFX_EXT_CLASS bool IsHDDevice(int nDevID);


//{{xmc add 20070830
AFX_EXT_CLASS int SD_GetFormatCount(void);
AFX_EXT_CLASS bool SD_GetFormatName(int nIndex,char*szName);
AFX_EXT_CLASS bool SD_GetFormatSize(int nIndex,int&nWidth,int&nHeight);

AFX_EXT_CLASS int HD_GetFormatCount(void);
AFX_EXT_CLASS bool HD_GetFormatName(int nIndex,char*szName);
AFX_EXT_CLASS bool HD_GetFormatSize(int nIndex,int&nWidth,int&nHeight);
//}}


class IHPVideoWindow
{
public:
	IHPVideoWindow(void){};
	virtual~IHPVideoWindow(void){};
public:
	virtual unsigned short GetWidth(void)=0;
	virtual unsigned short GetHeight(void)=0;
	virtual void ShowWindow(bool bShow)=0;
	virtual bool IsWindowVisible(void)=0;
	virtual void SetWindowPos(int x,int y,int cx,int cy)=0;
	virtual HWND GetHWnd(void)=0;
};

class IHPAVCaptureNotify
{
public:
	IHPAVCaptureNotify(void){};
	virtual~IHPAVCaptureNotify(void){};
public:
	virtual void OnIHPAVCaptureNotifyOutputAudioData(unsigned char*pPacketData,int nPacketLen)=0;
	virtual void OnIHPAVCaptureNotifyOutputVideoData(unsigned char*pPacketData,int nPacketLen)=0;
	virtual void OnIHPAVCaptureNotifyOutputSubVideoData(unsigned char*pPacketData,int nPacketLen)=0;
	virtual void OnIHPAVCaptureNotifyOutputQSubVideoData(unsigned char*pPacketData,int nPacketLen)=0;
	//nStreamType[0x01:main 0x02:sub 0x04:qsub]
	virtual bool OnIHPAVCaptureNotifyVideoCapture(int nStreamType)=0;//return bool,if true,encoder current frame,else skip it
	virtual void OnIHPAVCaptureNotifyMotionDetected(int*pResultList,int nResultCount)=0;//pResultList[i]==0,û�ж��������ڶ�
};

class IHPAVCapture
	: public IHPVideoWindow
{
public:
	IHPAVCapture(void){};
	virtual~IHPAVCapture(void){};
public:
	virtual void ReleaseConnections(void)=0;

	virtual void OpenAudioCapture(HP_AUDIO_CODEC_TYPE HPAudioCodec,bool bUseMic=false)=0;
	virtual bool HasAudioCapture(void)=0;
	virtual void ResetAudioParam(HP_AUDIO_CODEC_TYPE HPAudioCodec,bool bUseMic=false)=0;
	virtual void CloseAudioCapture(void)=0;
	virtual int GetAudioBitrate(void)=0;//����ÿ��2000�������һ��
	virtual void PlayDTMF(int nEventNumber)=0;
	virtual int GetAudioLevel(void)=0;//����ÿ��200�������һ��

	/*
	bEnableSub:�Ƿ���������,ȱʡΪfalse
	nSubScaleType:����ͼ����С���ʣ�0:����С,1:��С1/4,2:��С1/8,ȱʡΪ1
	nSubQuality:����ͼ������,����ͬ����ͼ���������壬ȱʡΪ0:64kbps
	nNetworkType,�μ�VIDEC.h�е�VIDEC_BITRATE_CONTROL_VBR�ȶ���
	*/
	virtual void OpenVideoCapture(int nVideoCaptureDevice,int nWidth,int nHeight,int nFrameRate,int nBitrate,VIDEC_CODEC_TYPE codecType,int nNetworkType,bool bSub=false,bool bQSub=false)=0;
	virtual bool HasVideoCapture(void)=0;
	virtual void ResetVideoCaptureParam(int nVideoCaptureDevice,int nWidth,int nHeight,int nFrameRate,int nBitrate,VIDEC_CODEC_TYPE codecType,int nNetworkType,bool bSub=false,bool bQSub=false)=0;
	virtual void CloseVideoCapture(void)=0;
	virtual void CaptureVideoKeyFrame(int nKeyFrame)=0;
	virtual void Denoise(bool bDenoise)=0;
	virtual bool IsDenoised(void)=0;
	virtual int GetVideoBitrate(void)=0;//����ÿ��2000�������һ��
	virtual bool HasVideoInputWindow(void)=0;
	virtual void ShowVideoInputWindow(void)=0;
	virtual bool HasVideoAdjustWindow(void)=0;
	virtual void ShowVideoAdjustWindow(void)=0;
	virtual void InvertVideo(bool bInvert)=0;
	virtual bool IsVideoInverted(void)=0;
	virtual bool CaptureToJPGFile(const char*cszJPGFile)=0;

	virtual bool GetMotionImageSize(int&nWidth,int&nHeight)=0;
	virtual bool StartMotionDetect(void)=0;
	virtual void StopMotionDetect(void)=0;
	virtual bool SetupMotionDetect(	int iGrade/*�����ȵȼ�0-6*/,
									int iFastMotionDetectFps/*�����˶�����֡��� 0-12 ȱʡ2 0�������*/,
									int iSlowMotionDetectFps/*�����˶�����֡��� 13-30 ȱʡ18 0�������*/,
									RECT *RectList, int iRectCount,bool bSimple=false)=0;

	virtual bool TVTunerPutChannel(int lChannel/*1-n*/)=0;

	//�������ڲ���������
	virtual bool SetupMask(RECT *RectList, int iRectCount)=0;
	//ֹͣ����
	virtual bool StopMask(void)=0;

	virtual bool CanSetVideoParam(void)=0;
	virtual void SetVideoParam(int nBrightness=128/*����ֵ0-255*/, int nContrast=64/*�Աȶ�0-127*/, int nSaturation=64/*���Ͷ�0-127*/, int nHue=0/*ɫ��0-255*/,int nGamma=100/*٤��1-400*/,int nSharpen=0/*��0-127*/,bool bSharpenLocalOnly=true)=0;
	
	//int nGammaR[1-400 100]
	//int nGammaG[1-400 100]
	//int nGammaB[1-400 100]
	virtual void SetVideoGammaRGB(int nGammaR,int nGammaG,int nGammaB)=0;

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
};


AFX_EXT_CLASS IHPAVCapture*CreateIHPAVCapture(IHPAVCaptureNotify&rIHPAVCaptureNotify
#ifdef _WIN32
											  ,HWND hWndVideo
#endif
											  );


class IHPAVPlayer
	: public IHPVideoWindow
{
public:
	IHPAVPlayer(void){};
	virtual~IHPAVPlayer(void){};
public:
	virtual void ReleaseConnections(void)=0;
	virtual void InputAudioPacket(unsigned char*pPacketData,int nPacketLen)=0;
	virtual void InputVideoPacket(unsigned char*pPacketData,int nPacketLen)=0;

	virtual bool HasAudio(void)=0;
	virtual void Mute(bool bMute)=0;
	virtual bool IsMute(void)=0;
	virtual void SetVolume(int nNewVolume)=0;
	virtual int GetVolume(void)=0;
	virtual unsigned int GetAudioLostCount(void)=0;
	virtual int GetAudioBitrate(void)=0;
	virtual int GetAudioLevel(void)=0;//����ÿ��200�������һ��

	virtual bool HasVideo(void)=0;
	virtual unsigned int GetVideoLostCount(void)=0;
	virtual int GetVideoBitrate(void)=0;

	virtual bool CaptureToJPGFile(const char*cszJPGFile)=0;

	virtual void EnableSyn(bool bEnableSyn)=0;
	virtual void SetVideoParam(int nBrightness=128/*����ֵ0-255*/, int nContrast=64/*�Աȶ�0-127*/, int nSaturation=64/*���Ͷ�0-127*/, int nHue=0/*ɫ��0-255*/,int nGamma=100/*٤��1-400*/,int nSharpen=0/*��0-127*/)=0;
	
	//int nGammaR[1-400 100]
	//int nGammaG[1-400 100]
	//int nGammaB[1-400 100]
	virtual void SetVideoGammaRGB(int nGammaR,int nGammaG,int nGammaB)=0;

	//��ȡ�Ƿ�Ӳ�����������ʾ�û�������������ֱ��ʵ����ƣ��޷����벢��ʾ����Ƶ����
	virtual bool GetHardwareError(void)=0;

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

AFX_EXT_CLASS IHPAVPlayer*CreateIHPAVPlayer(HWND hWndVideo);

#endif